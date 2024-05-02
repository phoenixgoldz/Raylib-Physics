#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum
{
	STATIC,
	KINEMATIC,
	DYNAMIC

} ncBodyType;

typedef struct ncBody
{
	ncBodyType body;
	Vector2 position;
	Vector2 velocity;
	Vector2 force;
	float mass;
	float inverseMass;

	struct ncBody* next;
	struct ncBody* prev;

} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force)
{
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(ncBody* body)
{
	body->force = Vector2Zero();
}
