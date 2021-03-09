#pragma once

#define GET_COMPONENT_OR_RETURN(m_class, m_return) \
if (!HasComponentS<m_class>(entityID)) \
{ \
    Log::LogError("Entity " + std::to_string(entityID) + " does not exist or have no " #m_class "component attached"); \
    return m_return; \
} \
auto& component = GetComponentS<m_class>(entityID);
