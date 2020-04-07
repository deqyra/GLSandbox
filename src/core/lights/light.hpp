#ifndef CORE__LIGHT_HPP
#define CORE__LIGHT_HPP

#include <memory>

#include "light_type.hpp"

class Light
{
    private:
        Light() = delete;

        // Keeps track of how many Light objects were created (used as an ID system)
        static unsigned int _count;

    protected:
        Light(LightType type);

    public:
        // ID of the light
        const unsigned int id;
        // Type of light
        const LightType lightType;
};

using LightPtr = std::shared_ptr<Light>;
using LightWPtr = std::weak_ptr<Light>;

#endif//CORE__LIGHT_HPP
