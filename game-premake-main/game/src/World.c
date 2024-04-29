#include <stdlib.h>
#include <assert.h>
#include "world.h"

Body* bodies = NULL;
int bodyCount = 0;

Body* CreateBody() {
    Body* newBody = (Body*)malloc(sizeof(Body));
    assert(newBody != NULL);

    newBody->position = (Vector2){ 0, 0 };
    newBody->velocity = (Vector2){ 0, 0 };
    newBody->force = (Vector2){ 0, 0 };

    newBody->prev = NULL;
    newBody->next = bodies;

    if (bodies != NULL) {
        bodies->prev = newBody;
    }

    bodies = newBody;

    bodyCount++;

    return newBody;
}

void DestroyBody(Body* body) {
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

    bodyCount--;

    free(body);
}
