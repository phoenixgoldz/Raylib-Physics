#pragma once
#include "Body.h"

void ExplicitEuler(ncBody* body, float timestep)
{
	body->position = Vector2Add (body->position , Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add (body->velocity , Vector2Scale(Vector2Scale(body->force, 1/ body->mass), timestep));
}

void SemiImplicitEuler(ncBody* body, float timestep)
{
    Vector2 futureForce = body->force; 
    body->velocity = Vector2Add(body->velocity, Vector2Scale(Vector2Scale(futureForce, 1.0f / body->mass), timestep));

    body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
}
