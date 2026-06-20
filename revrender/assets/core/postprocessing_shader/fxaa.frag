#version 460

out vec4 FragColor;

in vec2 u_TexCoords;

layout (binding = 0) uniform sampler2D u_SceneTexture;
uniform vec2 u_Resolution;

#define EDGE_THRESHOLD_MAX 0.125
#define EDGE_THRESHOLD_MIN 0.0312
#define ITERATIONS 12
#define SUBPIXEL_QUALITY 0.75

float rgb2luma(vec3 rgb){
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

float QUALITY(int i){
    if(i <=5 && i > 1){
        return 2.0;
    }else if(i > 5){
        return pow(2.0, i-4);
    }else {
        return 1.5;
    }
}

vec3 applyFXAA(sampler2D _texture, vec2 uv, vec2 res){
    vec2 inverse_resolution = vec2(1.0) /res;
    vec3 colorCenter = texture(_texture, uv).rgb;

    float lumaCenter = rgb2luma(colorCenter);

    float lumaDown = rgb2luma(textureOffset(_texture, uv, ivec2(0,-1)).rgb);
    float lumaUp = rgb2luma(textureOffset(_texture, uv, ivec2(0,1)).rgb);
    float lumaLeft = rgb2luma(textureOffset(_texture, uv, ivec2(-1,0)).rgb);
    float lumaRight = rgb2luma(textureOffset(_texture, uv, ivec2(1,0)).rgb);

    float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));;
    float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

    float lumaRange = lumaMax - lumaMin;

    if(lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax*EDGE_THRESHOLD_MAX)){
        return colorCenter;
    }

    float lumaDownLeft = rgb2luma(textureOffset(_texture, uv, ivec2(-1,-1)).rgb);
    float lumaDownRight = rgb2luma(textureOffset(_texture, uv, ivec2(1,-1)).rgb);
    float lumaUpLeft = rgb2luma(textureOffset(_texture, uv, ivec2(-1,1)).rgb);
    float lumaUpRight = rgb2luma(textureOffset(_texture, uv, ivec2(1,1)).rgb);

    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    float edgeHorizontal =  abs(-2.0 * lumaLeft + lumaLeftCorners)  + abs(-2.0 * lumaCenter + lumaDownUp ) * 2.0    + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical =    abs(-2.0 * lumaUp + lumaUpCorners)      + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0  + abs(-2.0 * lumaDown + lumaDownCorners);

    bool isHorizontal = (edgeHorizontal >= edgeVertical);


    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;

    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    bool is1steepest = abs(gradient1) >= abs(gradient2);

    float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

    float stepLength = isHorizontal ? inverse_resolution.y : inverse_resolution.x;
    float lumaLocalAverage = 0.0;
    if(is1steepest){
        stepLength = -stepLength;
        lumaLocalAverage = 0.5 * (luma1 + lumaCenter);
    }else{
        lumaLocalAverage = 0.5 * (luma2 + lumaCenter);
    }

    vec2 currentUv = uv;
    if(isHorizontal){
        currentUv.y += stepLength*0.5;
    }else{
        currentUv.x += stepLength*0.5;
    }

    vec2 offset = isHorizontal ? vec2(inverse_resolution.x, 0.0) : vec2(0.0, inverse_resolution.y);
    vec2 uv1 = currentUv - offset;
    vec2 uv2 = currentUv + offset;

    float lumaEnd1 = rgb2luma(texture(_texture, uv1).rgb);
    float lumaEnd2 = rgb2luma(texture(_texture, uv2).rgb);
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if(!reached1){
        uv1 -= offset;
    }
    if(!reached2){
        uv2 += offset;
    }
    if(!reachedBoth){
        for(int i = 2; i < ITERATIONS; i++){
            if(!reached1){
                lumaEnd1 = rgb2luma(texture(_texture, uv1).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
            if(!reached2){
                lumaEnd2 = rgb2luma(texture(_texture, uv2).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;
            if(!reached1){
                uv1 -= offset * QUALITY(i);
            }
            if(!reached2){
                uv2 += offset * QUALITY(i);
            }
            if(reachedBoth) { break; }
        }
    }

    float distance1 = isHorizontal ? (uv.x - uv1.x) : (uv.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - uv.x) : (uv2.y - uv.y);

    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    float edgeThickness = (distance1 + distance2);

    float pixelOffset = -distanceFinal / edgeThickness + 0.5;

    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

    float finalOffset = correctVariation ? pixelOffset : 0.0;

    float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaRange,0.0,1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;
    finalOffset = max(finalOffset,subPixelOffsetFinal);

    vec2 finalUv = uv;
    if(isHorizontal){
        finalUv.y += finalOffset * stepLength;
    } else {
        finalUv.x += finalOffset * stepLength;
    }
    return texture(_texture, finalUv).rgb;
}

void main(){
    vec2 resolution = u_Resolution;
    vec3 antiAliasedColor = applyFXAA(u_SceneTexture, u_TexCoords, resolution);
    FragColor = vec4(antiAliasedColor, 1.0);
}
