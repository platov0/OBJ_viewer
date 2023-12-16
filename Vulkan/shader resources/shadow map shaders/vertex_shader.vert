#version 450

layout(binding = 0) uniform UniformBufferObject
{
    mat4 scale;
    mat4 centering;
    mat4 rotate;
    mat4 translate;
    mat4 look_at;
    mat4 project;
} shm;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out float depth;

void main()
{
    gl_Position = shm.project * shm.look_at * shm.translate * shm.rotate * shm.centering * shm.scale * vec4(inPosition, 1.0);
    depth = gl_Position[2];
}