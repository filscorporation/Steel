#include "UILayoutGroup.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Serialization/AttributesRegistration.h"

void UILayoutGroup::RegisterType()
{
    REGISTER_COMPONENT(UILayoutGroup);
    REGISTER_ENUM_ATTRIBUTE(UILayoutGroup, "type", GetType, SetType, LayoutGroupTypes::LayoutGroupType, AttributeFlags::Public, "Type");
    REGISTER_LIST_ATTRIBUTE(UILayoutGroup, "elements", GetElementsList, SetElementsList, UILayoutElementInfo, AttributeFlags::Public, "Elements");
}

bool UILayoutGroup::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UILayoutGroup::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    entitiesRegistry->AddComponent<UIEventHandler>(Owner);
}

void UILayoutGroup::Rebuild(UILayer* layer, RectTransformation& transformation)
{
    if (!transformation.DidSizeChange())
        return;

    if (_elements.empty())
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    struct ElementRebuildInfo
    {
        ElementRebuildInfo(UILayoutElementInfo _info, int _index) { Info = _info; Index = _index; }
        UILayoutElementInfo Info;
        int Index;
    };
    std::vector<ElementRebuildInfo> elementsInfo;
    std::vector<float> elementSize;

    for (int i = 0; i < _elements.size(); ++i)
    {
        if (!entitiesRegistry->EntityExists(_elements[i].ElementID))
        {
            Log::LogWarning("Layout group contains deleted element");
            return;
        }

        bool isGroup = entitiesRegistry->HasComponent<UILayoutGroup>(_elements[i].ElementID);
        UILayoutElementInfo updatedElementInfo = isGroup
             ? entitiesRegistry->GetComponent<UILayoutGroup>(_elements[i].ElementID).GetInfo()
             : _elements[i];

        elementsInfo.emplace_back(updatedElementInfo, i);
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
    for (int i = 0; i < _elements.size(); ++i)
    {
        auto& elementRT = entitiesRegistry->GetComponent<RectTransformation>(_elements[i].ElementID);
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

UILayoutElementInfo UILayoutGroup::GetInfo()
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    UILayoutElementInfo updatedInfo { };

    for (UILayoutElementInfo& elementInfo : _elements)
    {
        bool isGroup = entitiesRegistry->EntityExists(elementInfo.ElementID)
                && entitiesRegistry->HasComponent<UILayoutGroup>(elementInfo.ElementID);
        UILayoutElementInfo updatedElementInfo = isGroup
                ? entitiesRegistry->GetComponent<UILayoutGroup>(elementInfo.ElementID).GetInfo()
                : elementInfo;

        if (_type == LayoutGroupTypes::Horizontal)
        {
            updatedInfo.MinWidth += updatedElementInfo.MinWidth;
            updatedInfo.MinHeight = std::max(updatedInfo.MinHeight, updatedElementInfo.MinHeight);
            updatedInfo.PreferredWidth += updatedElementInfo.PreferredWidth;
            updatedInfo.PreferredHeight = std::max(updatedInfo.PreferredHeight, updatedElementInfo.PreferredHeight);
        }
        else
        {
            updatedInfo.MinWidth = std::max(updatedInfo.MinWidth, updatedElementInfo.MinWidth);
            updatedInfo.MinHeight += updatedElementInfo.MinHeight;
            updatedInfo.PreferredWidth = std::max(updatedInfo.PreferredWidth, updatedElementInfo.PreferredWidth);
            updatedInfo.PreferredHeight += updatedElementInfo.PreferredHeight;
        }
    }

    return updatedInfo;
}

EntityID UILayoutGroup::AddElement(float minWidth, float minHeight, float prefWidth, float prefHeight)
{
    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();

    EntityID elementID = layer->CreateUIElement("Layout element", Owner);

    UILayoutElementInfo info;
    info.ElementID = elementID;
    info.MinWidth = minWidth;
    info.MinHeight = minHeight;
    info.PreferredWidth = std::max(prefWidth, minWidth);
    info.PreferredHeight = std::max(prefHeight, minHeight);

    _elements.push_back(info);

    return elementID;
}

EntityID UILayoutGroup::AddElement(EntityID elementID)
{
    for (auto element : _elements)
    {
        if (element.ElementID == elementID)
        {
            Log::LogWarning("Element {0} is already in group", elementID);
            return elementID;
        }
    }

    UILayoutElementInfo info;
    info.ElementID = elementID;

    _elements.push_back(info);

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

const std::vector<UILayoutElementInfo>& UILayoutGroup::GetElementsList() const
{
    return _elements;
}

void UILayoutGroup::SetElementsList(const std::vector<UILayoutElementInfo>& elements)
{
    _elements = elements;
}
