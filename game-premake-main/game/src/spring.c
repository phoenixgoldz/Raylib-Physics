#include "spring.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restlength, float k)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restlength;
	spring->k = k;

	AddSpring(spring);

	return spring;
}

void AddSpring(ncSpring_t* spring)
{

	assert(spring);
	spring->prev = NULL;
	spring->next = &ncSprings[0];

	if (ncSprings)
	{
		ncSprings[0].prev = spring;
	}

	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);


	if (spring->prev) spring->prev->next = spring->next;
	if (spring->next) spring->next->prev = spring->prev;


	free(spring);
}

void DestroyAllSprings()
{
	while (ncSprings)
	{
		ncSpring_t* next = ncSprings->next;
		free(ncSprings);
		ncSprings = next;
	}
}


float Vector2Magnitude(Vector2 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		struct ncBody* body1 = spring->body1;
		struct ncBody* body2 = spring->body2;

		Vector2 direction = Vector2Subtract(body1->position, body2->position);
		if (direction.x == 0 && direction.y == 0)
			continue;

		float length = Vector2Magnitude(direction);
		float displacement = length - spring->restLength;
		if (displacement == 0)
			continue;

		Vector2 ndirection = Vector2Normalize(direction);

		// Apply force using Hooke's law: F = -k * (displacement)
		float force = -spring->k * displacement;
		Vector2 forceVector = Vector2Scale(ndirection, force);

		ApplyForce(body1, forceVector, FM_FORCE);
		ApplyForce(body2, Vector2Negate(forceVector), FM_FORCE);
	}
}
void ApplySpringForcePosition(Vector2 position, struct ncBody* body, float restLength, float k, float damping)
{
	// Check if the body pointer is null; if so, exit the function
	if (!body) return;

	// Calculate the direction vector from the body's position to the given position
	Vector2 direction = Vector2Subtract(position, body->position);

	// If the direction vector is zero (i.e., positions are the same), exit the function
	if (direction.x == 0 && direction.y == 0) return;

	// Calculate the length of the direction vector (distance between the two positions)
	float length = Vector2Length(direction);

	// Calculate the displacement from the rest length
	float displacement = length - restLength;

	// Apply Hooke's Law (F = -k * x) to determine the spring force
	float springForce = -k * displacement;

	// Normalize the direction vector
	Vector2 ndirection = Vector2Normalize(direction);

	// Calculate the damping force (opposing force due to velocity)
	float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection);

	// Calculate the total force by combining spring force and damping force
	float totalForce = springForce + dampingForce;

	// Apply the total force to the body in the direction of the normalized vector
	Vector2 forceVector = Vector2Scale(ndirection, -totalForce);
	ApplyForce(body, forceVector, FM_FORCE);
}