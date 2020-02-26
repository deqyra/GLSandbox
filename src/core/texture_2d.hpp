#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <string>
#include <unordered_map>

#define MAX_TEXTURE_UNIT GL_TEXTURE31
#define MAX_TEXTURE_UNIT_OFFSET MAX_TEXTURE_UNIT - GL_TEXTURE0

class Texture2D
{
    private:
        unsigned int _id;
        std::string _path;

        // Structure storing program locations against the path of the shaders they were constructed from:
        // program shader names -> program location
        static std::unordered_map<std::string , unsigned int> _pathIds;

        // Structure storing how many shader instances are referencing a shader resource on the GPU:
        // program ID -> reference count
        static std::unordered_map<unsigned int, unsigned int> _refCount;

        static unsigned int loadTextureFromFile(const std::string path);

        void cleanup();

    public:
        Texture2D(std::string path);
        Texture2D(const Texture2D& other);
        Texture2D& operator=(const Texture2D& other);
        ~Texture2D();
        
        // Get texture ID.
        unsigned int id();

        // Bind the texture.
        void bind();
        void bind(unsigned int unit);
};

#endif//TEXTURE_2D_HPP