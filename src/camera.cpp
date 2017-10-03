#include "camera.h"

namespace World
{
    void Camera::render(Graphics::BackBuffer& buffer, const World::Mesh &mesh, const Graphics::Color& color) const
    {
        Math::Matrix view;
        Math::Matrix::lookAt(mPosition, mTarget, Vector::up(), view);

        Math::Matrix projection;

        float aspect = static_cast<float>(buffer.width()) / static_cast<float>(buffer.height());
        Math::Matrix::perspective(0.78, aspect, 0.1, 1.0, projection);

        // Math::Matrix::ortho(buffer.width() / 5, buffer.height() / 5, 0.01, 1.0, projection);

        Math::Matrix translate, scale, rotate, world;
        Math::Matrix::translate(mesh.mPosition, translate);
        Math::Matrix::rotate(mesh.mRotation.y, mesh.mRotation.x, mesh.mRotation.z, rotate);
        Math::Matrix::scale(mesh.mScale, scale);

        world = scale * rotate * translate;

        Math::Matrix transform = world * view * projection;

        for (const Face& face : mesh.mFaces) {
            Vector a, b, c;
            project(buffer, mesh.mVertices[face.a], transform, a);
            project(buffer, mesh.mVertices[face.b], transform, b);
            project(buffer, mesh.mVertices[face.c], transform, c);

            buffer.drawLine(a, b, {1, 1, 1, 1});
            buffer.drawLine(b, c, {1, 1, 1, 1});
            buffer.drawLine(c, a, {1, 1, 1, 1});

            // AXIS GUIDE
            Vector p0, p1, p2, p3;
            Matrix m0 = view * projection;
            project(buffer, Vector{0, 0, 0}, m0, p0);
            project(buffer, Vector{1, 0, 0}, m0, p1);
            project(buffer, Vector{0, 1, 0}, m0, p2);
            project(buffer, Vector{0, 0, 1}, m0, p3);

            buffer.drawLine(p0, p1, {1, 1, 1, 1});
            buffer.drawLine(p0, p2, {1, 1, 1, 1});
            buffer.drawLine(p0, p3, {1, 1, 1, 1});

            // float color = 0.25 + (i % max) * 0.75 / max;
            // buffer.drawTriangle(a, b, c, {0, 0, 0, 1});
            // ++i;
        }
    }

    void Camera::project(Graphics::BackBuffer& buffer, const Vector& vertex, const Matrix& transform, Vector& result) const
    {
        Vector tmp;
        Math::transform(vertex, transform, tmp);

        result.x = tmp.x * buffer.width() + buffer.width() / 2.0;
        result.y = -tmp.y * buffer.height() + buffer.height() / 2.0;
        result.z = tmp.z;
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
