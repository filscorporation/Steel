#include "SelectionController.h"
#include "HierarchyView.h"
#include "ProjectView.h"

void SelectionController::ClearSelection(EntitiesRegistry* entitiesRegistry)
{
    auto projectViewIterator = entitiesRegistry->GetComponentIterator<ProjectView>();
    for (int i = 0; i < projectViewIterator.Size(); ++i)
    {
        if (projectViewIterator[i].IsAlive())
        {
            projectViewIterator[i].DeselectAll(entitiesRegistry);
        }
    }

    auto hierarchyViewIterator = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViewIterator.Size(); ++i)
    {
        if (hierarchyViewIterator[i].IsAlive())
        {
            hierarchyViewIterator[i].DeselectAll(entitiesRegistry);
        }
    }
}
