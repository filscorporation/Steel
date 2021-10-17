#pragma once

#include "EditorScene.h"

#define STYLE_GREY glm::vec4(204.0f / 256.0f, 204.0f / 256.0f, 204.0f / 256.0f, 1.0f)
#define STYLE_MID_GREY glm::vec4(152.0f / 256.0f, 152.0f / 256.0f, 152.0f / 256.0f, 1.0f)
#define STYLE_DARK_GREY glm::vec4(100.0f / 256.0f, 100.0f / 256.0f, 100.0f / 256.0f, 1.0f)
#define STYLE_BUTTON_W 80
#define STYLE_BUTTON_H 20

class EditorBuilder
{
public:
    static void BuildLayout(EditorScene* editorScene);
};
