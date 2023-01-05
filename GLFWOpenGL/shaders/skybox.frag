#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    //pegamos nos atributos interpolados dos vertices como se fosse um vector das direccoes
    FragColor = texture(skybox, TexCoords);
}
