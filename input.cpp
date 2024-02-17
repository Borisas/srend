#include "input.h"
#include "core.h"

srend::Input::Input() {


    //automatic
    // for (int i = 0; ; ++i) {
    //     SDL_Keycode currentKey = static_cast<SDL_Keycode>(i);

    //     const char* keyName = SDL_GetKeyName(currentKey);

    //     // Check for an empty string or a known default value indicating the end of keycodes
    //     if (keyName[0] == '\0' || currentKey == SDLK_UNKNOWN) {
    //         break;
    //     } else {
    //         _keys[currentKey] = none;
    //     }
    // }

    //manual. faster
    //todo: remove all unneeded prolly.
    const SDL_Keycode ALL_KEYS[] = { SDLK_UNKNOWN,SDLK_RETURN,SDLK_ESCAPE,SDLK_BACKSPACE,SDLK_TAB,SDLK_SPACE,SDLK_EXCLAIM,SDLK_QUOTEDBL,SDLK_HASH,SDLK_PERCENT,SDLK_DOLLAR,SDLK_AMPERSAND,SDLK_QUOTE,SDLK_LEFTPAREN,SDLK_RIGHTPAREN,SDLK_ASTERISK,SDLK_PLUS,SDLK_COMMA,SDLK_MINUS,SDLK_PERIOD,SDLK_SLASH,SDLK_0,SDLK_1,SDLK_2,SDLK_3,SDLK_4,SDLK_5,SDLK_6,SDLK_7,SDLK_8,SDLK_9,SDLK_COLON,SDLK_SEMICOLON,SDLK_LESS,SDLK_EQUALS,SDLK_GREATER,SDLK_QUESTION,SDLK_AT,SDLK_LEFTBRACKET,SDLK_BACKSLASH,SDLK_RIGHTBRACKET,SDLK_CARET,SDLK_UNDERSCORE,SDLK_BACKQUOTE,SDLK_a,SDLK_b,SDLK_c,SDLK_d,SDLK_e,SDLK_f,SDLK_g,SDLK_h,SDLK_i,SDLK_j,SDLK_k,SDLK_l,SDLK_m,SDLK_n,SDLK_o,SDLK_p,SDLK_q,SDLK_r,SDLK_s,SDLK_t,SDLK_u,SDLK_v,SDLK_w,SDLK_x,SDLK_y,SDLK_z,SDLK_CAPSLOCK,SDLK_F1,SDLK_F2,SDLK_F3,SDLK_F4,SDLK_F5,SDLK_F6,SDLK_F7,SDLK_F8,SDLK_F9,SDLK_F10,SDLK_F11,SDLK_F12,SDLK_PRINTSCREEN,SDLK_SCROLLLOCK,SDLK_PAUSE,SDLK_INSERT,SDLK_HOME,SDLK_PAGEUP,SDLK_DELETE,SDLK_END,SDLK_PAGEDOWN,SDLK_RIGHT,SDLK_LEFT,SDLK_DOWN,SDLK_UP,SDLK_NUMLOCKCLEAR,SDLK_KP_DIVIDE,SDLK_KP_MULTIPLY,SDLK_KP_MINUS,SDLK_KP_PLUS,SDLK_KP_ENTER,SDLK_KP_1,SDLK_KP_2,SDLK_KP_3,SDLK_KP_4,SDLK_KP_5,SDLK_KP_6,SDLK_KP_7,SDLK_KP_8,SDLK_KP_9,SDLK_KP_0,SDLK_KP_PERIOD,SDLK_APPLICATION,SDLK_POWER,SDLK_KP_EQUALS,SDLK_F13,SDLK_F14,SDLK_F15,SDLK_F16,SDLK_F17,SDLK_F18,SDLK_F19,SDLK_F20,SDLK_F21,SDLK_F22,SDLK_F23,SDLK_F24,SDLK_EXECUTE,SDLK_HELP,SDLK_MENU,SDLK_SELECT,SDLK_STOP,SDLK_AGAIN,SDLK_UNDO,SDLK_CUT,SDLK_COPY,SDLK_PASTE,SDLK_FIND,SDLK_MUTE,SDLK_VOLUMEUP,SDLK_VOLUMEDOWN,SDLK_KP_COMMA,SDLK_KP_EQUALSAS400,SDLK_ALTERASE,SDLK_SYSREQ,SDLK_CANCEL,SDLK_CLEAR,SDLK_PRIOR,SDLK_RETURN2,SDLK_SEPARATOR,SDLK_OUT,SDLK_OPER,SDLK_CLEARAGAIN,SDLK_CRSEL,SDLK_EXSEL,SDLK_KP_00,SDLK_KP_000,SDLK_THOUSANDSSEPARATOR,SDLK_DECIMALSEPARATOR,SDLK_CURRENCYUNIT,SDLK_CURRENCYSUBUNIT,SDLK_KP_LEFTPAREN,SDLK_KP_RIGHTPAREN,SDLK_KP_LEFTBRACE,SDLK_KP_RIGHTBRACE,SDLK_KP_TAB,SDLK_KP_BACKSPACE,SDLK_KP_A,SDLK_KP_B,SDLK_KP_C,SDLK_KP_D,SDLK_KP_E,SDLK_KP_F,SDLK_KP_XOR,SDLK_KP_POWER,SDLK_KP_PERCENT,SDLK_KP_LESS,SDLK_KP_GREATER,SDLK_KP_AMPERSAND,SDLK_KP_DBLAMPERSAND,SDLK_KP_VERTICALBAR,SDLK_KP_DBLVERTICALBAR,SDLK_KP_COLON,SDLK_KP_HASH,SDLK_KP_SPACE,SDLK_KP_AT,SDLK_KP_EXCLAM,SDLK_KP_MEMSTORE,SDLK_KP_MEMRECALL,SDLK_KP_MEMCLEAR,SDLK_KP_MEMADD,SDLK_KP_MEMSUBTRACT,SDLK_KP_MEMMULTIPLY,SDLK_KP_MEMDIVIDE,SDLK_KP_PLUSMINUS,SDLK_KP_CLEAR,SDLK_KP_CLEARENTRY,SDLK_KP_BINARY,SDLK_KP_OCTAL,SDLK_KP_DECIMAL,SDLK_KP_HEXADECIMAL,SDLK_LCTRL,SDLK_LSHIFT,SDLK_LALT,SDLK_LGUI,SDLK_RCTRL,SDLK_RSHIFT,SDLK_RALT,SDLK_RGUI,SDLK_MODE,SDLK_AUDIONEXT,SDLK_AUDIOPREV,SDLK_AUDIOSTOP,SDLK_AUDIOPLAY,SDLK_AUDIOMUTE,SDLK_MEDIASELECT,SDLK_WWW,SDLK_MAIL,SDLK_CALCULATOR,SDLK_COMPUTER,SDLK_AC_SEARCH,SDLK_AC_HOME,SDLK_AC_BACK,SDLK_AC_FORWARD,SDLK_AC_STOP,SDLK_AC_REFRESH,SDLK_AC_BOOKMARKS,SDLK_BRIGHTNESSDOWN,SDLK_BRIGHTNESSUP,SDLK_DISPLAYSWITCH,SDLK_KBDILLUMTOGGLE,SDLK_KBDILLUMDOWN,SDLK_KBDILLUMUP,SDLK_EJECT,SDLK_SLEEP,SDLK_APP1,SDLK_APP2,SDLK_AUDIOREWIND,SDLK_AUDIOFASTFORWARD,SDLK_SOFTLEFT,SDLK_SOFTRIGHT,SDLK_CALL,SDLK_ENDCALL };

    for ( const auto & k : ALL_KEYS ) {
        _keys[k] = none;
    }

    _keysToReset.reserve(32);


    _mouse[left] = none;
    _mouse[right] = none;
    _mouse[middle] = none;
    _mouse[x_1] = none;
    _mouse[x_2] = none;

    _mouseToReset.reserve(5);
}

