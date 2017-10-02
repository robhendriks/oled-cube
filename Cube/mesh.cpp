#include "mesh.h"
#include "camera.h"

namespace World
{
    Mesh::Mesh(const std::string& name, const std::vector<Vector>& vertices, const std::vector<Face>& faces)
    {
        mName = name;
        mVertices = vertices;
        mFaces = faces;
    }
    
    std::string& Mesh::name()
    {
        return mName;
    }
    
    const std::string& Mesh::name() const
    {
        return mName;
    }
    
    void Mesh::setName(const std::string& name)
    {
        mName = name;
    }
    
    Vector& Mesh::position()
    {
        return mPosition;
    }
    
    const Vector& Mesh::position() const
    {
        return mPosition;
    }
    
    void Mesh::setPosition(const Vector& position)
    {
        mPosition = position;
    }
    
    Vector& Mesh::rotation()
    {
        return mRotation;
    }
    
    const Vector& Mesh::rotation() const
    {
        return mRotation;
    }
    
    void Mesh::setRotation(const Vector& rotation)
    {
        mRotation = rotation;
    }
}
