#include "Renderer.h"

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Program &program, const SSBO trails[2], bool swap_buffers) const
{
    program.bind();
    va.bind();
    if (swap_buffers) {
        trails[0].binding(4);
        trails[1].binding(3);
    } else {
        trails[0].binding(3);
        trails[1].binding(4);
    }
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Compute(const Program &program, const SSBO &particles, const SSBO trails[2], bool swap_buffers) {
    program.bind();
    program.use();
    if (swap_buffers) {
        trails[0].binding(4);
        trails[1].binding(3);
    } else {
        trails[0].binding(3);
        trails[1].binding(4);
    }
    particles.binding();
    
    unsigned int workgroup_size = 32;

    glDispatchCompute(workgroup_size, 1, 1);

    // To wait for compute shader to finish
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}
