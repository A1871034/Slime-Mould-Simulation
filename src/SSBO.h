#pragma once

#include <GL/glew.h>

class SSBO {
    unsigned int m_renderer_id;
    unsigned int m_index;

    public:
    SSBO(unsigned int index, const void* data, unsigned int size, GLenum usage);
    SSBO() {};
    ~SSBO();

    void binding() const;
    void binding(int slot) const;
    void bind() const;
    void unbind() const;

};