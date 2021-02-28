#pragma once

#include <glm/vec3.hpp>

namespace ComponentsInternalCalls
{
    // Transformation
    glm::vec3 Transformation_GetPosition(int64_t entityID);
    void Transformation_SetPosition(int64_t entityID, glm::vec3 position);
    glm::vec3 Transformation_GetRotation(int64_t entityID);
    void Transformation_SetRotation(int64_t entityID, glm::vec3 rotation);
    glm::vec3 Transformation_GetScale(int64_t entityID);
    void Transformation_SetScale(int64_t entityID, glm::vec3 scale);

    // Sprite Renderer
    uint64_t SpriteRenderer_GetSprite(int64_t entityID);
    void SpriteRenderer_SetSprite(int64_t entityID, int64_t spriteID);

    // Animator
    void Animator_PlayAnimation(int64_t entityID, int64_t animationID);
    void Animator_Play(int64_t entityID);
    void Animator_Pause(int64_t entityID);
    void Animator_Stop(int64_t entityID);
    void Animator_Restart(int64_t entityID);
}
