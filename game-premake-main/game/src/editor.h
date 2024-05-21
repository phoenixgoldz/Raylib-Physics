#pragma once
#include "raylib.h"

typedef struct ncEditorData
{
    Vector2 anchor01;
    bool EditorBoxActive;
    float MassValue;
    float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float DampingValue;
    float GravityScaleValue;
    float GravityValue;
    float StiffnessValue;

    // New variables for GUI elements
    float FixedTimeStep;
    bool Simulate;

} ncEditorData_t;

extern ncEditorData_t ncEditorData;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position);
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);