#include "camera.h"

namespace World
{
    void Camera::render(Graphics::BackBuffer& buffer, const World::Mesh &mesh, const Graphics::Color& color) const
    {
        Math::Matrix view;
        Math::Matrix::lookAt(mPosition, mTarget, Vector::up(), view);
        
//        float aspect = static_cast<float>(buffer.width()) / static_cast<float>(buffer.height());
        
        Math::Matrix projection;
//        Math::Matrix::perspective(0.78, aspect, 0.1, 1.0, projection);
        Math::Matrix::ortho(buffer.width() / 30, buffer.height() / 30, 0.01, 1.0, projection);
        
        Math::Matrix translate, rotate, world;
        Math::Matrix::translate(mesh.mPosition, translate);
        Math::Matrix::rotate(mesh.mRotation.y, mesh.mRotation.x, mesh.mRotation.z, rotate);
        
        world = rotate * translate;
        Math::Matrix transform = world * view * projection;
        
        for (const Face& face : mesh.mFaces) {
            Vertex a, b, c;
            project(buffer, mesh.mVertices[face.a], transform, world, a);
            project(buffer, mesh.mVertices[face.b], transform, world, b);
            project(buffer, mesh.mVertices[face.c], transform, world, c);
            
            buffer.drawLine(a, b, {1, 1, 1, 1});
            buffer.drawLine(b, c, {1, 1, 1, 1});
            buffer.drawLine(c, a, {1, 1, 1, 1});
            
            buffer.drawTriangle(a, b, c, {0, 0, 0, 1});
        }
    }
    
    void Camera::project(Graphics::BackBuffer& buffer, const Vertex& vertex, const Matrix& transform, const Matrix& world, Vertex& result) const
    {
        Vector coords, worldCoords, normal;
        Math::transform(vertex.coords, transform, coords);
        Math::transform(vertex.coords, world, worldCoords);
        Math::transform(vertex.normal, world, normal);
        
        float x = coords.x * buffer.width() + buffer.width() / 2.0;
        float y = -coords.y * buffer.height() + buffer.height() / 2.0;
        
        result.coords = {x, y, coords.z};
        result.normal = normal;
        result.worldCoords = worldCoords;
    }
    
    Vector& Camera::position()
    {
        return mPosition;
    }
    
    const Vector& Camera::position() const
    {
        return mPosition;
    }
    
    void Camera::setPosition(const Vector& position)
    {
        mPosition = position;
    }
    
    Vector& Camera::target()
    {
        return mTarget;
    }
    
    const Vector& Camera::target() const
    {
        return mTarget;
    }
    
    void Camera::setTarget(const Vector& target)
    {
        mTarget = target;
    }
}
