#ifndef CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP
#define CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

#include <glm/glm.hpp>

class CubeGenerator : public MeshGenerator
{
    public:
        static constexpr float DefaultSize = 1.f;

        struct Parameters
        {
            float size;
            glm::vec3 color;
            bool useColor;
        };

        CubeGenerator();
        CubeGenerator(float size);
        CubeGenerator(float size, glm::vec3 color);
        CubeGenerator(float size, float r, float g, float b);
        CubeGenerator(Parameters parameters);

        virtual MeshPtr generatePtr();
        
        Parameters parameters;
};

#endif//CORE__MESH_GENERATORS__CUBE_GENERATOR_HPP
