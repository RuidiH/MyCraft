#version 410 core

// in vec3 v_vertexColors;

in vec3 fPos;
in vec2 fTexCoord;
in vec3 fNormal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture1;

void main()
{   
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    //vec3 result = (ambient + diffuse + specular) * objectColor;

    // texture
    vec3 textureColor = texture(texture1, fTexCoord).rgb;

    //color = vec4(result * textureColor, 1.0);

    vec3 result = (ambient + diffuse + specular) * textureColor;
    color = vec4(result, 1.0); 
}