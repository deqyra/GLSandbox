#ifndef TOOLS__CLONABLE_HPP
#define TOOLS__CLONABLE_HPP

#include <memory>

template<typename T>
class Clonable
{
    public:
        virtual std::shared_ptr<T> clone() = 0;
};

template<typename T>
concept clonable = requires(T v) {
    { v.clone() } -> std::shared_ptr<T>
};

#endif//TOOLS__CLONABLE_HPP
