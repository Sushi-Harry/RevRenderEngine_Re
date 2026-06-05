#version 460 core

out vec4 FragColor;

in vec2 u_TexCoords;

// The uniform string we generated in C++
uniform sampler2D u_TextureDiffuse0;
uniform sampler2D u_TextureSpecular0;

void main() {
    // Sample the texture at the given UV coordinates
    vec4 texColor = texture(u_TextureDiffuse0, u_TexCoords);

    // Safety check for textures with alpha transparency
    if(texColor.a < 0.1) {
        discard;
    }

    FragColor = texColor;
}
