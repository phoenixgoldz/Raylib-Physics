#include "editor.h"
#include "Body.h"
#include "render.h"

#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;
Texture2D cursorTexture;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();

    ncEditorData.anchor01 = (Vector2){ 950, 48 };
    ncEditorData.EditorBoxActive = true;

    // Initialize body parameters
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.MassValue = 2.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 1.0f;
    ncEditorData.StiffnessValue = 10.0f;

    // Initialize world parameters
    ncEditorData.GravityValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;

    // Initialize fixed timestep and simulation control
    ncEditorData.FixedTimeStep = 1.0f / 50.0f; // Default value
    ncEditorData.Simulate = true; // Default value

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };
}

void UpdateEditor(Vector2 position)
{
    // Toggle show/hide editor box with key press
    if (IsKeyPressed(KEY_TAB)) ncEditorData.EditorBoxActive = !ncEditorData.EditorBoxActive;

    // Check if cursor position is intersecting the editor box 
    ncEditorIntersect = ncEditorData.EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 312, 608 }, "Editor");

        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 24, ncEditorData.anchor01.y + 48, 264, 216 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 144, 120, 16 }, "Mass", TextFormat("%0.2f", ncEditorData.MassValue), & ncEditorData.MassValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 168, 120, 16 }, "Damping", TextFormat("%0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 192, 120, 16 }, "Gravity Scale", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 216, 120, 16 }, "Stiffness (k)", TextFormat("%0.2f", ncEditorData.StiffnessValue), & ncEditorData.StiffnessValue, 0, 40);

        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 24, ncEditorData.anchor01.y + 336, 264, 200 }, "World");
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 384, 120, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, 0, 40);
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 360, 120, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, -10, 10);

        // GUI Slider for Fixed Time Step
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 112, ncEditorData.anchor01.y + 264, 120, 16 }, "Fixed Time Step", TextFormat("%0.3f", ncEditorData.FixedTimeStep), & ncEditorData.FixedTimeStep, 1.0f / 120.0f, 1.0f / 30.0f);

        // Toggle Button for Simulation Control
        if (GuiButton((Rectangle) { ncEditorData.anchor01.x + 24, ncEditorData.anchor01.y + 288, 120, 24 }, ncEditorData.Simulate ? "Stop Simulation" : "Start Simulation"))
        {
            ncEditorData.Simulate = !ncEditorData.Simulate;
        }

        // Button to Reset Simulation
        if (GuiButton((Rectangle) { ncEditorData.anchor01.x + 150, ncEditorData.anchor01.y + 288, 120, 24 }, "Reset Simulation"))
        {
            // Function to reset the simulation will be implemented in main.c
            // We'll set a flag here
            ResetSimulation();
        }

        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 40, ncEditorData.anchor01.y + 96, 192, 24 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, (Color) { 255, 255, 255, 255 });

    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixels(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x, (int)position.y, YELLOW);
}