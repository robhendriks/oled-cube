#include "camera.h"

namespace World
{
    void Camera::render(Graphics::BackBuffer& buffer, const World::Mesh &mesh) const
    {
        Math::Matrix view;
        Math::Matrix::lookAt(mPosition, mTarget, Vector::up(), view);
        
        float aspect = static_cast<float>(buffer.width()) / static_cast<float>(buffer.height());
        
        Math::Matrix projection;
        Math::Matrix::perspective(0.78, aspect, 0.01, 1.0, projection);
        
        Math::Matrix world;
        Math::Matrix::translate(mesh.mPosition, world);
        
        Math::Matrix transform = world * view * projection;
        
        for (const Vector& vertex : mesh.mVertices) {
            Vector result;
            project(buffer, vertex, transform, result);
            
            buffer.put(result, {1, 0, 0, 1});
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
