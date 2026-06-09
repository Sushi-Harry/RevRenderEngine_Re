#version 330

out vec4 FragColor;

in vec3 u_TexCoords;

uniform samplerCube skybox;

void main(){
    FragColor = texture(skybox, u_TexCoords);
}
