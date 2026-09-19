//
// Created by dlllibstdntc on 19.09.2026.
//

#ifndef FASTENGINE_INPUT_H
#define FASTENGINE_INPUT_H

#include <Platform.h>

namespace Input
{
    enum Action {
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        Button1,
        Button2,
        Jump,
        Quit,
        Fullscreen,
    };

    void InitializeInput();

    INLINE HASH_MAP<UINT, LIST<INT>> mappings;

    INLINE HASH_MAP<UINT, BOOL> keys;
    INLINE HASH_MAP<UINT, BOOL> previous_keys;

    INLINE HASH_MAP<UINT, BOOL> mouse_keys;
    INLINE HASH_MAP<UINT, BOOL> previous_mouse_keys;

    template <typename... Args>
    void AddMapping(const UINT action, Args&&... args) {
        mappings[action] = LIST<INT>{static_cast<INT>(std::forward<Args>(args))...};
    }

    void UpdateKeys();

    BOOL Down(UINT action);
    BOOL Pressed(UINT action);
    BOOL Released(UINT action);

    BOOL MouseDown(UINT action);
    BOOL MousePressed(UINT action);
    BOOL MouseReleased(UINT action);
}

#endif //FASTENGINE_INPUT_H