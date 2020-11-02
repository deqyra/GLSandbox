#ifndef CORE__TEXTURE_2D_HPP
#define CORE__TEXTURE_2D_HPP

#include <string>
#include <unordered_map>

#include <glad/gl.h>

#include "pixel_space.hpp"

/// @brief Handler for a 2D texture resource on the GPU.
class Texture2D
{
    private:
        /// @brief The location of the texture resource on the GPU.
        unsigned int _location;
        /// @brief The path of the image from which the texture was generated.
        std::string _path;

        /// @brief Structure mapping GPU texture locations against the path of 
        /// the image they were constructed from.
        static std::unordered_map<std::string , unsigned int> _pathsToIds;

        /// @brief Structure mapping how many Texture2D instances are handling 
        /// a texture resource on the GPU.
        static std::unordered_map<unsigned int, unsigned int> _locationRefCounts;

        /// @brief Process an image file and make a texture out of its content
        /// on the GPU.
        ///
        /// @param path Local path to the image file.
        ///
        /// @return The GPU location of the generated texture.
        ///
        /// @exception If the image could not be read, the function throws an
        /// std::runtime_error.
        static unsigned int loadTextureFromFile(const std::string filename, PixelSpace space);

        /// @brief Free resources upon instance destruction.
        void cleanup();

    public:

        /// @brief GPU location of the last texture unit available.
        static constexpr unsigned int MaxTextureUnit = GL_TEXTURE31;
        /// @brief Amount of texture units that can be handled by the GPU.
        static constexpr unsigned int MaxTextureUnitCount = MaxTextureUnit - GL_TEXTURE0 + 1;

        /// @param filename Local path to an image file out of which the texture
        /// should be generated.
        /// @param pixelSpace Literal describing which space the texture pixels
        /// are in.
        Texture2D(std::string filename, PixelSpace space);

        Texture2D(const Texture2D& other);

        ~Texture2D();

        Texture2D& operator=(const Texture2D& other);
        
        /// @brief Get the location of the texture on the GPU.
        ///
        /// @return The location of the texture on the GPU.
        unsigned int location();

        /// @brief Bind the texture to the current texture unit on the GPU.
        void bind();

        /// @brief Bind the texture to a particular texture unit on the GPU.
        ///
        /// @param unit 0-based index of the unit to which the texture should
        /// be bound.
        ///
        /// @exception If [unit] is too big and ends up targeting a unit past
        /// Texture2D::MaxTextureUnit, the function will throw an 
        /// IndexOutOfBoundsError.
        void bind(unsigned int unit);
};

#endif//CORE__TEXTURE_2D_HPP