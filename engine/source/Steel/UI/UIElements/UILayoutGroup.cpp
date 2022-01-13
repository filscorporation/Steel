#include "UILayoutGroup.h"
#include "../../Core/Application.h"
#include "../../Core/Log.h"

#define GET_ELEMENT(m_id) \
(entitiesRegistry->HasComponent<UILayoutGroup>(m_id) \
    ? (ILayoutElement&)entitiesRegistry->GetComponent<UILayoutGroup>(m_id) \
    : entitiesRegistry->HasComponent<UILayoutElement>(m_id) \
    ? (ILayoutElement&)entitiesRegistry->GetComponent<UILayoutElement>(m_id) \
    : (ILayoutElement&)entitiesRegistry->AddComponent<UILayoutElement>(m_id))

void UILayoutGroup::RegisterType()
{
    REGISTER_TYPE(UILayoutGroup);
    REGISTER_ID_ATTRIBUTE(UILayoutGroup, "groupID", GetGroupID, SetGroupID, AttributeFlags::Public);
    REGISTER_ENUM_ATTRIBUTE(UILayoutGroup, "type", GetType, SetType, LayoutGroupTypes::LayoutGroupType, AttributeFlags::Public);
}

bool UILayoutGroup::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UILayoutGroup::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<UILayoutElement>(Owner))
    {
        Log::LogWarning("UILayoutElement component will be replaced with UILayoutGroup");
        entitiesRegistry->RemoveComponent<UILayoutElement>(Owner);
    }
    entitiesRegistry->AddComponent<UIEventHandler>(Owner);

    // TODO: bind event handler to layout elements resizing
}

void UILayoutGroup::Rebuild(UILayer* layer, RectTransformation& transformation)
{
    if (!transformation.DidSizeChange() && !LayoutGroupDirty)
        return;

    LayoutGroupDirty = false;

    if (elements.empty())
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    struct ElementRebuildInfo
    {
        ElementRebuildInfo(LayoutElementInfo _info, int _index) { Info = _info; Index = _index; }
        LayoutElementInfo Info { };
        int Index;
    };
    std::vector<ElementRebuildInfo> elementsInfo;
    std::vector<float> elementSize;

    for (int i = 0; i < elements.size(); ++i)
    {
        if (!entitiesRegistry->EntityExists(elements[i]))
        {
            Log::LogWarning("Layout group contains deleted element");
            return;
        }

        elementsInfo.emplace_back(GET_ELEMENT(elements[i]).GetInfo(), i);
    }

    struct
    {
        inline bool operator() (const ElementRebuildInfo& a, const ElementRebuildInfo& b)
        {
            return (a.Info.PreferredWidth - a.Info.MinWidth < b.Info.PreferredWidth - b.Info.MinWidth);
        }
    } HorInfoComparer;
    struct
    {
        inline bool operator() (const ElementRebuildInfo& a, const ElementRebuildInfo& b)
        {
            return (a.Info.PreferredHeight - a.Info.MinHeight < b.Info.PreferredHeight - b.Info.MinHeight);
        }
    } VerInfoComparer;
    if (_type == LayoutGroupTypes::Horizontal)
        std::sort(elementsInfo.begin(), elementsInfo.end(), HorInfoComparer);
    else
        std::sort(elementsInfo.begin(), elementsInfo.end(), VerInfoComparer);

    float sumMin = 0;
    for (auto& info : elementsInfo)
    {
        sumMin += _type == LayoutGroupTypes::Horizontal ? info.Info.MinWidth : info.Info.MinHeight;
    }

    glm::vec2 size = transformation.GetRealSizeCached();
    float available = _type == LayoutGroupTypes::Horizontal ? size.x : size.y;
    available -= sumMin;
    for (int i = 0; i < elementsInfo.size(); ++i)
    {
        float ms = (_type == LayoutGroupTypes::Horizontal ? elementsInfo[i].Info.MinWidth : elementsInfo[i].Info.MinHeight);
        float ps = (_type == LayoutGroupTypes::Horizontal ? elementsInfo[i].Info.PreferredWidth : elementsInfo[i].Info.PreferredHeight);
        float s = std::min(ps - ms, available / (float)(elementsInfo.size() - i));
        available -= s;
        elementSize.push_back(ms + s);
    }

    for (int i = 0; i < elementsInfo.size(); ++i)
    {
        elementSize[i] += available / (float)(elementsInfo.size());
    }

    float offset = 0;
    for (int i = 0; i < elements.size(); ++i)
    {
        auto& elementRT = entitiesRegistry->GetComponent<RectTransformation>(elements[i]);
        elementRT.SetPivot(glm::vec2(0.0f, 0.0f));
        elementRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        elementRT.SetAnchorMax(glm::vec2(0.0f, 0.0f));
        elementRT.SetOffsetMin(glm::vec2(0.0f, 0.0f));
        elementRT.SetOffsetMax(glm::vec2(0.0f, 0.0f));
        elementRT.SetAnchoredPosition(_type == LayoutGroupTypes::Horizontal
            ? glm::vec2(offset, 0.0f) : glm::vec2(0.0f, offset));
        elementRT.SetSize(_type == LayoutGroupTypes::Horizontal
            ? glm::vec2(elementSize[elementsInfo[i].Index], size.y) : glm::vec2(size.x, elementSize[elementsInfo[i].Index]));
        offset += elementSize[elementsInfo[i].Index];
    }
}

LayoutElementInfo UILayoutGroup::GetInfo()
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    LayoutElementInfo info { };

    for (EntityID elementID : elements)
    {
        if (!entitiesRegistry->EntityExists(elementID))
            continue;

        LayoutElementInfo elementInfo = GET_ELEMENT(elementID).GetInfo();
        if (_type == LayoutGroupTypes::Horizontal)
        {
            info.MinWidth += elementInfo.MinWidth;
            info.MinHeight = std::max(info.MinHeight, elementInfo.MinHeight);
            info.PreferredWidth += elementInfo.PreferredWidth;
            info.PreferredHeight = std::max(info.PreferredHeight, elementInfo.PreferredHeight);
        }
        else
        {
            info.MinWidth = std::max(info.MinWidth, elementInfo.MinWidth);
            info.MinHeight += elementInfo.MinHeight;
            info.PreferredWidth = std::max(info.PreferredWidth, elementInfo.PreferredWidth);
            info.PreferredHeight += elementInfo.PreferredHeight;
        }
    }

    return info;
}

EntityID UILayoutGroup::AddElement(LayoutElementInfo info)
{
    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    info.PreferredHeight = std::max(info.PreferredHeight, info.MinHeight);
    info.PreferredWidth = std::max(info.PreferredWidth, info.MinWidth);

    EntityID elementEntity = layer->CreateUIElement("Layout element", Owner);
    AddElement(elementEntity);
    auto& element = entitiesRegistry->GetComponent<UILayoutElement>(elementEntity);
    element.Info = info;

    return elementEntity;
}

EntityID UILayoutGroup::AddElement(EntityID elementID)
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& element = GET_ELEMENT(elementID);

    if (element.GetGroupID() != NULL_ENTITY)
        Log::LogError("Changing layout element {0} group ID", elementID);
    element.SetGroupID(Owner);

    elements.emplace_back(elementID);

    return elementID;
}

void UILayoutGroup::SetType(LayoutGroupTypes::LayoutGroupType type)
{
    _type = type;
}

LayoutGroupTypes::LayoutGroupType UILayoutGroup::GetType() const
{
    return _type;
}
