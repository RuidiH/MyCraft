#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texture;
// layout(location=1) in vec3 vertexColors;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat3 u_Normal;

// out vec3 v_vertexColors;
out vec2 TexCoord;

void main()
{
    vec4 newPosition = u_Projection * u_View * u_ModelMatrix * vec4(position, 1.0f); 
    
    gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);

    // v_vertexColors = vertexColors;
    TexCoord = texture;
}