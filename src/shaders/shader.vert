#version 450

layout(binding = 0) uniform VertUniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat3 normalMatrix;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 worldPos;
layout(location = 3) out vec3 normal;

void main() {
    worldPos = vec3(ubo.model * vec4(inPosition, 1.0));

    gl_Position = ubo.proj * ubo.view * vec4(worldPos, 1.0);

    fragColor = inColor;
    fragTexCoord = inTexCoord;

    // normal = inNormal;
    mat3 normmat = mat3(transpose(inverse(ubo.model)));
    normal = normmat* inNormal;
    
}
