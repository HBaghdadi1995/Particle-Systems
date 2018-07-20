#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 Position;

layout(location = 5) in float LifeTime;

layout(location = 0) out vec3 fragColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
};

float x;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(Position, 1.0);
    fragColor = vec3(1.0);
    if(gl_Position.z==0){
      x = 0.01f;
    }else{
      x=1/gl_Position.z;
    }
    gl_PointSize = 20.0 * x;
}
