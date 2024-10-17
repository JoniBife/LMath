#define _USE_MATH_DEFINES

#include "LMath/MathAux.hpp"

using namespace LMath;

#define EPSILON 0.00005f

bool LMath::cmpf(float A, float B)
{
    return fabsf(A - B) < EPSILON;
}

float LMath::round6(float number) {
    return roundf(number * powf(10, 6)) / powf(10, 6);
}

float LMath::randomFloat() {
    float a = 5.0;
    return (static_cast<float>(rand()) / static_cast<float>((RAND_MAX))) * a;
}

float LMath::randomFloat(float min, float max) {

    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))) + min;
}

float LMath::randomBinomial()
{
    return randomFloat(0.f, 1.f) - randomFloat(0.f, 1.f);
}

float LMath::degreesToRadians(float angle) {
    return (angle * static_cast<float>(M_PI)) / 180.0f;
}

float LMath::radiansToDegrees(float angleRad) {
    return (angleRad * 180.0f) / static_cast<float>(M_PI);
}

float LMath::lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

float LMath::clerp(float a, float b, float t)
{
    const float t2 = (1.f - cosf(t * PI)) / 2.f;
    return lerp(a, b , t2);
}

float LMath::smoothsteplerp(float a, float b, float t)
{
    const float t2 = t * t * (3.f - 2.f * t);
    return lerp(a, b , t2);
}
