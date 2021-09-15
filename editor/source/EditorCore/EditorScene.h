#pragma once

#include <Iron.h>

class EditorScene : public Scene
{
public:
    EditorScene() : Scene() { };
    ~EditorScene() override = default;

protected:
    void BeforeRebuildUI() override;
    void AfterPrepareDraw() override;
};
