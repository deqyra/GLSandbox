#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <memory>

#include "light_type.hpp"

class Light
{
    private:
        Light() = delete;

        static unsigned int _count;

    protected:
        Light(LightType type);

    public:
        const unsigned int id;
        const LightType lightType;
};

using LightPtr = std::shared_ptr<Light>;

#endif//LIGHT_HPP
