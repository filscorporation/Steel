#include "InputHandler.h"

ButtonStates::ButtonState InputHandler::PressedKeys[MAX_KEY_CODE + 1];
ButtonStates::ButtonState InputHandler::PressedMouse[MAX_MOUSE_CODE + 1];
std::string InputHandler::TextInput;
glm::vec2 InputHandler::MousePosition;
glm::vec2 InputHandler::MouseScrollDelta;

bool InputHandler::KeyIsDirty;
bool InputHandler::MouseIsDirty;
bool InputHandler::ScrollDeltaIsDirty;
