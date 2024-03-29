#include "EditorTypeSystem.h"
#include "../UI/AppView.h"
#include "../UI/ControlPanel.h"
#include "../UI/HierarchyElement.h"
#include "../UI/HierarchyView.h"
#include "../UI/ProjectView.h"
#include "../UI/ProjectViewElement.h"
#include "../UI/PropertiesView.h"
#include "../UI/SceneView.h"
#include "../UI/StatsWindow.h"
#include "../UI/UIEditorTab.h"

#include <Steel/EntityComponentSystem/TypeInfoStorage.h>

void EditorTypeSystem::Init()
{
    AppView::RegisterType();
    ControlPanel::RegisterType();
    HierarchyElement::RegisterType();
    HierarchyView::RegisterType();
    ProjectView::RegisterType();
    ProjectViewElement::RegisterType();
    PropertiesView::RegisterType();
    SceneView::RegisterType();
    StatsWindow::RegisterType();
    UIEditorTab::RegisterType();
}

void EditorTypeSystem::Terminate()
{
    TypeInfoStorage::Clear();
}
