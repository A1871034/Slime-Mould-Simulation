#pragma once

#include <cmath>

class Settings {
    float sensor_angle = 30/360*M_PI; // Deg -> Rad
    float sensor_offset = 3.0f; // Pixels
    unsigned char sensor_size = 1;
    float rotate_angle = sensor_angle;
    float move_speed = 1.0f;
    float decay_rate = 0.5f;
    float spread_rate = 1.0f;
    float deposit_strenght = 0.1f;
};