#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap;

void main()
{   
float depthValue = texture(shadowMap, TexCoords).r;
depthValue = depthValue * 0.5 + 0.5; // Adjust as needed for visibility
FragColor = vec4(vec3(depthValue), 1.0); // Grayscale based on adjusted depth
}
