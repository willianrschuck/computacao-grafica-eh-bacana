#version 450 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;


struct Material {
    sampler2D diffuseTexture;
    sampler2D specularTexture;
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

uniform Material material;
uniform Light light;

void main() {

    vec3 lightDir = normalize(light.position - FragPos);

    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));

    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoord).rgb;

        // diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoord).rgb;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoord).rgb;

        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);

        // FragColor = texture(ourTexture, TexCoord);
    } else {

        FragColor = vec4(light.ambient * texture(material.diffuseTexture, TexCoord).rgb, 1.0);

    }


//     FragColor = vec4(vec3(gl_FragCoord.z), 1.0);

}