void srend::Input::frameStart() {

    _resetKeys();
}

void srend::Input::process(const SDL_Event & e) {


    switch(e.type) {
        case SDL_QUIT:
        break;
        case SDL_KEYDOWN:
        // events
            _onKeyDown(e.key.keysym.sym);
        break;
        case SDL_KEYUP:
            _onKeyUp(e.key.keysym.sym);
        break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            int x, y;
            auto mouse = SDL_GetMouseState(&x,&y);
            _mousePosition.x = x;
            _mousePosition.y = _screenHeight - y;

            if ( mouse != 0 || e.type == SDL_MOUSEBUTTONUP ) {
                _onMouse(mouse, e.type);
            }
        break;
    }
}

bool srend::Input::keyDown(SDL_Keycode k) {
    return _keys[k] == down;
}

bool srend::Input::keyUp(SDL_Keycode k) {
    return _keys[k] == up;
}

bool srend::Input::keyHeld(SDL_Keycode k) {
    return _keys[k] == held || _keys[k] == down;
}

bool srend::Input::mouseUp(MouseButton b) {
  return _mouse[b] == up;
}

bool srend::Input::mouseDown(MouseButton b) {
  return _mouse[b] == down;
}

const srend::float2& srend::Input::mousePosition() {
    return _mousePosition;
}

