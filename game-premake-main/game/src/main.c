#include "body.h"
#include "mathf.h"
#include "world.h"
#include "force.h"
#include "spring.h"
#include "integrator.h"
#include "render.h"
#include "editor.h"
#include "collision.h"
#include "contact.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	ncContact_t* contacts = NULL;

	// Init world
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);
	InitEditor();

	// Define cursor texture
	Texture2D cursorTexture = LoadTexture("C:\\Users\\Trevor Hicks\\Desktop\\Neumont College\\Third Year\\Spring 2024\\Game Physics\\Raylib Physics\\Raylib-Physics\\game-premake-main\\game\\src\\R.png");
	const float cursorScale = 0.1f; // Adjust the scale factor as needed
	Vector2 cursorSize = { (float)cursorTexture.width * cursorScale, (float)cursorTexture.height * cursorScale };

	ncGravity = (Vector2){ 0,-1 };

	// Game loop
	while (!WindowShouldClose())
	{
		// Update
		float dt = GetFrameTime();
		float fps = GetFrameTime();
		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position); // Update editor

		selectedBody = GetBodyIntersect(bodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixels(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			float angle = GetRandomFloatValue(0, 360);
			for (int i = 0; i < 1; i++)
			{
				ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.MassValue);
				body->damping = ncEditorData.DampingValue;
				body->gravityScale = ncEditorData.GravityScaleValue;
				body->color = WHITE;// ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			}
		}

		if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) && IsKeyPressed(KEY_T))
		{
			if (selectedBody)
			{
				connectBody = selectedBody;
			}
		}


		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody)
			DrawLineBodyToPosition(connectBody, position);

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			if (connectBody && selectedBody && connectBody != selectedBody)
			{
				// Check if the mouse is over the selected body when releasing the right mouse button
				if (CheckCollisionPointCircle(position, selectedBody->position, ConvertWorldToPixels(selectedBody->mass * 0.5f)))
				{
					//AddSpring(spring);
				}
					ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
			}
			connectBody = NULL; // Reset connectBody after releasing right mouse button
		}

		//apply force
		ApplyGravitation(bodies, ncEditorData.GravitationValue);
		ApplySpringForce(ncSprings);

		// Update collisions and contacts
		for (ncBody* body = bodies; body; body = body->next)
		{
			Step(body, dt);
		}

		ncContact_t* contacts = NULL;
		CreateContacts(bodies, &contacts); 
		SeparateContacts(contacts);
		ResolveContacts(contacts);

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Stats
		DrawText(TextFormat("FPS: %.2f(%.2ms)", fps, 1000 / fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.4f", dt), 10, 30, 20, LIME);

		// Draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}

		// Draw bodies
		for (ncBody* body = bodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixels(body->mass * 0.5f), body->color);
		}

		// Draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixels(contact->body1->mass * 0.5f), RED);
		}

		// Draw cursor texture
		DrawTextureEx(cursorTexture, (Vector2) { position.x - cursorSize.x / 2, position.y - cursorSize.y / 2 }, 0, cursorScale, WHITE);

		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();
	free(bodies);
	return 0;
}
