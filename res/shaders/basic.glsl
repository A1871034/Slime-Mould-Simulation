#shader vertex
#version 430 core

layout(location = 0) in vec4 position;

out vec2 pixel_coords;

// Model view proejction matrix
uniform mat4 u_MVP;

void main() {
   gl_Position = u_MVP * position;
   pixel_coords = position.xy;
}

#shader fragment
#version 430 core

layout(std430, binding = 3) buffer trails_buffer_in
{
   float trails_in[];
};

layout(std430, binding = 4) buffer trails_buffer_out
{
   float trails_out[];
};

layout(location = 0) out vec4 color;

in vec2 pixel_coords;

int width = 541;

float strength;

void main() {
   // Display trail
   strength = trails_in[int(pixel_coords.y)*width + int(pixel_coords.x)]; 
   color = vec4(1,1,1,strength);

   // Convolution
   // Spread trail
   ///float div_by = 1.0f/9;
   ///trails_out[int(pixel_coords.y)*width + int(pixel_coords.x)] = (trails_in[int(pixel_coords.y+1)*width + int(pixel_coords.x)-1]*div_by + trails_in[int(pixel_coords.y+1)*width + int(pixel_coords.x)]*div_by + trails_in[int(pixel_coords.y+1)*width + int(pixel_coords.x)+1]*div_by +
   ///                                                         trails_in[int(pixel_coords.y)*width + int(pixel_coords.x)-1]*div_by + trails_in[int(pixel_coords.y)*width + int(pixel_coords.x)]*div_by + trails_in[int(pixel_coords.y)*width + int(pixel_coords.x)+1]*div_by + 
   ///                                                         trails_in[int(pixel_coords.y-1)*width + int(pixel_coords.x)-1]*div_by + trails_in[int(pixel_coords.y-1)*width + int(pixel_coords.x)]*div_by + trails_in[int(pixel_coords.y-1)*width + int(pixel_coords.x)+1]*div_by)*0.8;
   
   // Decay Trail
   trails_out[int(pixel_coords.y)*width + int(pixel_coords.x)] *= 0.85f;
   
  
}

#shader compute
#version 430 core
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
// Local workgroup size
layout (local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

struct particle { // May want to use doubles
    float pos_x; // 0 Left
    float pos_y; // 0 Bottom
    float rot; // Up of Right
};

layout(std430, binding = 3) buffer trails_buffer_in
{
   float trails_in[];
};

layout(std430, binding = 4) buffer trails_buffer_out
{
   float trails_out[];
};

layout(std430, binding = 5) buffer particles_buffer
{
   particle particles[];
};

float sensor_offset = 9.0f;
float sensor_angle = 0.5236f; // ~30 deg
float speed = 1.0f;
float rotate_angle = sensor_angle;

int width = 541;
int height = 541;

float LV;
float MV;
float RV;

int LI;
int MI;
int RI;


uint random;

uint xorshift32(uint x) {
   x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
   return x;
}

int sensor_index(struct particle p, float adjust) {
   float angle = p.rot + adjust;
   int y = int(p.pos_y + sensor_offset*sin(angle));
   int x = int(p.pos_x + sensor_offset*cos(angle));

   // Border checking
   // Returns more negative values the further from bounds you are
   if (y < 0 && x < 0) {
      return -y*x;
   } else if (y > height && x > width) {
      return -(y-height)*(x-width);
   } else if (x < 0) {
      return x;
   } else if (x > width) {
      return width-x;
   } else if (y < 0) {
      return y;
   } else if (y > height) {
      return height-y;
   }

   return y*width + x;
}

void main() {
   for (unsigned int i = 0; i < particles.length(); i++) {
      // Add Particle to trail map
      trails_out[int(particles[i].pos_y)*width + int(particles[i].pos_x)] += 0.125f;

      // Move particle
      particles[i].pos_x += speed*cos(particles[i].rot);
      particles[i].pos_y += speed*sin(particles[i].rot);

      // Sense to adjust direction
      LI = sensor_index(particles[i], -sensor_angle);
      MI = sensor_index(particles[i], 0.0f);
      RI = sensor_index(particles[i], sensor_angle);

      LV = (LI >= 0) ? trails_in[LI] : LI;
      MV = (MI >= 0) ? trails_in[MI] : MI;
      RV = (RI >= 0) ? trails_in[RI] : RI;

      // Adjust direction
      if ((MV > RV) && (MV > LV)) {
         continue;
      } else if ((MV < RV) && (MV < LV)) {
         // Random rotate left/right
         random = xorshift32((uint(particles[i].pos_x)*width + uint(particles[i].pos_y))*7273) + xorshift32(uint(gl_LocalInvocationIndex)*132563);
         particles[i].rot += (xorshift32(random)/4294967296.0f - 0.5f)*2.0f * rotate_angle;
      } else if (LV > RV) {
         particles[i].rot -= rotate_angle;
      } else if (RV > LV) {
         particles[i].rot += rotate_angle;
      }
   }  

}