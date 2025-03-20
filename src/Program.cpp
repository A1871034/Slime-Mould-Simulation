#include "Program.h"

#include "core.h"

int Program::get_uniform_location(const std::string &name) {
    if (uniform_location_cache.find(name) != uniform_location_cache.end()) {
        return uniform_location_cache[name];
    }

    int location = glGetUniformLocation(m_render_id, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform \"" << name << "\" not found in shader" << std::endl;
    }

    uniform_location_cache[name] = location;
    
    return location;
}

Program::Program() {
    m_render_id = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(m_render_id);
}

void Program::attach_shader(unsigned int shader) {
    glAttachShader(m_render_id, shader);
}

void Program::link() {
    glLinkProgram(m_render_id);

    GLint program_linked;
    glGetProgramiv(m_render_id, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        int length;
        glGetProgramiv(m_render_id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length*sizeof(char));
        glGetProgramInfoLog(m_render_id, length, &length, message);
        std::cerr << "----- Program Linking ERROR" << std::endl;
        std::cerr << message << std::endl << std::endl;
        std::cerr << "----- END ERROR" << std::endl << std::endl;
        ASSERT("")
    }
}

void Program::validate() {
    glValidateProgram(m_render_id);
}

void Program::use() const {
    glUseProgram(m_render_id);
}

void Program::bind() const {
    glUseProgram(m_render_id);
}

void Program::unbind() const {
    glUseProgram(0);
}

void Program::set_uniform_1i(const std::string &name, int value) {
    glUniform1i(get_uniform_location(name), value);
}

void Program::set_uniform_1f(const std::string &name, int value) {
    glUniform1f(get_uniform_location(name), value);
}

void Program::set_uniform_3f(const std::string &name, float v0, float v1, float v2) {
    glUniform3f(get_uniform_location(name), v0, v1, v2);
}

void Program::set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Program::set_uniform_mat4f(const std::string &name, const glm::mat4& mat) {
    glUniformMatrix4fv(get_uniform_location(name), 1, false, &(mat[0][0]));
}