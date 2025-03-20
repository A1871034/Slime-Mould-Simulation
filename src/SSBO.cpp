#include "SSBO.h"

SSBO::SSBO(unsigned int index, const void *data, unsigned int size, GLenum usage) 
    : m_index(index)
{
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderer_id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
}

SSBO::~SSBO() {
    glDeleteBuffers(1, &m_renderer_id);
}

void SSBO::binding() const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_index, m_renderer_id);
}

void SSBO::binding(int slot) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_renderer_id);
}

void SSBO::bind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_renderer_id);
}

void SSBO::unbind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
