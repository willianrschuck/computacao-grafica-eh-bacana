#version 450 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 Normal;

in FragData {
    vec3 FragPos;
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fragData;

struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D normalMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    float cutOff;
    float outerCutOff;

    vec3 direction;
    vec3 ambient;
    vec3 diffuse;

    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};


in Light theLight;

uniform Material material;

in mat3 TBN;

vec4 normalLight();

void main() {

    vec3 alightDir = normalize(theLight.position - fragData.FragPos);

    vec3 lightDir = normalize(fragData.TangentLightPos - fragData.TangentFragPos);

    float theta = dot(alightDir, normalize(-theLight.direction));
    float epsilon = theLight.cutOff - theLight.outerCutOff;
    float intensity = clamp((theta - theLight.outerCutOff) / epsilon, 0.0, 1.0);

    if (theta > theLight.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {
        vec3 normal = texture(material.normalMap, fragData.TexCoord).rgb;
        // transform normal vector to range [-1,1]
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

        // get diffuse color
        vec3 color = texture(material.diffuseTexture, fragData.TexCoord).rgb;
        // ambient
        vec3 ambient = 0.1 * color;

        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 viewDir = normalize(fragData.TangentViewPos - fragData.TangentFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        vec3 specular = theLight.specular * spec * texture(material.specularTexture, fragData.TexCoord).rgb;


        float distance    = length(theLight.position - fragData.FragPos);
        float attenuation = 1.0 / (theLight.constant + theLight.linear * distance + theLight.quadratic * (distance * distance));

        ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse  *= attenuation * (1-intensity);
        specular *= attenuation * (1-intensity);

        //vec3 specular = vec3(0.2) * spec;
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
//    FragColor = vec4(abs(TBN[0]), 1.0);
    else {
        FragColor = vec4(0.0);
    }
//    FragColor = vec4(light.ambient * texture(material.diffuseTexture, fragData.TexCoord).rgb, 1.0);
}

//vec4 normalLight() {

//    vec3 lightDir = normalize(light.position - fragData.FragPos);
//
//    float theta = dot(lightDir, normalize(-light.direction));
//
//    vec3 ambient = light.ambient * texture(material.diffuseTexture, fragData.TexCoord).rgb;
//
//    // diffuse
//    vec3 norm = normalize(Normal);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, fragData.TexCoord).rgb;
//
//    // specular
//    vec3 viewDir = normalize(viewPos - fragData.FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = light.specular * spec * texture(material.specularTexture, fragData.TexCoord).rgb;
//
//    // attenuation
//    float distance    = length(light.position - fragData.FragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//
//    // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
//    diffuse   *= attenuation;
//    specular *= attenuation;
//
//    vec3 result = ambient + diffuse + specular;
//    return vec4(result, 1.0);

//}