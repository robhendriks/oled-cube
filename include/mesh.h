#ifndef mesh_h
#define mesh_h

#include <string>
#include <vector>
#include "vector.h"

namespace World
{
    using namespace Math;
    
    struct Face
    {
        int a, b, c;
    };
    
    struct Vertex
    {
        Vector coords, normal, worldCoords;
    };
    
    class Camera;
    class Mesh
    {
        friend class Camera;
        
        std::string mName;
        Vector mPosition;
        Vector mRotation;
        std::vector<Vertex> mVertices;
        std::vector<Face> mFaces;
        
    public:
        Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<Face>& faces);
        ~Mesh() = default;
        
        std::string& name();
        const std::string& name() const;
        void setName(const std::string& name);
        Vector& position();
        const Vector& position() const;
        void setPosition(const Vector& position);
        Vector& rotation();
        const Vector& rotation() const;
        void setRotation(const Vector& rotation);
    };
}

#endif
