#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out FragData {
    vec3 FragPos;
    vec2 TexCoord;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fragData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;

out mat3 TBN;
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

uniform vec3 viewPos;

uniform Light light;

out Light theLight;

void main() {
    fragData.FragPos = vec3(model * vec4(aPos, 1.0));
    fragData.TexCoord = aTexCoord;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
//    vec3 T = normalize(normalMatrix * aTangent);
//    vec3 N = normalize(normalMatrix * aNormal);
//    vec3 B = aBitangent;

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));
    fragData.TangentLightPos = TBN * light.position;
    fragData.TangentViewPos  = TBN * viewPos;
    fragData.TangentFragPos  = TBN * fragData.FragPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    theLight = light;

}