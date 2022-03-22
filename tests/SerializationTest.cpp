#include "TestsSetUp.h"
#include "common/SerializedComponent.h"
#include "Steel/Serialization/SerializationManager.h"

TEST(SerializationTest, SceneSerialization)
{
    auto scene = new Scene("Test scene");
    Application::Instance->GetSceneManager()->SetActiveScene(scene);
    scene->CreateEntity();
    scene->CreateEntity();
    scene->CreateEntity();

    YAML::Node sceneNode;
    SerializationManager::SerializeScene(scene, sceneNode);

    EXPECT_EQ(scene->GetName(), sceneNode["name"].as<std::string>());
}

TEST(SerializationTest, SceneDeserialization)
{
    auto scene = new Scene("");
    Application::Instance->GetSceneManager()->SetActiveScene(scene);

    YAML::Node sceneNode = YAML::Load("name: Test scene\n"
                                      "mainCamera: 3813675691461294676\n"
                                      "childrenCount: 2\n"
                                      "firstChildNode: 3813675691461294676\n"
                                      "entities:\n"
                                      "  - uuid: 3813675691461294676\n"
                                      "    state: 3\n"
                                      "    components:\n"
                                      "      HierarchyNode:\n"
                                      "        childrenCount: 0\n"
                                      "        firstChildNode: 0\n"
                                      "        hierarchyDepth: 0\n"
                                      "        nodeIndex: 0\n"
                                      "        thickness: 1\n"
                                      "        previousNode: 7064078186238868073\n"
                                      "        nextNode: 7064078186238868073\n"
                                      "        parentNode: 0\n"
                                      "      IDComponent:\n"
                                      "        uuid: 3813675691461294676\n"
                                      "      NameComponent:\n"
                                      "        name: Test entity1\n"
                                      "        tag: \"\"\n"
                                      "      Transformation:\n"
                                      "        localPosition: [0, 0, 0]\n"
                                      "        localRotation: [0, 0, 0]\n"
                                      "        localScale: [1, 1, 1]\n"
                                      "  - uuid: 7064078186238868073\n"
                                      "    state: 3\n"
                                      "    components:\n"
                                      "      HierarchyNode:\n"
                                      "        childrenCount: 0\n"
                                      "        firstChildNode: 0\n"
                                      "        hierarchyDepth: 0\n"
                                      "        nodeIndex: 1\n"
                                      "        thickness: 1\n"
                                      "        previousNode: 3813675691461294676\n"
                                      "        nextNode: 3813675691461294676\n"
                                      "        parentNode: 0\n"
                                      "      IDComponent:\n"
                                      "        uuid: 7064078186238868073\n"
                                      "      NameComponent:\n"
                                      "        name: Test entity2\n"
                                      "        tag: \"\"\n"
                                      "      Transformation:\n"
                                      "        localPosition: [0, 0, 0]\n"
                                      "        localRotation: [0, 0, 0]\n"
                                      "        localScale: [1, 1, 1]");

    SerializationManager::DeserializeScene(scene, sceneNode);

    EXPECT_EQ(scene->GetName(), "Test scene");
    EXPECT_EQ(scene->GetChildrenCount(), 2);
}

TEST(SerializationTest, ComponentSerialization)
{
    auto scene = new Scene("Test scene");
    Application::Instance->GetSceneManager()->SetActiveScene(scene);
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    UUID_TYPE entityUUID = registry->GetComponent<IDComponent>(entityID).GetUUID();
    auto& component = registry->AddComponent<SerializedComponent>(entityID);
    component.SetIntegerAttribute(71);
    component.SetFloatAttribute(3.14f);
    component.SetBoolAttribute(true);
    component.SetEntityIDAttribute(entityID);
    component.SetStringAttribute("Hello world!");
    component.SetVec2Attribute(glm::vec2(0.4f, 0.9f));
    component.SetVec3Attribute(glm::vec3(0.7f, 1.2f, -1.0f));
    component.SetVec4Attribute(glm::vec4(0.02f, 0.3f, 0.5f, 1.0f));
    std::vector<TestType> list { TestType(73), TestType(-934)};
    component.SetTestTypeListAttribute(list);

    YAML::Node sceneNode;
    SerializationManager::SerializeScene(scene, sceneNode);

    auto loadedScene = new Scene("");
    SerializationManager::DeserializeScene(loadedScene, sceneNode);
    Application::Instance->GetSceneManager()->SetActiveScene(loadedScene);
    auto loadedRegistry = loadedScene->GetEntitiesRegistry();

    EntityID loadedEntityID = loadedScene->GetEntityByUUID(entityUUID);
    EXPECT_NE(loadedEntityID, NULL_ENTITY);
    EXPECT_TRUE(loadedRegistry->HasComponent<SerializedComponent>(loadedEntityID));

    auto& loadedComponent = loadedRegistry->GetComponent<SerializedComponent>(loadedEntityID);

    EXPECT_EQ(loadedComponent.GetIntegerAttribute(), 71);
    EXPECT_EQ(loadedComponent.GetFloatAttribute(), 3.14f);
    EXPECT_EQ(loadedComponent.GetBoolAttribute(), true);
    EXPECT_EQ(loadedComponent.GetEntityIDAttribute(), entityID);
    EXPECT_EQ(loadedComponent.GetStringAttribute(), "Hello world!");
    EXPECT_EQ(loadedComponent.GetVec2Attribute(), glm::vec2(0.4f, 0.9f));
    EXPECT_EQ(loadedComponent.GetVec3Attribute(), glm::vec3(0.7f, 1.2f, -1.0f));
    EXPECT_EQ(loadedComponent.GetVec4Attribute(), glm::vec4(0.02f, 0.3f, 0.5f, 1.0f));
    auto& loadedList = loadedComponent.GetTestTypeListAttribute();
    EXPECT_EQ(loadedList.size(), 2);
    EXPECT_EQ(loadedList[0].GetValue(), 73);
    EXPECT_EQ(loadedList[1].GetValue(), -934);
}
