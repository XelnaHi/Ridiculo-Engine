#include <math.h>
#include <iostream>
#include <vector>

// Fixed version... Now faces are ccw instead of cw...

// It's not the nicest sphere, nor the nicest code, but it sort of works.
void generateSortofSphere(std::vector<float> &vertices, std::vector<unsigned int> &indices, int rings, int slices) {

    if (rings < 3)  { std::cout << "no sphere generated. rings value < 3"  << std::endl;  return; }
    if (slices < 4) { std::cout << "no sphere generated. slices value < 4" << std::endl;  return; }

    float PIE = 3.1415f;

    vertices.clear();
    indices.clear();

    float angle_step    = 360.0f / slices; 
    float current_angle = 0.0f;

    // Vertices
    float y_step = 2.0f / (rings - 1);
    float y_pos  = 1.0f;

    for (int ring = 0; ring < rings; ring++) {

        int iteration_slices = slices;
        if (ring == 0 || ring == rings - 1) { iteration_slices = 1; }

        for (int i = 0; i < iteration_slices; i++) {

            float radius = sinf(acos(y_pos));

            float x_pos = cosf(current_angle * PIE / 180) * radius;
            float z_pos = sinf(current_angle * PIE / 180) * radius;

            vertices.push_back(x_pos);
            vertices.push_back(y_pos);
            vertices.push_back(z_pos);

            current_angle += angle_step;
        }

        y_pos -= y_step;
        current_angle = 0.0f;
    }

    std::cout << "Sphere vertex count:" << vertices.size() / 3 << std::endl;

    //Indices
    unsigned int top_most_index = 0;
    unsigned int bottom_most_index = vertices.size() / 3 - 1;

    for (int i = 0; i < rings - 1; i++) {

        for (int j = 0; j < slices; j++) {

            unsigned int tl_index = (i - 1) * slices + j + 1;
            unsigned int tr_index = (i - 1) * slices + j + 1 + 1;
            unsigned int bl_index = (i + 0) * slices + j + 1;
            unsigned int br_index = (i + 0) * slices + j + 1 + 1;

            if (j == slices - 1) { // For the seam

                tr_index = (i - 2) * slices + j + 1 + 1;
                br_index = (i - 1) * slices + j + 1 + 1;
            }

            if (i == 0) { // First ring

                indices.push_back(top_most_index);
                indices.push_back(bl_index);
                indices.push_back(br_index);

            } else if (i == rings - 2) { // Last ring

                indices.push_back(tr_index);
                indices.push_back(tl_index);
                indices.push_back(bottom_most_index);


            } else {

                indices.push_back(tl_index);
                indices.push_back(bl_index);
                indices.push_back(br_index);

                indices.push_back(tl_index);
                indices.push_back(br_index);
                indices.push_back(tr_index);
            }
        }
    }

    std::cout << "Sphere triangle count:" << indices.size() / 3 << std::endl;
}