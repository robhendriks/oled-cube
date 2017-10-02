#include "camera.h"

namespace World
{
    void Camera::render(Graphics::BackBuffer& buffer, const World::Mesh &mesh, const Graphics::Color& color) const
    {
        Math::Matrix view;
        Math::Matrix::lookAt(mPosition, mTarget, Vector::up(), view);
        
        float aspect = static_cast<float>(buffer.width()) / static_cast<float>(buffer.height());
        
        Math::Matrix projection;
        Math::Matrix::perspective(0.78, aspect, 0.01, 1.0, projection);
        
        Math::Matrix translate, rotate, world;
        Math::Matrix::translate(mesh.mPosition, translate);
        Math::Matrix::rotate(mesh.mRotation.y, mesh.mRotation.x, mesh.mRotation.z, rotate);
        
        world = rotate * translate;
        
        Math::Matrix transform = world * view * projection;
        
        for (const Face& face : mesh.mFaces) {
            Vector a, b, c;
            project(buffer, mesh.mVertices[face.a], transform, a);
            project(buffer, mesh.mVertices[face.b], transform, b);
            project(buffer, mesh.mVertices[face.c], transform, c);
            
            buffer.drawLine(a, b, color);
            buffer.drawLine(b, c, color);
            buffer.drawLine(c, a, color);
        }
    }
    
    void Camera::project(Graphics::BackBuffer& buffer, const Vector& vertex, const Matrix& transform, Vector& result) const
    {
        Vector tmp;
        Math::transform(vertex, transform, tmp);
        
        result.x = tmp.x * buffer.width() + buffer.width() / 2.0;
        result.y = -tmp.y * buffer.height() + buffer.height() / 2.0;
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
