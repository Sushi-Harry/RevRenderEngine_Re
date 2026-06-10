#version 330

out vec4 FragColor;

in vec3 u_TexCoords;

uniform samplerCube u_Skybox;

void main(){
    FragColor = texture(u_Skybox, u_TexCoords);
}
