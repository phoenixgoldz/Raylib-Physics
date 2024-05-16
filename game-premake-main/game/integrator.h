#pragma once

#include "Body.h"

void Step(ncBody* body, float timestep); 

inline void ExplicitEuler(ncBody* body, float timestep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(Vector2Scale(body->force, 1.0f / body->mass), timestep));
}

inline void SemiImplicitEuler(ncBody* body, float timestep) {
	
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->force, 1.0f / body->mass));

	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
}
