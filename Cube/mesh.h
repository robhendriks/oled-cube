#ifndef mesh_h
#define mesh_h

#include <string>
#include <vector>
#include "vector.h"

namespace World
{
    using namespace Math;
    
    class Camera;
    class Mesh
    {
        friend class Camera;
        
        std::string mName;
        Vector mPosition;
        Vector mRotation;
        std::vector<Vector> mVertices;
        
    public:
        Mesh(const std::string& name, const std::vector<Vector>& vertices);
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
