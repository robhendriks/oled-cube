#ifndef Vector_h
#define Vector_h

#include <cmath>
#include <iostream>

namespace Math
{
    struct Vector4
    {
        float x, y, z, w;
    };
    
    struct Vector
    {        
        float x, y, z;
        
        Vector() : x{0}, y{0}, z{0}
        {
        }
        
        Vector(float x, float y, float z) : x{x}, y{y}, z{z}
        {
        }
        
        Vector(const Vector& vec) : x{vec.x}, y{vec.y}, z {vec.z}
        {
        }
        
        //
        // Methods
        //
        
        static Vector up()
        {
            return {0, -1, 0};
        }
        
        float length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        }
        
        float lengthSqrt() const
        {
            return x * x + y * y + z * z;
        }
        
        void normalize()
        {
            float len = length();
            
            if (!(std::abs(len) < 1e-6f)) {
                float inv = 1.0 / len;
                x *= inv;
                y *= inv;
                z *= inv;
            }
        }
        
        void print() const
        {
            std::cout << (*this) << '\n';
        }
        
        //
        // Assignment
        //
        
        Vector& operator=(const Vector& vec)
        {
            if (this != &vec) {
                x = vec.x;
                y = vec.y;
                z = vec.z;
            }
            return *this;
        }
        
        //
        // Increment and decrement
        //
        
        Vector& operator++()
        {
            ++x;
            ++y;
            ++z;
            return *this;
        }
        
        Vector operator++(int)
        {
            Vector tmp{*this};
            operator++();
            return tmp;
        }
        
        Vector& operator--()
        {
            --x;
            --y;
            --z;
            return *this;
        }
        
        Vector operator--(int)
        {
            Vector tmp{*this};
            operator--();
            return tmp;
        }
        
        //
        // Arithmetic
        //
        
        Vector& operator+=(float rhs)
        {
            x += rhs;
            y += rhs;
            z += rhs;
            return *this;
        }
        
        Vector& operator+=(const Vector& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        
        Vector& operator-=(float rhs)
        {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            return *this;
        }
        
        Vector& operator-=(const Vector& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        
        Vector& operator*=(float rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }
        
        Vector& operator*=(const Vector& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }
        
        Vector& operator/=(float rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }
        
        Vector& operator/=(const Vector& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }
        
        //
        // Stream extraction and insertion
        //
        
        friend std::ostream& operator<<(std::ostream& os, const Vector& vec);
    };
    
    //
    // Static methods
    //
    
    inline void cross(const Vector& a, const Vector& b, Vector& c)
    {
        c.x = (a.y * b.z) - (a.z * b.y);
        c.y = (a.z * b.x) - (a.x * b.z);
        c.z = (a.x * b.y) - (a.y * b.x);
    }
    
    inline Vector cross(const Vector& a, const Vector& b)
    {
        Vector tmp;
        cross(a, b, tmp);
        return tmp;
    }
    
    inline float dot(const Vector& a, const Vector& b)
    {
        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }
    
    inline void min(const Vector& a, const Vector& b, Vector& c)
    {
        c.x = std::min(a.x, b.x);
        c.y = std::min(a.y, b.y);
        c.z = std::min(a.z, b.z);
    }
    
    inline Vector min(const Vector& a, const Vector& b)
    {
        Vector tmp;
        min(a, b, tmp);
        return tmp;
    }
    
    inline void max(const Vector& a, const Vector& b, Vector& c)
    {
        c.x = std::max(a.x, b.x);
        c.y = std::max(a.y, b.y);
        c.z = std::max(a.z, b.z);
    }
    
    inline Vector max(const Vector& a, const Vector& b)
    {
        Vector tmp;
        max(a, b, tmp);
        return tmp;
    }
    
    //
    // Arithmetic
    //
    
    inline Vector operator+(Vector lhs, float rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    inline Vector operator+(Vector lhs, const Vector& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    inline Vector operator-(Vector lhs, float rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    inline Vector operator-(Vector lhs, const Vector& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    inline Vector operator*(Vector lhs, float rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    inline Vector operator*(Vector lhs, const Vector& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    inline Vector operator/(Vector lhs, float rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    
    inline Vector operator/(Vector lhs, const Vector& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    
    //
    // Relational
    //
    
    inline bool operator<(const Vector& lhs, const Vector& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    }
    
    inline bool operator>(const Vector& lhs, const Vector& rhs)
    {
        return rhs < lhs;
    }
    
    inline bool operator<=(const Vector& lhs, const Vector& rhs)
    {
        return !(lhs > rhs);
    }
    
    inline bool operator>=(const Vector& lhs, const Vector& rhs)
    {
        return !(lhs < rhs);
    }
    
    inline bool operator==(const Vector& lhs, const Vector& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }
    
    inline bool operator!=(const Vector& lhs, const Vector& rhs)
    {
        return !(lhs == rhs);
    }
    
    //
    // Stream extraction and insertion
    //
    
    inline std::ostream& operator<<(std::ostream& os, const Vector& vec)
    {
        os << "x:" << vec.x << " y:" << vec.y << " z:" << vec.z;
        return os;
    }
}

#endif
