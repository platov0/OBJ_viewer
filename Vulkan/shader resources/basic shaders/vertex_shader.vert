#version 450

layout(binding = 0) uniform ModelUniformBuffer
{
    mat4 shadow_matrix;
    mat4 scale;
    mat4 centering;
    mat4 rotate;
    mat4 translate;
    mat4 project;

    vec3 shadow_vector;
} mub;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 shadow_map_vertex_position;
layout(location = 1) out float shadow_and_normal_dot;

void main()
{
    gl_Position = mub.project * mub.translate * mub.rotate * mub.centering * mub.scale * vec4(inPosition, 1.0);

    shadow_map_vertex_position = mub.shadow_matrix * vec4(inPosition, 1.0);

    vec4 shadow_vertex_normal_4;
    vec3 shadow_vertex_normal_3;

    shadow_vertex_normal_4 = mub.rotate * vec4(inNormal, 1.0f);

    shadow_vertex_normal_3[0] = shadow_vertex_normal_4[0];
    shadow_vertex_normal_3[1] = shadow_vertex_normal_4[1];
    shadow_vertex_normal_3[2] = shadow_vertex_normal_4[2];

    shadow_vertex_normal_3 = normalize(shadow_vertex_normal_3);
    shadow_and_normal_dot = dot(mub.shadow_vector, shadow_vertex_normal_3);
}