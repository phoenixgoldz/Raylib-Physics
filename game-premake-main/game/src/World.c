#include "World.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* bodies = NULL;
int ncbodyCount = 0;

ncBody* CreateBody() {
    ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
    assert(newBody != NULL);

    memset(newBody, 0, sizeof(ncBody));

    newBody->position = (Vector2){ 0, 0 };
    newBody->velocity = (Vector2){ 0, 0 };
    newBody->force = (Vector2){ 0, 0 };

    newBody->prev = NULL;
    newBody->next = bodies;

    if (bodies != NULL) {
        bodies->prev = newBody;
    }

    bodies = newBody;

    ncbodyCount++;

    return newBody;
}

void DestroyBody(ncBody* body) {
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
