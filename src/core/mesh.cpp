/**
    GLTest, mesh.hpp
    Purpose: Implement part of class Mesh. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 13/02/2020
 */

#include "mesh.hpp"
#include "materials.hpp"

unsigned int Mesh::_count = 0;

Mesh::Mesh() :
    _buffersSetUp(false),
    id(_count++),
    material(Materials::Default)
{

}
