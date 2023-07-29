#include "shader_manager.h"
#include "shader_compiler.h"
#include "core/hash.h"

void ShaderManager::setup()
{
}

void ShaderManager::cleanup()
{
    for (auto shader_iter : shader_dict)
    {
        ez_destroy_shader(shader_iter.second);
    }
    shader_dict.clear();
}

EzShader ShaderManager::get_shader_internal(const std::string& file, const std::vector<std::string>& macros)
{
    std::size_t hash = 0;
    hash_combine(hash, file);
    for (auto& macro : macros)
    {
        hash_combine(hash, macro);
    }

    auto iter = shader_dict.find(hash);
    if (iter != shader_dict.end()) {
        return iter->second;
    }

    void* data;
    uint32_t size;
    EzShader shader;
    ShaderCompiler::get()->compile(file, macros, &data, size);
    ez_create_shader(data, size, shader);
    shader_dict[hash] = shader;
    if (data)
        free(data);
    return shader;
}

EzShader ShaderManager::get_shader(const std::string& file)
{
    std::vector<std::string> macros;
    return get_shader_internal(file, macros);
}

EzShader ShaderManager::get_shader(const std::string& file, const std::vector<std::string>& macros)
{
    return get_shader_internal(file, macros);
}