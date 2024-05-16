#pragma once
#include "raylib.h"

#include <stdlib.h>

// Function to generate a random float value between 0 and 1
inline float GetRandomFloatValue01()
{
    return rand() / (float)RAND_MAX;
}

// Function to generate a random float value between min and max
inline float GetRandomFloatValue(float min, float max)
{
    return min + (max - min) * GetRandomFloatValue01();
}

// Function to create a Vector2 with given x and y coordinates
inline Vector2 CreateVector2(float x, float y)
{
    return (Vector2) { x, y };
}
