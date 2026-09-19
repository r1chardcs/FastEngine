//
// Created by dlllibstdntc on 19.09.2026.
//

#include "Input.h"

#include "glfw3.h"
#include "Engine/App.h"

void Input::InitializeInput() {
    AddMapping(Action::MoveUp, GLFW_KEY_W, GLFW_KEY_UP);
    AddMapping(Action::MoveDown, GLFW_KEY_S, GLFW_KEY_DOWN);
    AddMapping(Action::MoveLeft, GLFW_KEY_A, GLFW_KEY_LEFT);
    AddMapping(Action::MoveRight, GLFW_KEY_D, GLFW_KEY_RIGHT);

    AddMapping(Action::Button1, GLFW_KEY_1);
    AddMapping(Action::Button2, GLFW_KEY_2);

    AddMapping(Action::Jump, GLFW_KEY_SPACE);
    AddMapping(Action::Quit, GLFW_KEY_ESCAPE);

    AddMapping(Action::Fullscreen, GLFW_KEY_F11);

    AddMapping(Action::Button1, GLFW_MOUSE_BUTTON_LEFT);
    AddMapping(Action::Button2, GLFW_MOUSE_BUTTON_RIGHT);
}

void Input::UpdateKeys() {
    previous_keys = keys;
    previous_mouse_keys = mouse_keys;

    const auto window = App::GetInstance().GetWindow();

    for (const auto& [action, mapping] : mappings) {
        BOOL key_down = false;
        BOOL mouse_down = false;

        for (const auto key : mapping) {
            if (window->GetKey(key)) {
                key_down = true;
                break;
            }
        }

        for (const auto key : mapping) {
            if (window->GetMouseKey(key)) {
                mouse_down = true;
                break;
            }
        }

        keys[action] = key_down;
        mouse_keys[action] = mouse_down;
    }
}

BOOL Input::Down(const UINT action) {
    return keys[action];
}

BOOL Input::Pressed(const UINT action) {
    return keys[action] && !previous_keys[action];
}

BOOL Input::Released(const UINT action) {
    return !keys[action] && previous_keys[action];
}

BOOL Input::MouseDown(const UINT action) {
    return mouse_keys[action];
}

BOOL Input::MousePressed(const UINT action) {
    return mouse_keys[action] && !previous_mouse_keys[action];
}

BOOL Input::MouseReleased(const UINT action) {
    return !mouse_keys[action] && previous_mouse_keys[action];
}