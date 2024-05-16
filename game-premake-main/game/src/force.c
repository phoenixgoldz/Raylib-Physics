#include "force.h"
#include "Body.h"

void ApplyGravitation(ncBody* bodies, float strength)
{
    for (ncBody* body1 = bodies; body1; body1 = body1->next)
    {
        for (ncBody* body2 = bodies; body2; body2 = body2->next)
        {
            if (body1 == body2) 
                continue;

            Vector2 direction = Vector2Subtract(body2->position, body1->position);
            float distance = Vector2Length(direction);

            distance = fmaxf(distance, 1.0f); 

            float force = (body1->mass * body2->mass / (distance * distance)) * strength;

            direction = Vector2Normalize(direction);
            direction = Vector2Scale(direction, force);

            ApplyForce(body1, direction, 1);
            ApplyForce(body2, Vector2Negate(direction), 1);
        }
    }
}
