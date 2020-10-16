#ifndef CORE__FRAME_OF_REFERENCE_HPP
#define CORE__FRAME_OF_REFERENCE_HPP

/// @brief Collection of literals to be used as template parameters for 
/// functions, describing the frame of reference in which the arguments are 
/// given to the function. 
enum class FrameOfReference
{
    /// @brief Indicates that arguments are provided relative to the frame of 
    /// reference of the object on which the function is called.
    Self,
    /// @brief Indicates that arguments are provided relative to the frame of 
    /// reference of the parent object of the object on which the function is 
    /// called.
    Parent,
    /// @brief Indicates that arguments are provided relative to the global 
    /// frame of reference.
    World
};

#endif//CORE__FRAME_OF_REFERENCE_HPP