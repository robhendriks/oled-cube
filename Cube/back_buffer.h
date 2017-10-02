#ifndef back_buffer_h
#define back_buffer_h

#include <stdint.h>
#include <vector>
#include "color.h"
#include "vector.h"

namespace Graphics
{
    using namespace Math;
    
    class BackBuffer
    {
        const uint8_t PixelWidth = 4;
        
        int mWidth;
        int mHeight;
        std::size_t mSize;
        std::vector<uint8_t> mBuffer;
        
        void put(int x, int y, const Color& color);
        
    public:
        BackBuffer(int width, int height);
        ~BackBuffer() = default;
        
        void clear(const Color& color);
        void put(const Vector& vec, const Color& color);
        
        int width() const;
        int height() const;
        std::size_t size() const;
        std::vector<uint8_t>& buffer();
        const std::vector<uint8_t>& buffer() const;
    };
}

#endif
