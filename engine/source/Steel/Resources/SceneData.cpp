#include "SceneData.h"

SceneData::SceneData(const std::string& name)
{
    Type = ResourceTypes::SceneData;

    // we do not require path in constructor, so scene ResourceID will be generated randomly
    // this needs to be kept in mind after scene is saved and gets path
    _name = name;
}

const std::string& SceneData::GetName()
{
    return _name;
}
