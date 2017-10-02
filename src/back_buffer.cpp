#include "back_buffer.h"

namespace Graphics
{
    BackBuffer::BackBuffer(int width, int height)
    {
        mWidth = width;
        mHeight = height;
        mSize = mWidth * mHeight * PixelWidth;
        mBuffer.reserve(mSize);
    }
    
    void BackBuffer::clear(const Color& color)
    {
        uint8_t r, g, b, a;
        r = static_cast<uint8_t>(color.r * 255);
        g = static_cast<uint8_t>(color.g * 255);
        b = static_cast<uint8_t>(color.b * 255);
        a = static_cast<uint8_t>(color.a * 255);
        
        for (uint32_t i = 0; i < mSize; i += PixelWidth) {
            mBuffer[i]     = r;
            mBuffer[i + 1] = g;
            mBuffer[i + 2] = b;
            mBuffer[i + 3] = a;
        }
    }
    
    void BackBuffer::put(int x, int y, const Color& color)
    {
        std::size_t idx = (x + y * mWidth) * PixelWidth;
        
        mBuffer[idx]     = static_cast<uint8_t>(color.r * 255);
        mBuffer[idx + 1] = static_cast<uint8_t>(color.g * 255);
        mBuffer[idx + 2] = static_cast<uint8_t>(color.b * 255);
        mBuffer[idx + 3] = static_cast<uint8_t>(color.a * 255);
    }
    
    void BackBuffer::put(const Vector& vec, const Color& color)
    {
        if (vec.x >= 0 && vec.y >= 0 && vec.x < mWidth && vec.y < mHeight) {
            put(static_cast<int>(vec.x), static_cast<int>(vec.y), color);
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
