#version 410 core

in vec3 fPos;
// in vec2 fTexCoord;
in vec4 fColor;
in vec3 fNormal;
in vec4 fPosLight;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

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
    
    // texture
    // vec3 textureColor = texture(texture1, fTexCoord).rgb;

    // calculate shadow
    float shadow = 0.0f;
    float bias = 0.0005f;
    vec3 lightCoords = fPosLight.xyz / fPosLight.w;
    if (lightCoords.z <= 1.0f) {
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float closestDepth = texture(shadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        if(currentDepth - bias > closestDepth) {
            shadow = 1.0f;
        }
    }

    vec3 incomingColor = fColor.rgb;

    vec3 result = (ambient + (1.0 - shadow) * diffuse) * incomingColor;
    color = vec4(result, 1.0); 
}