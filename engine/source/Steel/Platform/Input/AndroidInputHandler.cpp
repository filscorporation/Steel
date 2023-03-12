#include "AndroidInputHandler.h"

void AndroidInputHandler::PollEvents()
{

}

void AndroidInputHandler::HandleInputEvent(AInputEvent* event)
{
    auto eventType = AInputEvent_getType(event);
    switch (eventType)
    {
        case AINPUT_EVENT_TYPE_MOTION:
        {
            auto x = AMotionEvent_getX(event, 0);
            auto y = AMotionEvent_getY(event, 0);
            InputHandler::MousePosition = glm::vec2(x, y);

            switch (AInputEvent_getSource(event))
            {
                case AINPUT_SOURCE_TOUCHSCREEN:
                    int action = AKeyEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK;
                    switch(action)
                    {
                        case AMOTION_EVENT_ACTION_DOWN:
                            InputHandler::PressedMouse[0] = ButtonStates::JustPressed;
                            break;
                        case AMOTION_EVENT_ACTION_UP:
                            InputHandler::PressedMouse[0] = ButtonStates::JustReleased;
                            break;
                        case AMOTION_EVENT_ACTION_MOVE:
                            break;
                    }
                    break;
            }
            break;
        }
        case AINPUT_EVENT_TYPE_KEY:
            break;
        default:
            break;
    }
}
