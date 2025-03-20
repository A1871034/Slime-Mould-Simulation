#pragma once

#include <string>

struct ShaderSource {
    std::string VertexShader;
    std::string FragmentShader;
    std::string ComputeShader;
};

class Shaders {
    unsigned int vs;
    unsigned int fs;
    unsigned int cs;

    std::string m_filepath;

    public:
    Shaders(const std::string& filepath);
    ~Shaders();

    unsigned int compile_shader(unsigned int type, const std::string& source);
    void compile_shaders(ShaderSource ss);
    unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& computeShader);
    ShaderSource parse_shader(const std::string& filepath);

    inline unsigned int get_vs() const {return vs;}
    inline unsigned int get_fs() const {return fs;}
    inline unsigned int get_cs() const {return cs;}
};