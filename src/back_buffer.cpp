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
    
    void BackBuffer::drawLine(const World::Vertex& va, const World::Vertex& vb, const Color& color)
    {
        // Bresenham’s line algorithm
        
        Vector a = va.coords;
        Vector b = vb.coords;
        
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
            put(Vector{static_cast<float>(x0), static_cast<float>(y0), a.z}, color);
            
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
    
    void BackBuffer::drawTriangle(World::Vertex va, World::Vertex vb, World::Vertex vc, const Color& color)
    {
        if (va.coords.y > vb.coords.y) {
            World::Vertex temp = vb;
            vb = va;
            va = temp;
        }
        if (vb.coords.y > vc.coords.y) {
            World::Vertex temp = vb;
            vb = vc;
            vc = temp;
        }
        if (va.coords.y > vb.coords.y) {
            World::Vertex temp = vb;
            vb = va;
            va = temp;
        }

        Vector a = va.coords;
        Vector b = vb.coords;
        Vector c = vc.coords;
        
        Vector lightPos{0, 10, 10};
        
        float nl1 = computeNDotL(va.worldCoords, va.normal, lightPos);
        float nl2 = computeNDotL(vb.worldCoords, vb.normal, lightPos);
        float nl3 = computeNDotL(vc.worldCoords, vc.normal, lightPos);
        
        ScanLineData data;
        
        if (side(b, a, c) > 0) {
            for (int y = static_cast<int>(a.y); y <= static_cast<int>(c.y); y++) {
                data.currentY = y;
                
                if (y < b.y) {
                    data.ndotla = nl1;
                    data.ndotlb = nl3;
                    data.ndotlc = nl1;
                    data.ndotld = nl2;
                    scanLine(data, va, vc, va, vb, color);
                } else {
                    data.ndotla = nl1;
                    data.ndotlb = nl3;
                    data.ndotlc = nl2;
                    data.ndotld = nl3;
                    scanLine(data, va, vc, vb, vc, color);
                }
            }
        } else {
            for (int y = static_cast<int>(a.y); y <= static_cast<int>(c.y); y++) {
                data.currentY = y;
                
                if (y < b.y) {
                    data.ndotla = nl1;
                    data.ndotlb = nl3;
                    data.ndotlc = nl1;
                    data.ndotld = nl2;
                    scanLine(data, va, vb, va, vc, color);
                } else {
                    data.ndotla = nl1;
                    data.ndotlb = nl3;
                    data.ndotlc = nl2;
                    data.ndotld = nl3;
                    scanLine(data, vb, vc, va, vc, color);
                }
            }
        }
    }
    
    void BackBuffer::scanLine(ScanLineData data, const World::Vertex& va, const World::Vertex& vb, const World::Vertex& vc, const World::Vertex& vd, const Color& color)
    {
        Vector pa = va.coords;
        Vector pb = vb.coords;
        Vector pc = vc.coords;
        Vector pd = vd.coords;
        
        float gradient1 = pa.y != pb.y ? (data.currentY - pa.y) / (pb.y - pa.y) : 1;
        float gradient2 = pc.y != pd.y ? (data.currentY - pc.y) / (pd.y - pc.y) : 1;
        
        int sx = static_cast<int>(interpolate(pa.x, pb.x, gradient1));
        int ex = static_cast<int>(interpolate(pc.x, pd.x, gradient2));
        
        float z1 = interpolate(pa.z, pb.z, gradient1);
        float z2 = interpolate(pc.z, pd.z, gradient2);
        
        float snl = interpolate(data.ndotla, data.ndotlb, gradient1);
        float enl = interpolate(data.ndotlc, data.ndotld, gradient2);
        
        for (int x = sx; x < ex; ++x) {
            float gradient = (x - sx) / static_cast<float>(ex - sx);
            float z = interpolate(z1, z2, gradient);
            float ndotl = interpolate(snl, enl, gradient);
            put(Vector(x, data.currentY, z), color * ndotl);
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
