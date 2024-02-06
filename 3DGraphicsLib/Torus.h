#pragma once
#include "Main.h"

class Torus 
{
public:
    std::vector<float> torus(size_t radial_resolution, size_t tubular_resolution, float radius, float thickness)
    {
        std::vector<float> mesh;

        // generate vertices
        for (size_t i = 0; i < radial_resolution; i++) {
            for (size_t j = 0; j < tubular_resolution; j++) {
                float u = (float)j / tubular_resolution * M_PI * 2.0;
                float v = (float)i / radial_resolution * M_PI * 2.0;
                float x = (radius + thickness * std::cos(v)) * std::cos(u);
                float y = (radius + thickness * std::cos(v)) * std::sin(u);
                float z = thickness * std::sin(v);
                mesh.push_back(x);
                mesh.push_back(y);
                mesh.push_back(z);
            }
        }

        // add quad faces
        for (size_t i = 0; i < radial_resolution; i++) {
            auto i_next = (i + 1) % radial_resolution;
            for (size_t j = 0; j < tubular_resolution; j++) {
                auto j_next = (j + 1) % tubular_resolution;
                auto i0 = i * tubular_resolution + j;
                auto i1 = i * tubular_resolution + j_next;
                auto i2 = i_next * tubular_resolution + j_next;
                auto i3 = i_next * tubular_resolution + j;
                mesh.push_back(i0);
                mesh.push_back(i1); 
                mesh.push_back(i2); 
                mesh.push_back(i3);
            }
        }

        return mesh;
    }
private:
};
