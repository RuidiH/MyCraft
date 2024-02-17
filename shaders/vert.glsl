#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vTexture;
layout(location=2) in vec3 vNormal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_LightProjection;

out vec3 fPos;
out vec2 fTexCoord;
out vec3 fNormal;
out vec4 fPosLight;

void main()
{
    fPos = vec3(u_ModelMatrix * vec4(vPosition, 1.0));
    fTexCoord = vTexture;
    fNormal = mat3(transpose(inverse(u_ModelMatrix))) * vNormal;
    gl_Position = u_Projection * u_View * vec4(fPos, 1.0);
    fPosLight = u_LightProjection * u_ModelMatrix * vec4(vPosition, 1.0);
}