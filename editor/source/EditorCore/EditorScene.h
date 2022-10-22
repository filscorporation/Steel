#pragma once

#include <Steel.h>

class EditorScene : public Scene
{
public:
    EditorScene() : Scene("Editor scene", NULL_RESOURCE) { };
    ~EditorScene() override = default;

protected:
    void AfterUpdate() override;
    void BeforeRebuildUI() override;
    void AfterPrepareDraw() override;
};
