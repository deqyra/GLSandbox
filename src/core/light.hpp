#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "light_type.hpp"

class Light
{
    private:
        Light() = delete;

    protected:
        Light(LightType type);

    public:
        const LightType lightType;
};

#endif//LIGHT_HPP
