#ifndef matrix_h
#define matrix_h

#include <iostream>
#include <stdint.h>
#include <array>

#include "vector.h"

namespace Math
{
    const size_t M11 = 0, M12 = 4, M13 = 8, M14 = 12, M21 = 1, M22 = 5, M23 = 9, M24 = 13, M31 = 2, M32 = 6, M33 = 10, M34 = 14, M41 = 3, M42 = 7, M43 = 11, M44 = 15;
    
    struct Matrix
    {
        std::array<float, 16> data;
        
        Matrix()
        {
            identity();
        }
        
        Matrix(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44)
        {
            set(m11, m12, m13, m14,
                m21, m22, m23, m24,
                m31, m32, m33, m34,
                m41, m42, m43, m44);
        }
        
        Matrix(const Matrix& mat)
        {
            data = mat.data;
        }
        
        //
        // Methods
        //
        
        static void translate(const Vector& vec, Matrix& result)
        {
            result.identity();
            result.data[M41] = vec.x;
            result.data[M42] = vec.y;
            result.data[M43] = vec.z;
        }
        
        static void lookAt(const Vector& eye, const Vector& target, const Vector& up, Matrix &result)
        {
            Vector zAxis = target - eye; zAxis.normalize();
            Vector xAxis = cross(up, zAxis); xAxis.normalize();
            Vector yAxis = cross(zAxis, xAxis);
            
            result.identity();
            result.data[M11] = xAxis.x; result.data[M21] = xAxis.y; result.data[M31] = xAxis.z;
            result.data[M12] = yAxis.x; result.data[M22] = yAxis.y; result.data[M32] = yAxis.z;
            result.data[M13] = zAxis.x; result.data[M23] = zAxis.y; result.data[M33] = zAxis.z;
            
            result.data[M41] = -dot(xAxis, eye);
            result.data[M42] = -dot(yAxis, eye);
            result.data[M43] = -dot(zAxis, eye);
        }
        
        static void perspective(float fov, float aspect, float zNear, float zFar, Matrix& result)
        {
            float yScale = 1.0 / std::tan(fov * 0.5);
            float q = zFar / (zNear - zFar);
            
            result.identity();
            
            result.data[M11] = yScale / aspect;
            result.data[M22] = yScale;
            result.data[M33] = q;
            result.data[M34] = -1.0;
            result.data[M43] = q * zNear;
        }
        
        void identity()
        {
            data[M11] = 1; data[M12] = 0; data[M13] = 0; data[M14] = 0;
            data[M21] = 0; data[M22] = 1; data[M23] = 0; data[M24] = 0;
            data[M31] = 0; data[M32] = 0; data[M33] = 1; data[M34] = 0;
            data[M41] = 0; data[M42] = 0; data[M43] = 0; data[M44] = 1;
        }
        
        void set(float m11, float m12, float m13, float m14,
                 float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34,
                 float m41, float m42, float m43, float m44)
        {
            data[M11] = m11; data[M12] = m12; data[M13] = m13; data[M14] = m14;
            data[M21] = m21; data[M22] = m22; data[M23] = m23; data[M24] = m24;
            data[M31] = m31; data[M32] = m32; data[M33] = m33; data[M34] = m34;
            data[M41] = m41; data[M42] = m42; data[M43] = m43; data[M44] = m44;
        }
        
        void print()
        {
            std::cout << (*this) << '\n';
        }
        
        //
        // Arithmetic
        //
        
