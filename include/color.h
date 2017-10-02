#ifndef color_h
#define color_h

namespace Graphics
{
    struct Color
    {
        float r, g, b, a;
        
        Color() : r{0}, g{0}, b{0}, a{0}
        {
        }
        
        Color(float r, float g, float b) : r{r}, g{g}, b{b}, a{1}
        {
        }
        
        Color(float r, float g, float b, float a) : r{r}, g{g}, b{b}, a{a}
        {
        }
    };
}

#endif
