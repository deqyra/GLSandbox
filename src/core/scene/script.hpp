#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <memory>

#include "scene_types_decl.hpp"

// Abstract class for a script referencing objects from a scene and updating them on a per-frame basis
class Script
{
    private:
        // Keeps track of how many Script objects were created (used as an ID system)
        static unsigned int _count;

    public:
        Script();
        virtual ~Script();

        // To be called once per frame
        virtual void update(float timeElapsed) = 0;

        // Unique ID of the script
        const unsigned int id;
};

using ScriptPtr = std::shared_ptr<Script>;
using ScriptWPtr = std::weak_ptr<Script>;

#endif//SCRIPT_HPP