#pragma once

#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"

class Program {
    unsigned int m_render_id;
    std::unordered_map<std::string, unsigned int> uniform_location_cache;

    int get_uniform_location(const std::string& name);

    public:
    Program();
    ~Program();

    void attach_shader(unsigned int shader);
    void link();
    void validate();
    void use() const;

    void bind() const;
    void unbind() const;

    void set_uniform_1i(const std::string& name, int value);
    void set_uniform_1f(const std::string& name, int value);
    void set_uniform_3f(const std::string& name, float v0, float v1, float v2);
    void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
    void set_uniform_mat4f(const std::string& name, const glm::mat4& mat);
};