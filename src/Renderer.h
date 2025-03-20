#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "Program.h"
#include "SSBO.h"

/* 
        ideal order in terms of no attraction to particles
            Move particles
            Blur/Decay Trailmap
            Add Particle to trailmap
        Used order (attraction to particles and simpler more performant shaders)
            Add Particle to trailmap
            Move particles
            Blur/Decay Trailmap
        */

        

class Renderer {

    public:
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Program &program, const SSBO trails[2], bool swap_buffers) const;
    void Compute(const Program &program, const SSBO &particles, const SSBO trails[2], bool swap_buffers);
};