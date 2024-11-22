#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform FragUniformBufferObject {
    vec3 color;
} ubo;

layout(binding = 2) uniform sampler2D texSampler;
layout(binding = 3) uniform sampler2D normSampler;

float floatToSrgb(float value) {
    const float inv_12_92 = 0.0773993808;
    return value <= 0.04045
       ? value * inv_12_92 
       : pow((value + 0.055) / 1.055, 2.4);
}
vec3 vec3ToSrgb(vec3 value) {
    return vec3(floatToSrgb(value.x), floatToSrgb(value.y), floatToSrgb(value.z));
}

void main() {
    vec3 color;
    color = texture(texSampler, fragTexCoord).rgb;

    color = vec3ToSrgb(color);

    // outColor = vec4(color, 1.0);
    outColor = vec4(ubo.color, 1.0);


}
