#include "./editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950,50 };

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/syle_cyber.rgs");

        ncEditorData.GravitationValue = 2; 
        ncEditorData.MassMaxValue = 2; 
        ncEditorData.MassMinValue = 2; 
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor()
{
    //
}
