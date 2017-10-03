#ifndef back_buffer_h
#define back_buffer_h

#include <stdint.h>
#include <vector>
#include "color.h"
#include "vector.h"
#include "mesh.h"

namespace Graphics
{
    using namespace Math;
    
    inline float clamp(float value, float min, float max)
    {
        return std::max(min, std::min(max, value));
    }
    
    inline float interpolate(float min, float max, float gradient)
    {
        return min + (max - min) * clamp(gradient, 0, 1);
    }
    
    inline float cross(float x0, float y0, float x1, float y1)
    {
        return x0 * y1 - x1 * y0;
    }
    
    inline float side(const Vector& vec, const Vector& from, const Vector& to)
    {
        return cross(vec.x - from.x, vec.y - from.y, to.x - from.x, to.y - from.y);
    }
    
    inline float computeNDotL(Vector vertex, Vector normal, Vector lightPosition)
    {
        Vector lightDirection = lightPosition - vertex;
        normal.normalize();
        lightDirection.normalize();
        return std::fmax(0, dot(normal, lightPosition));
    }
    
    struct ScanLineData
    {
        int currentY;
        float ndotla;
        float ndotlb;
        float ndotlc;
        float ndotld;
    };
    
    class BackBuffer
    {
        const uint8_t PixelWidth = 4;
        
        int mWidth;
        int mHeight;
        std::size_t mSize;
        std::vector<uint8_t> mBuffer;
        std::vector<float> mDepthBuffer;
        
        void put(int x, int y, float z, const Color& color);
        
    public:
        BackBuffer(int width, int height);
        ~BackBuffer() = default;
        
        void clear(const Color& color);
        void put(const Vector& vec, const Color& color);
        
        void drawLine(const World::Vertex& va, const World::Vertex& vb, const Color& color);
        void drawTriangle(World::Vertex a, World::Vertex b, World::Vertex c, const Color& color);
        void scanLine(ScanLineData data, const World::Vertex& a, const World::Vertex& b, const World::Vertex& c, const World::Vertex& d, const Color& color);
        
        int width() const;
        int height() const;
        std::size_t size() const;
        std::vector<uint8_t>& buffer();
        const std::vector<uint8_t>& buffer() const;
    };
}

#endif
