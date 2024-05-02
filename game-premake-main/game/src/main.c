#include "raylib.h"
#include "World.h"
#include "integrator.h"

#include <Mathf.h>
#include <stdlib.h>

extern ncBody* bodies;
extern int ncbodyCount;

int main(void) {
	InitWindow(1280, 720, "Raylib Physics Engine");
	SetTargetFPS(60);

	// Game loop
	while (!WindowShouldClose()) 
	{

		// Update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition(); // Correctly declared as Vector2

		if (IsMouseButtonDown(0)) {
			ncBody* newBody = CreateBody();
			newBody->position = position;
			newBody->mass = GetRandomFloatValue(5,10);
		}

		//apply force
		ncBody* body = bodies;
		while (body)
		{
			ApplyForce(body, CreateVector2(0, -50));
			body = body->next;
		}

		// Update bodies
		body = bodies;
		while (body)
		{
			ExplicitEuler(body, dt);
			body->position.x += body->velocity.x;
			body->position.y += body->velocity.y;
			body = body->next;
		}

		// Render or Draw
		BeginDrawing();
		ClearBackground(BLACK);

		// Stats
		DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000.0f / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.2f ", dt), 10, 30, 20, LIME);

		// Draw bodies
		body = bodies;
		while (body)
		{
			DrawCircle(body->position.x, body->position.y, body->mass, RED);
			body = body->next;
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
