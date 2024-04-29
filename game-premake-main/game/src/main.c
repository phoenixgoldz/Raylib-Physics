#include "raylib.h"
#include "World.h"
#include <stdlib.h>

#define MAX_BODIES 10000

float GetRandomFloatValue(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

int main(void) {
    InitWindow(1280, 720, "Raylib Physics Engine");
    SetTargetFPS(120);

    bodyCount = 0; // Reset body count
    bodies = NULL; // Reset bodies list

    // Game loop
    while (!WindowShouldClose()) {
        // Update
        float dt = GetFrameTime();
        float fps = (float)GetFPS();

        Vector2 position = GetMousePosition();
        if (IsMouseButtonDown(0)) {
            Body* newBody = CreateBody();
            newBody->position = position;
            newBody->velocity = (Vector2){ GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5) };
        }

        // Render
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle((int)position.x, (int)position.y, 10, YELLOW);
        for (Body* body = bodies; body != NULL; body = body->next) {
            body->position.x += body->velocity.x;
            body->position.y += body->velocity.y;
            DrawCircle((int)body->position.x, (int)body->position.y, 10, RED);
        }


        // Stats
        DrawText(TextFormat("FPS: %.2f (%.2f ms)", fps, 1000 / fps), 10, 10, 20, LIME);
        DrawText(TextFormat("FRAME: %.2f ", dt, 1000 / fps), 10, 30, 20, LIME);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
