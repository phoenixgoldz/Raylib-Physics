#include "collision.h"
#include "contact.h"
#include "body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ncBody* body1, ncBody* body2)
{
    float distance = Vector2Distance(body1->position, body2->position);
    float radius = body1->mass + body2->mass;

    return (distance <= radius);
}

void CreateContacts(ncBody* bodies, ncContact_t** contacts)
{
    for (ncBody* body1 = bodies; body1; body1 = body1->next)
    {
        for (ncBody* body2 = body1->next; body2; body2 = body2->next)
        {
            if (body1->bodytype != BT_DYNAMIC && body2->bodytype != BT_DYNAMIC)
                continue;

            if (Intersects(body1, body2))
            {
                ncContact_t* contact = GenerateContact(body1, body2);
                AddContact(contact, contacts);
            }
        }
    }
}

ncContact_t* GenerateContact(ncBody* body1, ncBody* body2)
{
    ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
    assert(contact);

    memset(contact, 0, sizeof(ncContact_t));

    contact->body1 = body1;
    contact->body2 = body2;

    Vector2 direction = Vector2Subtract(body1->position, body2->position);
    float distance = Vector2Length(direction);
    if (distance == 0)
    {
        direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
        distance = Vector2Length(direction);
    }

    float radius = body1->mass + body2->mass;

    contact->depth = radius - distance;
    contact->normal = Vector2Normalize(direction);
    contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

    return contact;
}

void SeparateContacts(ncContact_t* contacts)
{
    for (ncContact_t* contact = contacts; contact; contact = contact->next)
    {
        float separationImpulse = contact->depth * 0.5f; // Separate by half the depth
        Vector2 separationVector = Vector2Scale(contact->normal, separationImpulse);

        if (contact->body1->bodytype == BT_DYNAMIC)
            contact->body1->position = Vector2Add(contact->body1->position, separationVector);
        if (contact->body2->bodytype == BT_DYNAMIC)
            contact->body2->position = Vector2Subtract(contact->body2->position, separationVector);
    }
}

void ResolveContacts(ncContact_t* contacts)
{
    for (ncContact_t* contact = contacts; contact; contact = contact->next)
    {
        // Calculate relative velocity
        Vector2 relativeVelocity = Vector2Subtract(contact->body2->velocity, contact->body1->velocity);

        // Calculate relative velocity in terms of the normal direction
        float velocityAlongNormal = Vector2DotProduct(relativeVelocity, contact->normal);

        // Do not resolve if velocities are separating
        if (velocityAlongNormal > 0)
            continue;

        // Calculate restitution
        float e = contact->restitution;

        // Calculate impulse 
        float j = -(1 + e) * velocityAlongNormal;
        j /= (1 / contact->body1->mass + 1 / contact->body2->mass);

        // Apply impulse to contact
        Vector2 impulse = Vector2Scale(contact->normal, j);
        if (contact->body1->bodytype == BT_DYNAMIC)
            contact->body1->velocity = Vector2Subtract(contact->body1->velocity, Vector2Scale(impulse, 1 / contact->body1->mass));
        if (contact->body2->bodytype == BT_DYNAMIC)
            contact->body2->velocity = Vector2Add(contact->body2->velocity, Vector2Scale(impulse, 1 / contact->body2->mass));
    }
}
