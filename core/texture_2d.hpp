#ifndef CORE__TEXTURE_2D_HPP
#define CORE__TEXTURE_2D_HPP

#include <string>
#include <unordered_map>
#include <glad/gl.h>

// Proxy class to a texture 2D resource on the GPU
class Texture2D
{
    private:
        // The location of the texture resource on the GPU
        unsigned int _location;
        // The image path from which the texture was generated
        std::string _path;

        // Structure storing GPU texture locations against the path of the image they were constructed from:
        // disk image name -> texture location
        static std::unordered_map<std::string , unsigned int> _pathsToIds;

        // Structure storing how many Texture2D instances are handling a texture resource on the GPU:
        // texture ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _refCount;

        // Process an image on the hard drive and upload it to the GPU
        static unsigned int loadTextureFromFile(const std::string path);

        // Free resources
        void cleanup();

    public:
        static constexpr unsigned int MaxTextureUnit = GL_TEXTURE31;
        static constexpr unsigned int MaxTextureUnitOffset = MaxTextureUnit - GL_TEXTURE0;

        Texture2D(std::string path);
        Texture2D(const Texture2D& other);
        Texture2D& operator=(const Texture2D& other);
        ~Texture2D();
        
        // Get texture location
        unsigned int location();

        // Bind the texture
        void bind();
        // Bind the texture to a particular unit
        void bind(unsigned int unit);
};

#endif//CORE__TEXTURE_2D_HPP