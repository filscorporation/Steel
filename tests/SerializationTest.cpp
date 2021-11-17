#include "TestsSetUp.h"
#include "common/SerializedComponent.h"
#include "Steel/Serialization/SerializationManager.h"

TEST(SerializationTest, ToString)
{
    SerializedComponent::RegisterType();

    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    auto& component = registry->AddComponent<SerializedComponent>(entityID);
    component.SetIntegerAttribute(71);
    component.SetFloatAttribute(3.14f);
    component.SetBoolAttribute(true);
    component.SetEntityIDAttribute(entityID);
    component.SetStringAttribute("Hello world!");
    component.SetVec2Attribute(glm::vec2(0.4f, 0.9f));
    component.SetVec3Attribute(glm::vec3(0.7f, 1.2f, -1.0f));
    component.SetVec4Attribute(glm::vec4(0.02f, 0.3f, 0.5f, 1.0f));

    auto& attributes = SerializationManager::GetAttributes(CLASS_ID(SerializedComponent));
    int attributesCount = 0;
    for (auto& attribute : attributes)
    {
        if (attribute.Name() == "integerAttribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<int>(attribute.ToString(&component)), 71);
        }
        if (attribute.Name() == "floatAttribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<float>(attribute.ToString(&component)), 3.14f);
        }
        if (attribute.Name() == "boolAttribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<bool>(attribute.ToString(&component)), true);
        }
        if (attribute.Name() == "entityIDAttribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<EntityID>(attribute.ToString(&component)), entityID);
        }
        if (attribute.Name() == "stringAttribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<std::string>(attribute.ToString(&component)), "Hello world!");
        }
        if (attribute.Name() == "vec2Attribute")
        {
            attributesCount++;
            auto result = attribute.ToString(&component);
            EXPECT_EQ(StringUtils::FromString<glm::vec2>(attribute.ToString(&component)), glm::vec2(0.4f, 0.9f));
        }
        if (attribute.Name() == "vec3Attribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<glm::vec3>(attribute.ToString(&component)), glm::vec3(0.7f, 1.2f, -1.0f));
        }
        if (attribute.Name() == "vec4Attribute")
        {
            attributesCount++;
            EXPECT_EQ(StringUtils::FromString<glm::vec4>(attribute.ToString(&component)), glm::vec4(0.02f, 0.3f, 0.5f, 1.0f));
        }
    }

    EXPECT_EQ(attributesCount, 8);
}

TEST(SerializationTest, FromString)
{
    SerializedComponent::RegisterType();

    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    auto& component = registry->AddComponent<SerializedComponent>(entityID);

    auto& attributes = SerializationManager::GetAttributes(CLASS_ID(SerializedComponent));
    int attributesCount = 0;
    for (auto& attribute : attributes)
    {
        if (attribute.Name() == "integerAttribute")
        {
            attributesCount++;
            attribute.FromString(&component, "71");
        }
        if (attribute.Name() == "floatAttribute")
        {
            attributesCount++;
            attribute.FromString(&component, "3.14f");
        }
        if (attribute.Name() == "boolAttribute")
        {
            attributesCount++;
            attribute.FromString(&component, "1");
        }
        if (attribute.Name() == "entityIDAttribute")
        {
            attributesCount++;
            attribute.FromString(&component, StringUtils::ToString(entityID));
        }
        if (attribute.Name() == "stringAttribute")
        {
            attributesCount++;
            attribute.FromString(&component, "Hello world!");
        }
        if (attribute.Name() == "vec2Attribute")
        {
            attributesCount++;
            attribute.FromString(&component, StringUtils::ToString(glm::vec2(0.4f, 0.9f)));
        }
        if (attribute.Name() == "vec3Attribute")
        {
            attributesCount++;
            attribute.FromString(&component, StringUtils::ToString(glm::vec3(0.7f, 1.2f, -1.0f)));
        }
        if (attribute.Name() == "vec4Attribute")
        {
            attributesCount++;
            attribute.FromString(&component, StringUtils::ToString(glm::vec4(0.02f, 0.3f, 0.5f, 1.0f)));
        }
    }

    EXPECT_EQ(attributesCount, 8);
    EXPECT_EQ(component.GetIntegerAttribute(), 71);
    EXPECT_EQ(component.GetFloatAttribute(), 3.14f);
    EXPECT_EQ(component.GetBoolAttribute(), true);
    EXPECT_EQ(component.GetEntityIDAttribute(), entityID);
    EXPECT_EQ(component.GetStringAttribute(), "Hello world!");
    EXPECT_EQ(component.GetVec2Attribute(), glm::vec2(0.4f, 0.9f));
    EXPECT_EQ(component.GetVec3Attribute(), glm::vec3(0.7f, 1.2f, -1.0f));
    EXPECT_EQ(component.GetVec4Attribute(), glm::vec4(0.02f, 0.3f, 0.5f, 1.0f));
}
