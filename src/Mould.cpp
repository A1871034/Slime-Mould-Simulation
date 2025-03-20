#include "Mould.h"

#include <cstdlib>
#include <iostream>

Mould::Mould(unsigned short width, unsigned short height) 
    : m_width(width), m_height(height) {
    // calloc instead of malloc to set everything to 0
    m_trails = (float*) calloc((m_height+1) * (m_width+1), sizeof(float));
    
    // Do Trail Map initialisation here
    /*
    for (unsigned short i = m_height/4; i < m_height*3/4; i++) {
        for (unsigned short j = m_width/4; j < (m_width*3/4+1); j++) {
            m_trails[i*(m_width+1) + j] = float(j > i ? j : i)/(m_width/2);
        }
    }
    */

    //free(m_trails);
}

Mould::~Mould() {
    free(m_trails);
    free(m_particles);
}

void Mould::gen_random_mould(unsigned int n_particles) {
    m_particles = (particle*) malloc(n_particles*sizeof(particle));
    
    float particle_height;
    float particle_width;
    float particle_rot;

    for (unsigned int i = 0; i < n_particles; i++) {
        particle_height = 220;//rand() % (m_height + 1);
        particle_width = 220;//rand() % (m_width + 1);
        particle_rot = fmod((double) rand(),M_PI*2.0f);
        m_particles[i] = {particle_height, particle_width, particle_rot};
    }
}
