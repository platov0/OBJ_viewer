#version 450

layout(binding = 1) uniform sampler2D depth_map;

layout(location = 0) in vec4 shadow_map_vertex_position;
layout(location = 1) in float shadow_and_normal_dot;

layout(location = 0) out vec4 outColor;

void main()
{
    float vertex_depth_fragment = shadow_map_vertex_position[2];

    float shadow_map_bias = 0.004 * (1 - abs(shadow_and_normal_dot));

    // normalize shadow vertices
    vec2 uv_shadow;
    uv_shadow[0] = (shadow_map_vertex_position[0] + 1) / 2;
    uv_shadow[1] = (shadow_map_vertex_position[1] + 1) / 2;

    vec2 texel_size = 1.0 / textureSize(depth_map, 0);
    float shadow_depth = 0.0f;

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float depth = texture(depth_map, uv_shadow.xy + vec2(x, y) * texel_size)[0]; 
            if(vertex_depth_fragment > depth + shadow_map_bias)
            {
               shadow_depth += 1;
            }   
            if(vertex_depth_fragment <= depth + shadow_map_bias)
            {
               shadow_depth += 0;
            }     
        }   
    }
    
    shadow_depth /= 9.0f;

    vec4 shadow_color;
    shadow_color[0] = 0.02f;
    shadow_color[1] = 0.02f;
    shadow_color[2] = 0.02f;
    shadow_color[3] = 1.0f;

    vec4 model_color;
    model_color[0] = (abs(shadow_and_normal_dot) / 3) + 0.2;
    model_color[1] = (abs(shadow_and_normal_dot) / 3) + 0.2;
    model_color[2] = (abs(shadow_and_normal_dot) / 3) + 0.2;
    model_color[3] = 1.0f;

    outColor[0] = (shadow_color[0] * shadow_depth) + (model_color[0] * (1 - shadow_depth));
    outColor[1] = (shadow_color[1] * shadow_depth) + (model_color[1] * (1 - shadow_depth));
    outColor[2] = (shadow_color[2] * shadow_depth) + (model_color[2] * (1 - shadow_depth));
    outColor[3] = 1.0f;
}