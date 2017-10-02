#ifndef quaternion_h
#define quaternion_h

namespace Math
{
    struct Quaternion
    {
        float x, y, z, w;
        
        static void rotate(float yaw, float pitch, float roll, Quaternion& result)
        {
            float halfRoll = roll * 0.5;
            float halfPitch = pitch * 0.5;
            float halfYaw = yaw * 0.5;
            
            float sinRoll = std::sin(halfRoll);
            float cosRoll = std::cos(halfRoll);
            float sinPitch = std::sin(halfPitch);
            float cosPitch = std::cos(halfPitch);
            float sinYaw = std::sin(halfYaw);
            float cosYaw = std::cos(halfYaw);
            
            result.x = (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll);
            result.y = (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll);
            result.z = (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll);
            result.w = (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll);
        }
    };
}

#endif
