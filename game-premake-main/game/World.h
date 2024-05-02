#pragma once
#include "Body.h"

extern ncBody* bodies;
extern int ncbodyCount;

ncBody* CreateBody();
void DestroyBody(ncBody* body);
void DestroyBody();
