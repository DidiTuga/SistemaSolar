#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    //declaramos o vector local de posicoes como output de coordenadas da textura, para ser interpolado pelo fragment shader, o fragment shader ira pegar este nestes outputs para fazer a amostragem do cubo
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  