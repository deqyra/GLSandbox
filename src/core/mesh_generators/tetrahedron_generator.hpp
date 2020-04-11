#ifndef CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
#define CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP

#include "../mesh_generator.hpp"
#include "../mesh.hpp"

#include <glm/glm.hpp>

class TetrahedronGenerator : public MeshGenerator
{
    public:
        static constexpr float DefaultSize = 1.f;

        struct Parameters
        {
            float size;
            glm::vec3 color;
            bool useColor;
        };

        TetrahedronGenerator();
        TetrahedronGenerator(float size);
        TetrahedronGenerator(float size, glm::vec3 color);
        TetrahedronGenerator(float size, float r, float g, float b);
        TetrahedronGenerator(Parameters parameters);

        virtual MeshPtr generatePtr();
        
        Parameters parameters;
};

#endif//CORE__MESH_GENERATORS__TETRAHEDRON_GENERATOR_HPP
