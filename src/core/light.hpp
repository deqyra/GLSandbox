#ifndef LIGHT_HPP
#define LIGHT_HPP

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

#endif//LIGHT_HPP
