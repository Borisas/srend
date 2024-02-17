#pragma once
#include "SDL2/SDL.h"
#include <unordered_map>
#include <vector>
#include "types.h"

namespace srend {

    enum KeyState {
        none = 0,
        up = 1,
        down = 2,
        held = 3
    };

    enum MouseButton {
        left = 0,
        right = 1,
        middle = 2,
        x_1 = 3,
        x_2 = 4
    };

    class Input {

        public:

            Input();

            void frameStart();
            void process(const SDL_Event &);

            bool keyDown(SDL_Keycode);
            bool keyUp(SDL_Keycode);
            bool keyHeld(SDL_Keycode);

            bool mouseUp(MouseButton);
            bool mouseDown(MouseButton);

            const float2 & mousePosition();
        private:

            void _onKeyDown(SDL_Keycode);
            void _onKeyUp(SDL_Keycode);

            void _onMouse(uint32, uint32);
            void _onMouseDown(MouseButton);
            void _onMouseUp(MouseButton);

            void _resetKeys();

            std::unordered_map<SDL_Keycode, KeyState> _keys;
            std::vector<SDL_Keycode> _keysToReset;
            
            srend::float2 _mousePosition;
            std::unordered_map<MouseButton, KeyState> _mouse;
            std::vector<MouseButton> _mouseToReset;
            
    };
}