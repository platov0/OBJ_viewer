#ifndef MODEL_SHADERS_LOADER
#define MODEL_SHADERS_LOADER
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
class ModelShadersLoader
{
public:
    void LoadVertexShaderCode(const std::string& path)
    {
        std::ifstream shader_code_file(path, std::ios::ate | std::ios::binary);
        if (!shader_code_file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        vertex_shader_code.resize(shader_code_file.tellg());
        shader_code_file.seekg(0);
        shader_code_file.read(vertex_shader_code.data(), vertex_shader_code.size());
        shader_code_file.close();
    }
    void LoadFragmentShaderCode(const std::string& path)
    {
        std::ifstream shader_code_file(path, std::ios::ate | std::ios::binary);
        if (!shader_code_file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        fragment_shader_code.resize(shader_code_file.tellg());
        shader_code_file.seekg(0);
        shader_code_file.read(fragment_shader_code.data(), fragment_shader_code.size());
        shader_code_file.close();
    }
public:
    std::vector<char> vertex_shader_code;
    std::vector<char> fragment_shader_code;
};
#endif // !SHADERS_LOADER

