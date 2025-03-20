#include<GL/glew.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#include "core.h"
#include "Shaders.h"

Shaders::Shaders(const std::string &filepath) : m_filepath(filepath) {
    ShaderSource sources = parse_shader(filepath);
    compile_shaders(sources);
}

Shaders::~Shaders() {
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(cs);
}

unsigned int Shaders::compile_shader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int compile_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile ";
        switch (type) {
            case GL_VERTEX_SHADER:
                std::cerr << "Vertex";
                break;
            case GL_FRAGMENT_SHADER:
                std::cerr << "Fragment";
                break;
            case GL_COMPUTE_SHADER:
                std::cerr << "Compute";
                break;
            default:
                std::cerr << "Unkown" << std::endl;
        };
        std::cerr << "Shader" << std::endl;
        std::cerr << message << std::endl;
        ASSERT("");
        glDeleteShader(id);
        return 0;        
    } 

    return id;
}

void Shaders::compile_shaders(ShaderSource ss) {
    vs = compile_shader(GL_VERTEX_SHADER, ss.VertexShader);
    fs = compile_shader(GL_FRAGMENT_SHADER, ss.FragmentShader);
    cs = compile_shader(GL_COMPUTE_SHADER, ss.ComputeShader);
}

ShaderSource Shaders::parse_shader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
        COMPUTE = 2,
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            } else if (line.find("compute") != std::string::npos) {
                type = ShaderType::COMPUTE;
            }
        } else if (type != ShaderType::NONE) {
            ss[(int) type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str(), ss[2].str()};
}


