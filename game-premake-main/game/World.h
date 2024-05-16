#pragma once

#include "raylib.h"
#include "Body.h"

extern ncBody* bodies;
extern int ncbodyCount;
extern Vector2 ncGravity;
//
//void InitWorld();
//void UpdateWorld(float dt);
//void Step(ncBody* body, float timestep);

ncBody* CreateBody(Vector2 position,float mass, ncBodyType bodyType );
void DestroyBody(ncBody* body);
void AddBody(ncBody* body);


