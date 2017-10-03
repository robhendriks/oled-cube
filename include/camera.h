#ifndef camera_h
#define camera_h

#include "back_buffer.h"
#include "matrix.h"
#include "vector.h"
#include "mesh.h"

namespace World
{
    using namespace Math;
    
    class Camera
    {        
        Vector mPosition;
        Vector mTarget;
        
    public:
        Camera() = default;
        ~Camera() = default;
        
        void render(Graphics::BackBuffer& buffer, const Mesh& mesh, const Graphics::Color& color) const;
        void project(Graphics::BackBuffer& buffer, const Vertex& vertex, const Matrix& transform, const Matrix& world, Vertex& result) const;
        Vector& position();
        const Vector& position() const;
        void setPosition(const Vector& position);
        Vector& target();
        const Vector& target() const;
        void setTarget(const Vector& target);
    };
}

#endif