        Matrix& operator*=(const Matrix& rhs)
        {
            float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
            
            m11 = (data[M11] * rhs.data[M11]) + (data[M12] * rhs.data[M21]) + (data[M13] * rhs.data[M31]) + (data[M14] * rhs.data[M41]);
            m12 = (data[M11] * rhs.data[M12]) + (data[M12] * rhs.data[M22]) + (data[M13] * rhs.data[M32]) + (data[M14] * rhs.data[M42]);
            m13 = (data[M11] * rhs.data[M13]) + (data[M12] * rhs.data[M23]) + (data[M13] * rhs.data[M33]) + (data[M14] * rhs.data[M43]);
            m14 = (data[M11] * rhs.data[M14]) + (data[M12] * rhs.data[M24]) + (data[M13] * rhs.data[M34]) + (data[M14] * rhs.data[M44]);
            
            m21 = (data[M21] * rhs.data[M11]) + (data[M22] * rhs.data[M21]) + (data[M23] * rhs.data[M31]) + (data[M24] * rhs.data[M41]);
            m22 = (data[M21] * rhs.data[M12]) + (data[M22] * rhs.data[M22]) + (data[M23] * rhs.data[M32]) + (data[M24] * rhs.data[M42]);
            m23 = (data[M21] * rhs.data[M13]) + (data[M22] * rhs.data[M23]) + (data[M23] * rhs.data[M33]) + (data[M24] * rhs.data[M43]);
            m24 = (data[M21] * rhs.data[M14]) + (data[M22] * rhs.data[M24]) + (data[M23] * rhs.data[M34]) + (data[M24] * rhs.data[M44]);
            
            m31 = (data[M31] * rhs.data[M11]) + (data[M32] * rhs.data[M21]) + (data[M33] * rhs.data[M31]) + (data[M34] * rhs.data[M41]);
            m32 = (data[M31] * rhs.data[M12]) + (data[M32] * rhs.data[M22]) + (data[M33] * rhs.data[M32]) + (data[M34] * rhs.data[M42]);
            m33 = (data[M31] * rhs.data[M13]) + (data[M32] * rhs.data[M23]) + (data[M33] * rhs.data[M33]) + (data[M34] * rhs.data[M43]);
            m34 = (data[M31] * rhs.data[M14]) + (data[M32] * rhs.data[M24]) + (data[M33] * rhs.data[M34]) + (data[M34] * rhs.data[M44]);
            
            m41 = (data[M41] * rhs.data[M11]) + (data[M42] * rhs.data[M21]) + (data[M43] * rhs.data[M31]) + (data[M44] * rhs.data[M41]);
            m42 = (data[M41] * rhs.data[M12]) + (data[M42] * rhs.data[M22]) + (data[M43] * rhs.data[M32]) + (data[M44] * rhs.data[M42]);
            m43 = (data[M41] * rhs.data[M13]) + (data[M42] * rhs.data[M23]) + (data[M43] * rhs.data[M33]) + (data[M44] * rhs.data[M43]);
            m44 = (data[M41] * rhs.data[M14]) + (data[M42] * rhs.data[M24]) + (data[M43] * rhs.data[M34]) + (data[M44] * rhs.data[M44]);
            
            
            data[M11] = m11; data[M12] = m12; data[M13] = m13; data[M14] = m14;
            data[M21] = m21; data[M22] = m22; data[M23] = m23; data[M24] = m24;
            data[M31] = m31; data[M32] = m32; data[M33] = m33; data[M34] = m34;
            data[M41] = m41; data[M42] = m42; data[M43] = m43; data[M44] = m44;
            
            return *this;
        }
        
        //
        // Stream extraction and insertion
        //
        
        friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
    };
    
    //
    // Methods
    //
    
    inline void transform(const Vector& vec, const Matrix& mat, Vector& result)
    {
        Vector4 tmp;
        tmp.x = (vec.x * mat.data[M11]) + (vec.y * mat.data[M21]) + (vec.z * mat.data[M31]) + mat.data[M41];
        tmp.y = (vec.x * mat.data[M12]) + (vec.y * mat.data[M22]) + (vec.z * mat.data[M32]) + mat.data[M42];
        tmp.z = (vec.x * mat.data[M13]) + (vec.y * mat.data[M23]) + (vec.z * mat.data[M33]) + mat.data[M43];
        tmp.w = 1.0 / ((vec.x * mat.data[M14]) + (vec.y * mat.data[M24]) + (vec.z * mat.data[M34]) + mat.data[M44]);
        
        result.x = tmp.x * tmp.w;
        result.y = tmp.y * tmp.w;
        result.z = tmp.z * tmp.w;
    }
    
    //
    // Arithmetic
    //
    
    inline Matrix operator*(Matrix lhs, const Matrix& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    //
    // Stream extraction and insertion
    //
    
    inline std::ostream& operator<<(std::ostream& os, const Matrix& mat)
    {
        os << mat.data[M11] << ' ' << mat.data[M12] << ' ' << mat.data[M13] << ' ' << mat.data[M14] << '\n'
           << mat.data[M21] << ' ' << mat.data[M22] << ' ' << mat.data[M23] << ' ' << mat.data[M24] << '\n'
           << mat.data[M31] << ' ' << mat.data[M32] << ' ' << mat.data[M33] << ' ' << mat.data[M34] << '\n'
           << mat.data[M41] << ' ' << mat.data[M42] << ' ' << mat.data[M43] << ' ' << mat.data[M44];
        
        return os;
    }
}

#endif
