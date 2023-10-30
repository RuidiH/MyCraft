#version 410 core

// in vec3 v_vertexColors;

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;

void main()
{   
    // color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0f);
    color = texture(texture1, TexCoord);
}