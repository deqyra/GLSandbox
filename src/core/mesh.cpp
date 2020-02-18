/**
    GLTest, mesh.hpp
    Purpose: Implement part of class Mesh. See .hpp file.

    @author François Brachais (deqyra)
    @version 1.0 13/02/2020
 */

#include "mesh.hpp"

unsigned int Mesh::_count = 0;

Mesh::Mesh() :
    _id(_count++),
    _buffersSetUp(false),
    material(Materials::Default)
{

}

unsigned int Mesh::getId()
{
    return _id;
}