void srend::Input::_onKeyDown(SDL_Keycode key) {
    if ( _keys[key] == down ) {
        _keys[key] = held;
    }
    else {
        _keys[key] = down;
    }
}

void srend::Input::_onKeyUp(SDL_Keycode key) {
    _keys[key] = up;
    _keysToReset.push_back(key);
}

void srend::Input::_onMouse(uint32 flag, uint32 eventType) {

    if ( eventType == SDL_MOUSEBUTTONDOWN ) {

        if ( flag & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
            _onMouseDown(left);
        }
        if ( flag & SDL_BUTTON(SDL_BUTTON_MIDDLE) ) {
            _onMouseDown(middle);
        }
        if ( flag & SDL_BUTTON(SDL_BUTTON_RIGHT) ) {
            _onMouseDown(right);
        }
        if ( flag & SDL_BUTTON(SDL_BUTTON_X1) ) {
            _onMouseDown(x_1);
        }
        if ( flag & SDL_BUTTON(SDL_BUTTON_X2) ) {
            _onMouseDown(x_2);
        }
    }
    else if ( eventType == SDL_MOUSEBUTTONUP ) {
        
        if ( !(flag & SDL_BUTTON(SDL_BUTTON_LEFT)) ) {
            _onMouseUp(left);
        }
        if ( !(flag & SDL_BUTTON(SDL_BUTTON_MIDDLE)) ) {
            _onMouseUp(middle);
        }
        if ( !(flag & SDL_BUTTON(SDL_BUTTON_RIGHT)) ) {
            _onMouseUp(right);
        }
        if ( !(flag & SDL_BUTTON(SDL_BUTTON_X1)) ) {
            _onMouseUp(x_1);
        }
        if ( !(flag & SDL_BUTTON(SDL_BUTTON_X2)) ) {
            _onMouseUp(x_2);
        }
    }
}

void srend::Input::_onMouseDown(MouseButton mouse) {

    _mouse[mouse] = down;
}

void srend::Input::_onMouseUp(MouseButton mouse) {
    if ( _mouse[mouse] != down ) return;

    _mouse[mouse] = up;
    _mouseToReset.push_back(mouse);
}

void srend::Input::_resetKeys() {

    for ( const auto & k : _keysToReset ) {
        _keys[k] = none;
    }

    _keysToReset.clear();

    for ( const auto & m : _mouseToReset ) {
        _mouse[m] = none;
    }
    _mouseToReset.clear();
}