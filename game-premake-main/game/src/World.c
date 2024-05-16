#include "World.h"
#include "Body.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

ncBody* bodies = NULL;
int ncbodyCount = 0;
Vector2 ncGravity = { 0, 30 };

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
	ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
	assert(newBody);

	memset(newBody, 0, sizeof(ncBody));
	newBody->position = position;
	newBody->mass = mass;
	newBody->inverseMass = (bodyType = BT_DYNAMIC) ? 1 / mass : 0;
	newBody->bodytype = bodyType;
	AddBody(newBody);

	return newBody;
}
void AddBody(ncBody* body)
{
	assert(body);
	body->prev = NULL;
	body->next = bodies;

	if (bodies != NULL) {
		bodies->prev = body;
	}
	bodies = body;

	ncbodyCount++;
}

void DestroyBody(ncBody* body) 
{
	assert(body != NULL);

	if (bodies == body) {
		bodies = body->next;
	}
	if (body->prev != NULL) {
		body->prev->next = body->next;
	}
	if (body->next != NULL) {
		body->next->prev = body->prev;
	}

	ncbodyCount--;
	free(body);
}



