#pragma once

#include <cmath>
#include "SSBO.h"

struct particle { // May want to use doubles
    float pos_x; // 0 Left
    float pos_y; // 0 Bottom
    float rot; // Up of Right
};

class Mould {
    float *m_trails;
    particle *m_particles;
    unsigned short m_width;
    unsigned short m_height;
    SSBO trails;
    public:
    Mould(unsigned short width, unsigned short height);
    ~Mould();

    void gen_random_mould(unsigned int n_particles);
    inline float * get_trails() const {return m_trails;}   
    inline particle * get_particles() const {return m_particles;} 
    inline unsigned short get_width() const {return m_width;}
    inline unsigned short get_height() const {return m_height;}
};