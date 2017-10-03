#include "back_buffer.h"

namespace Graphics
{
    BackBuffer::BackBuffer(int width, int height)
    {
        mWidth = width;
        mHeight = height;
        mSize = mWidth * mHeight * PixelWidth;
        mBuffer.reserve(mSize);
        mDepthBuffer.resize(mWidth * mHeight);
    }

    void BackBuffer::clear(const Color& color)
    {
        uint8_t r, g, b, a;
        r = static_cast<uint8_t>(color.r * 255);
        g = static_cast<uint8_t>(color.g * 255);
        b = static_cast<uint8_t>(color.b * 255);
        a = static_cast<uint8_t>(color.a * 255);

        for (std::size_t i = 0; i < mSize; i += PixelWidth) {
            mBuffer[i]     = r;
            mBuffer[i + 1] = g;
            mBuffer[i + 2] = b;
            mBuffer[i + 3] = a;
        }

        for (std::size_t i = 0; i < mWidth * mHeight; ++i) {
            mDepthBuffer[i] = std::numeric_limits<float>::max();
        }
    }

    void BackBuffer::put(int x, int y, float z, const Color& color)
    {
        std::size_t idx = (x + y * mWidth);
        std::size_t idx4 = idx * PixelWidth;

        if (mDepthBuffer[idx] < z) {
            return;
        }

        mDepthBuffer[idx] = z;

        mBuffer[idx4]     = static_cast<uint8_t>(color.r * 255);
        mBuffer[idx4 + 1] = static_cast<uint8_t>(color.g * 255);
        mBuffer[idx4 + 2] = static_cast<uint8_t>(color.b * 255);
        mBuffer[idx4 + 3] = static_cast<uint8_t>(color.a * 255);
    }

    void BackBuffer::put(const Vector& vec, const Color& color)
    {
        if (vec.x >= 0 && vec.y >= 0 && vec.x < mWidth && vec.y < mHeight) {
            put(static_cast<int>(vec.x), static_cast<int>(vec.y), vec.z, color);
        }
    }

    void BackBuffer::drawLine(const Vector& a, const Vector& b, const Color& color)
    {
        // Bresenham’s line algorithm

        int x0 = static_cast<int>(a.x);
        int y0 = static_cast<int>(a.y);
        int x1 = static_cast<int>(b.x);
        int y1 = static_cast<int>(b.y);

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);

        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            put(Vector{static_cast<float>(x0), static_cast<float>(y0), 0}, color);

            if (x0 == x1 && y0 == y1) {
                break;
            }

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx){
                err += dx;
                y0 += sy;
            }
        }
    }

    void BackBuffer::drawTriangle(Vector a, Vector b, Vector c, const Color& color)
    {
        if (a.y > b.y)
        {
            auto temp = b;
            b = a;
            a = temp;
        }

        if (b.y > c.y)
        {
            auto temp = b;
            b = c;
            c = temp;
        }

        if (a.y > b.y)
        {
            auto temp = b;
            b = a;
            a = temp;
        }

        if (side(b, a, c) > 0) {
            for (auto y = (int)a.y; y <= (int)c.y; y++) {
                if (y < b.y) {
                     scanLine(y, a, c, a, b, color);
                } else {
                    scanLine(y, a, c, b, c, color);
                }
            }
        } else {
            for (auto y = (int)a.y; y <= (int)c.y; y++) {
                if (y < b.y) {
                    scanLine(y, a, b, a, c, color);
                } else {
                    scanLine(y, b, c, a, c, color);
                }
            }
        }
    }

    void BackBuffer::scanLine(int y, const Vector& a, const Vector& b, const Vector& c, const Vector& d, const Color& color)
    {
        float gradient1 = a.y != b.y ? (y - a.y) / (b.y - a.y) : 1;
        float gradient2 = c.y != d.y ? (y - c.y) / (d.y - c.y) : 1;

        int sx = static_cast<int>(interpolate(a.x, b.x, gradient1));
        int ex = static_cast<int>(interpolate(c.x, d.x, gradient2));

        float z1 = interpolate(a.z, b.z, gradient1);
        float z2 = interpolate(c.z, d.z, gradient2);

        for (int x = sx; x < ex; x++) {
            float gradient = (x - sx) / static_cast<float>(ex - sx);
            float z = interpolate(z1, z2, gradient);
            put(Vector{static_cast<float>(x), static_cast<float>(y), z}, color);
        }
    }

    int BackBuffer::width() const
    {
        return mWidth;
    }

    int BackBuffer::height() const
    {
        return mHeight;
    }

    std::size_t BackBuffer::size() const
    {
        return mSize;
    }

    std::vector<uint8_t>& BackBuffer::buffer()
    {
        return mBuffer;
    }

    const std::vector<uint8_t>& BackBuffer::buffer() const
    {
        return mBuffer;
    }
}
