#include "AudioListener.h"
#include "AudioCore.h"
#include "../Core/Application.h"
#include "../Scene/Transformation.h"
#include "../Scene/SceneHelper.h"

void AudioListener::OnUpdate()
{
    if (Application::Instance->GetCurrentScene()->IsEntityDestroyed(Owner))
        return;

    auto& transform = GetComponentS<Transformation>(Owner);
    if (transform.IsTransformationDirty())
        AudioCore::SetListenerPosition(transform.GetPosition());
}

void AudioListener::SetVolume(float volume)
{
    if (listenerVolume != volume)
    {
        listenerVolume = volume;
        AudioCore::SetListenerVolume(volume);
    }
}

float AudioListener::GetVolume() const
{
    return listenerVolume;
}
