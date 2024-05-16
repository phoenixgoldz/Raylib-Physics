#pragma once

#include "raylib.h"
#include "raymath.h"

typedef enum {
    BT_STATIC,
    BT_KINEMATIC,
    BT_DYNAMIC
} ncBodyType;

typedef enum {
    FM_FORCE,
    FM_IMPULSE,
    FM_VELOCITY
} ncForceMode;

typedef struct ncBody {
    ncBodyType bodytype;
    Vector2 position;
    Vector2 velocity;
    Vector2 force;
    Vector2 acceleration;

    float mass;
    float inverseMass;
    float gravityScale;
    float damping;

    float restitution;

    Color color;

    int index;
    int initialIndex; 

    struct ncBody* next;
    struct ncBody* prev;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode)
{
    if (body->bodytype != BT_DYNAMIC) return;

    switch (forceMode)
    {
    case FM_FORCE:
        body->force = Vector2Add(body->force, force);
        break;
    case FM_IMPULSE:
        // applies a sudden change in momentum
        body->velocity = body->velocity,Vector2Scale(force, body->inverseMass);
        break;
    case FM_VELOCITY:
        body->velocity = force;
        break;
    }
}

inline void ClearForce(ncBody* body)
{
    body->force = Vector2Zero();
}

void Step(ncBody* body, float timestep);

