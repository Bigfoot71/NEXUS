/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#ifndef NEXUS_INPUTS_KEYBOARD_HPP
#define NEXUS_INPUTS_KEYBOARD_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <SDL_events.h>

namespace nexus { namespace input {

    /**
     * @brief Enumeration of key codes.
     *
     * This enum defines identifiers for various keyboard key codes.
     */
    enum class KeyCode : Uint32
    {
        Unknown                 = SDLK_UNKNOWN,

        Return                  = SDLK_RETURN,
        Escape                  = SDLK_ESCAPE,
        Backspace               = SDLK_BACKSPACE,
        Tab                     = SDLK_TAB,
        Space                   = SDLK_SPACE,
        Exclaim                 = SDLK_EXCLAIM,
        QuotedBL                = SDLK_QUOTEDBL,
        Hash                    = SDLK_HASH,
        Percent                 = SDLK_PERCENT,
        Dollar                  = SDLK_DOLLAR,
        Ampersand               = SDLK_AMPERSAND,
        Quote                   = SDLK_QUOTE,
        LeftParen               = SDLK_LEFTPAREN,
        RightParen              = SDLK_RIGHTPAREN,
        Asterisk                = SDLK_ASTERISK,
        Plus                    = SDLK_PLUS,
        Comma                   = SDLK_COMMA,
        Minus                   = SDLK_MINUS,
        Period                  = SDLK_PERIOD,
        Slash                   = SDLK_SLASH,
        Zero                    = SDLK_0,
        One                     = SDLK_1,
        Two                     = SDLK_2,
        Three                   = SDLK_3,
        Four                    = SDLK_4,
        Five                    = SDLK_5,
        Six                     = SDLK_6,
        Seven                   = SDLK_7,
        Eight                   = SDLK_8,
        Nine                    = SDLK_9,
        Colon                   = SDLK_COLON,
        Semicolon               = SDLK_SEMICOLON,
        Less                    = SDLK_LESS,
        Equals                  = SDLK_EQUALS,
        Greater                 = SDLK_GREATER,
        Question                = SDLK_QUESTION,
        At                      = SDLK_AT,

        // Skip uppercase letters

        LeftBracket             = SDLK_LEFTBRACKET,
        BackSlash               = SDLK_BACKSLASH,
        RightBracket            = SDLK_RIGHTBRACKET,
        Caret                   = SDLK_CARET,
        Underscore              = SDLK_UNDERSCORE,
        BackQuote               = SDLK_BACKQUOTE,
        A                       = SDLK_a,
        B                       = SDLK_b,
        C                       = SDLK_c,
        D                       = SDLK_d,
        E                       = SDLK_e,
        F                       = SDLK_f,
        G                       = SDLK_g,
        H                       = SDLK_h,
        I                       = SDLK_i,
        J                       = SDLK_j,
        K                       = SDLK_k,
        L                       = SDLK_l,
        M                       = SDLK_m,
        N                       = SDLK_n,
        O                       = SDLK_o,
        P                       = SDLK_p,
        Q                       = SDLK_q,
        R                       = SDLK_r,
        S                       = SDLK_s,
        T                       = SDLK_t,
        U                       = SDLK_u,
        V                       = SDLK_v,
        W                       = SDLK_w,
        X                       = SDLK_x,
        Y                       = SDLK_y,
        Z                       = SDLK_z,

        CapsLock                = SDLK_CAPSLOCK,

        F1                      = SDLK_F1,
        F2                      = SDLK_F2,
        F3                      = SDLK_F3,
        F4                      = SDLK_F4,
        F5                      = SDLK_F5,
        F6                      = SDLK_F6,
        F7                      = SDLK_F7,
        F8                      = SDLK_F8,
        F9                      = SDLK_F9,
        F10                     = SDLK_F10,
        F11                     = SDLK_F11,
        F12                     = SDLK_F12,

        PrintScreen             = SDLK_PRINTSCREEN,
        ScrollLock              = SDLK_SCROLLLOCK,
        Pause                   = SDLK_PAUSE,
        Insert                  = SDLK_INSERT,
        Home                    = SDLK_HOME,
        PageUp                  = SDLK_PAGEUP,
        Delete                  = SDLK_DELETE,
        End                     = SDLK_END,
        PageDown                = SDLK_PAGEDOWN,
        Right                   = SDLK_RIGHT,
        Left                    = SDLK_LEFT,
        Down                    = SDLK_DOWN,
        Up                      = SDLK_UP,

        NumLockClear            = SDLK_NUMLOCKCLEAR,
        KpDivide                = SDLK_KP_DIVIDE,
        KpMultiply              = SDLK_KP_MULTIPLY,
        kpMinus                 = SDLK_KP_MINUS,
        KpPlus                  = SDLK_KP_PLUS,
        KpEnter                 = SDLK_KP_ENTER,
        KpOne                   = SDLK_KP_1,
        KpTwo                   = SDLK_KP_2,
        KpThree                 = SDLK_KP_3,
        KpFour                  = SDLK_KP_4,
        KpFive                  = SDLK_KP_5,
        KpSix                   = SDLK_KP_6,
        KpSeven                 = SDLK_KP_7,
        KpEight                 = SDLK_KP_8,
        KpNine                  = SDLK_KP_9,
        KpZero                  = SDLK_KP_0,
        KpPeriod                = SDLK_KP_PERIOD,

        Application             = SDLK_APPLICATION,
        Power                   = SDLK_POWER,
        KpEquals                = SDLK_KP_EQUALS,
        F13                     = SDLK_F13,
        F14                     = SDLK_F14,
        F15                     = SDLK_F15,
        F16                     = SDLK_F16,
        F17                     = SDLK_F17,
        F18                     = SDLK_F18,
        F19                     = SDLK_F19,
        F20                     = SDLK_F20,
        F21                     = SDLK_F21,
        F22                     = SDLK_F22,
        F23                     = SDLK_F23,
        F24                     = SDLK_F24,
        Execute                 = SDLK_EXECUTE,
        Help                    = SDLK_HELP,
        Menu                    = SDLK_MENU,
        Select                  = SDLK_SELECT,
        Stop                    = SDLK_STOP,
        Again                   = SDLK_AGAIN,
        Undo                    = SDLK_UNDO,
        Cut                     = SDLK_CUT,
        Copy                    = SDLK_COPY,
        Paste                   = SDLK_PASTE,
        Find                    = SDLK_FIND,
        Mute                    = SDLK_MUTE,
        VolumeUp                = SDLK_VOLUMEUP,
        VolumeDown              = SDLK_VOLUMEDOWN,
        KpComma                 = SDLK_KP_COMMA,
        KpEqualsAs400           = SDLK_KP_EQUALSAS400,

        AltErase                = SDLK_ALTERASE,
        SysReq                  = SDLK_SYSREQ,
        Cancel                  = SDLK_CANCEL,
        Clear                   = SDLK_CLEAR,
        Prior                   = SDLK_PRIOR,
        Return2                 = SDLK_RETURN2,
        Separator               = SDLK_SEPARATOR,
        Out                     = SDLK_OUT,
        Oper                    = SDLK_OPER,
        ClearAgain              = SDLK_CLEARAGAIN,
        CRSEL                   = SDLK_CRSEL,
        EXSEL                   = SDLK_EXSEL,

        Kp00                    = SDLK_KP_00,
        Kp000                   = SDLK_KP_000,
        ThousandsSeparator      = SDLK_THOUSANDSSEPARATOR,
        DecimalSeparator        = SDLK_DECIMALSEPARATOR,
        CurrencyUnit            = SDLK_CURRENCYUNIT,
        CurrencySubUnit         = SDLK_CURRENCYSUBUNIT,
        KpLeftParen             = SDLK_KP_LEFTPAREN,
        KpRightParen            = SDLK_KP_RIGHTPAREN,
        KpLeftBrace             = SDLK_KP_LEFTBRACE,
        KpRightBrace            = SDLK_KP_RIGHTBRACE,
        KpTab                   = SDLK_KP_TAB,
        KpBackspace             = SDLK_KP_BACKSPACE,
        KpA                     = SDLK_KP_A,
        KpB                     = SDLK_KP_B,
        KpC                     = SDLK_KP_C,
        KpD                     = SDLK_KP_D,
        KpE                     = SDLK_KP_E,
        KpF                     = SDLK_KP_F,
        KpXOR                   = SDLK_KP_XOR,
        KpPower                 = SDLK_KP_POWER,
        KpPercent               = SDLK_KP_PERCENT,
        KpLess                  = SDLK_KP_LESS,
        KpGreater               = SDLK_KP_GREATER,
        KpAmpersand             = SDLK_KP_AMPERSAND,
        KpDBLAmpersand          = SDLK_KP_DBLAMPERSAND,
        KpVerticalBar           = SDLK_KP_VERTICALBAR,
        KpDBLVerticalBar        = SDLK_KP_DBLVERTICALBAR,
        KpColon                 = SDLK_KP_COLON,
        KpHash                  = SDLK_KP_HASH,
        KpSpace                 = SDLK_KP_SPACE,
        KpAt                    = SDLK_KP_AT,
        KpExclam                = SDLK_KP_EXCLAM,
        KpMemStore              = SDLK_KP_MEMSTORE,
        KpMemRecall             = SDLK_KP_MEMRECALL,
        KpMemClear              = SDLK_KP_MEMCLEAR,
        KpMemAdd                = SDLK_KP_MEMADD,
        KpMemSubtract           = SDLK_KP_MEMSUBTRACT,
        KpMemMultiply           = SDLK_KP_MEMMULTIPLY,
        KpMemDivide             = SDLK_KP_MEMDIVIDE,
        KpPlusMinus             = SDLK_KP_PLUSMINUS,
        KpClear                 = SDLK_KP_CLEAR,
        KpClearEntry            = SDLK_KP_CLEARENTRY,
        KpBinary                = SDLK_KP_BINARY,
        KpOctal                 = SDLK_KP_OCTAL,
        KpDecimal               = SDLK_KP_DECIMAL,
        KpHexadecimal           = SDLK_KP_HEXADECIMAL,

        LCtrl                   = SDLK_LCTRL,
        LShift                  = SDLK_LSHIFT,
        LAlt                    = SDLK_LALT,
        LGui                    = SDLK_LGUI,
        RCtrl                   = SDLK_RCTRL,
        RShift                  = SDLK_RSHIFT,
        RAlt                    = SDLK_RALT,
        RGui                    = SDLK_RGUI,

        Mode                    = SDLK_MODE,

        AudioNext               = SDLK_AUDIONEXT,
        AudioPrev               = SDLK_AUDIOPREV,
        AudioStop               = SDLK_AUDIOSTOP,
        AudioPlay               = SDLK_AUDIOPLAY,
        AudioMute               = SDLK_AUDIOMUTE,
        MediaSelect             = SDLK_MEDIASELECT,
        WWW                     = SDLK_WWW,
        Mail                    = SDLK_MAIL,
        Calculator              = SDLK_CALCULATOR,
        Computer                = SDLK_COMPUTER,
        ACSearch                = SDLK_AC_SEARCH,
        ACHome                  = SDLK_AC_HOME,
        ACBack                  = SDLK_AC_BACK,
        ACForward               = SDLK_AC_FORWARD,
        ACStop                  = SDLK_AC_STOP,
        ACRefresh               = SDLK_AC_REFRESH,
        ACBookmarks             = SDLK_AC_BOOKMARKS,

        BrightnessDown          = SDLK_BRIGHTNESSDOWN,
        BrightnessUp            = SDLK_BRIGHTNESSUP,
        DisplaySwitch           = SDLK_DISPLAYSWITCH,
        KBDIllumToggle          = SDLK_KBDILLUMTOGGLE,
        KBDIllumDown            = SDLK_KBDILLUMDOWN,
        KBDIllumUp              = SDLK_KBDILLUMUP,
        Eject                   = SDLK_EJECT,
        Sleep                   = SDLK_SLEEP,
        App1                    = SDLK_APP1,
        App2                    = SDLK_APP2,

        AudioRewind             = SDLK_AUDIOREWIND,
        AudioFastForward        = SDLK_AUDIOFASTFORWARD,

        SoftLeft                = SDLK_SOFTLEFT,
        SoftRight               = SDLK_SOFTRIGHT,
        Call                    = SDLK_CALL,
        EndCall                 = SDLK_ENDCALL
    };

    /**
     * @brief Equality comparison between a KeyCode and an SDL_KeyCode.
     *
     * @param a The KeyCode to compare.
     * @param b The SDL_KeyCode to compare.
     * @return True if the KeyCode is equal to the integer, false otherwise.
     */
    constexpr bool operator==(KeyCode a, SDL_KeyCode b)
    {
        return static_cast<SDL_KeyCode>(a) == b;
    }

    /**
     * @brief Equality comparison between an SDL_KeyCode and a KeyCode.
     *
     * @param a The SDL_KeyCode to compare.
     * @param b The KeyCode to compare.
     * @return True if the integer is equal to the KeyCode, false otherwise.
     */
    constexpr bool operator==(SDL_KeyCode a, KeyCode b)
    {
        return b == a;
    }

    /**
     * @brief Inequality comparison between a KeyCode and an SDL_KeyCode.
     *
     * @param a The KeyCode to compare.
     * @param b The SDL_KeyCode to compare.
     * @return True if the KeyCode is not equal to the integer, false otherwise.
     */
    constexpr bool operator!=(KeyCode a, SDL_KeyCode b)
    {
        return !(a == b);
    }

    /**
     * @brief Inequality comparison between an SDL_KeyCode and a KeyCode.
     *
     * @param a The SDL_KeyCode to compare.
     * @param b The KeyCode to compare.
     * @return True if the integer is not equal to the KeyCode, false otherwise.
     */
    constexpr bool operator!=(SDL_KeyCode a, KeyCode b)
    {
        return !(a == b);
    }

    /**
     * @brief Enumeration of scancodes.
     *
     * This enum defines identifiers for scancodes, which represent keycodes in a platform-independent manner.
     */
    enum class Scancode : Uint16
    {
        Unknown                 = SDL_SCANCODE_UNKNOWN,

        A                       = SDL_SCANCODE_A,
        B                       = SDL_SCANCODE_B,
        C                       = SDL_SCANCODE_C,
        D                       = SDL_SCANCODE_D,
        E                       = SDL_SCANCODE_E,
        F                       = SDL_SCANCODE_F,
        G                       = SDL_SCANCODE_G,
        H                       = SDL_SCANCODE_H,
        I                       = SDL_SCANCODE_I,
        J                       = SDL_SCANCODE_J,
        K                       = SDL_SCANCODE_K,
        L                       = SDL_SCANCODE_L,
        M                       = SDL_SCANCODE_M,
        N                       = SDL_SCANCODE_N,
        O                       = SDL_SCANCODE_O,
        P                       = SDL_SCANCODE_P,
        Q                       = SDL_SCANCODE_Q,
        R                       = SDL_SCANCODE_R,
        S                       = SDL_SCANCODE_S,
        T                       = SDL_SCANCODE_T,
        U                       = SDL_SCANCODE_U,
        V                       = SDL_SCANCODE_V,
        W                       = SDL_SCANCODE_W,
        X                       = SDL_SCANCODE_X,
        Y                       = SDL_SCANCODE_Y,
        Z                       = SDL_SCANCODE_Z,

        One                     = SDL_SCANCODE_1,
        Two                     = SDL_SCANCODE_2,
        Three                   = SDL_SCANCODE_3,
        Four                    = SDL_SCANCODE_4,
        Five                    = SDL_SCANCODE_5,
        Six                     = SDL_SCANCODE_6,
        Seven                   = SDL_SCANCODE_7,
        Eight                   = SDL_SCANCODE_8,
        Nine                    = SDL_SCANCODE_9,
        Zero                    = SDL_SCANCODE_0,

        Return                  = SDL_SCANCODE_RETURN,
        Escape                  = SDL_SCANCODE_ESCAPE,
        Backspace               = SDL_SCANCODE_BACKSPACE,
        Tab                     = SDL_SCANCODE_TAB,
        Space                   = SDL_SCANCODE_SPACE,

        Minus                   = SDL_SCANCODE_MINUS,
        Equals                  = SDL_SCANCODE_EQUALS,
        LeftBracket             = SDL_SCANCODE_LEFTBRACKET,
        RightBracket            = SDL_SCANCODE_RIGHTBRACKET,
        BackSlash               = SDL_SCANCODE_BACKSLASH,
        NonUSHash               = SDL_SCANCODE_NONUSHASH,
        Semicolon               = SDL_SCANCODE_SEMICOLON,
        Apostrophe              = SDL_SCANCODE_APOSTROPHE,
        Grave                   = SDL_SCANCODE_GRAVE,
        Comma                   = SDL_SCANCODE_COMMA,
        Period                  = SDL_SCANCODE_PERIOD,
        Slash                   = SDL_SCANCODE_SLASH,

        CapsLock                = SDL_SCANCODE_CAPSLOCK,

        F1                      = SDL_SCANCODE_F1,
        F2                      = SDL_SCANCODE_F2,
        F3                      = SDL_SCANCODE_F3,
        F4                      = SDL_SCANCODE_F4,
        F5                      = SDL_SCANCODE_F5,
        F6                      = SDL_SCANCODE_F6,
        F7                      = SDL_SCANCODE_F7,
        F8                      = SDL_SCANCODE_F8,
        F9                      = SDL_SCANCODE_F9,
        F10                     = SDL_SCANCODE_F10,
        F11                     = SDL_SCANCODE_F11,
        F12                     = SDL_SCANCODE_F12,

        PrintScreen             = SDL_SCANCODE_PRINTSCREEN,
        ScrollLock              = SDL_SCANCODE_SCROLLLOCK,
        Pause                   = SDL_SCANCODE_PAUSE,
        Insert                  = SDL_SCANCODE_INSERT,
        Home                    = SDL_SCANCODE_HOME,
        PageUp                  = SDL_SCANCODE_PAGEUP,
        Delete                  = SDL_SCANCODE_DELETE,
        End                     = SDL_SCANCODE_END,
        PageDown                = SDL_SCANCODE_PAGEDOWN,
        Right                   = SDL_SCANCODE_RIGHT,
        Left                    = SDL_SCANCODE_LEFT,
        Down                    = SDL_SCANCODE_DOWN,
        Up                      = SDL_SCANCODE_UP,

        NumLockClear            = SDL_SCANCODE_NUMLOCKCLEAR,
        KpDivide                = SDL_SCANCODE_KP_DIVIDE,
        KpMultiply              = SDL_SCANCODE_KP_MULTIPLY,
        KpMinus                 = SDL_SCANCODE_KP_MINUS,
        KpPlus                  = SDL_SCANCODE_KP_PLUS,
        KpEnter                 = SDL_SCANCODE_KP_ENTER,
        KpOne                   = SDL_SCANCODE_KP_1,
        KpTwo                   = SDL_SCANCODE_KP_2,
        KpThree                 = SDL_SCANCODE_KP_3,
        KpFour                  = SDL_SCANCODE_KP_4,
        KpFive                  = SDL_SCANCODE_KP_5,
        KpSix                   = SDL_SCANCODE_KP_6,
        KpSeven                 = SDL_SCANCODE_KP_7,
        KpEight                 = SDL_SCANCODE_KP_8,
        KpNine                  = SDL_SCANCODE_KP_9,
        KpZero                  = SDL_SCANCODE_KP_0,
        KpPeriod                = SDL_SCANCODE_KP_PERIOD,

        NonUSBackSlash          = SDL_SCANCODE_NONUSBACKSLASH,
        Application             = SDL_SCANCODE_APPLICATION,                 ///< windows contextual menu, compose
        Power                   = SDL_SCANCODE_POWER,
        KpEquals                = SDL_SCANCODE_KP_EQUALS,
        F13                     = SDL_SCANCODE_F13,
        F14                     = SDL_SCANCODE_F14,
        F15                     = SDL_SCANCODE_F15,
        F16                     = SDL_SCANCODE_F16,
        F17                     = SDL_SCANCODE_F17,
        F18                     = SDL_SCANCODE_F18,
        F19                     = SDL_SCANCODE_F19,
        F20                     = SDL_SCANCODE_F20,
        F21                     = SDL_SCANCODE_F21,
        F22                     = SDL_SCANCODE_F22,
        F23                     = SDL_SCANCODE_F23,
        F24                     = SDL_SCANCODE_F24,
        Execute                 = SDL_SCANCODE_EXECUTE,
        Help                    = SDL_SCANCODE_HELP,                        ///< AL Integrated Help Center
        Menu                    = SDL_SCANCODE_MENU,                        ///< Menu (show menu)
        Select                  = SDL_SCANCODE_SELECT,
        Stop                    = SDL_SCANCODE_STOP,                        ///< AC Stop
        Again                   = SDL_SCANCODE_AGAIN,                       ///< AC Redo/Repeat
        Undo                    = SDL_SCANCODE_UNDO,                        ///< AC Undo
        Cut                     = SDL_SCANCODE_CUT,                         ///< AC Cut
        Copy                    = SDL_SCANCODE_COPY,                        ///< AC Copy
        Paste                   = SDL_SCANCODE_PASTE,                       ///< AC Paste
        Find                    = SDL_SCANCODE_FIND,                        ///< AC Find
        Mute                    = SDL_SCANCODE_MUTE,
        VolumeUp                = SDL_SCANCODE_VOLUMEUP,
        VolumeDown              = SDL_SCANCODE_VOLUMEDOWN,
        KpComma                 = SDL_SCANCODE_KP_COMMA,
        KpEqualsAs400           = SDL_SCANCODE_KP_EQUALSAS400,

        International1          = SDL_SCANCODE_INTERNATIONAL1,              ///< used on Asian keyboards, see footnotes in USB doc
        International2          = SDL_SCANCODE_INTERNATIONAL2,
        International3          = SDL_SCANCODE_INTERNATIONAL3,              ///< Yen
        International4          = SDL_SCANCODE_INTERNATIONAL4,
        International5          = SDL_SCANCODE_INTERNATIONAL5,
        International6          = SDL_SCANCODE_INTERNATIONAL6,
        International7          = SDL_SCANCODE_INTERNATIONAL7,
        International8          = SDL_SCANCODE_INTERNATIONAL8,
        International9          = SDL_SCANCODE_INTERNATIONAL9,
        Lang1                   = SDL_SCANCODE_LANG1,                       ///< Hangul/English toggle
        Lang2                   = SDL_SCANCODE_LANG2,                       ///< Hanja conversion
        Lang3                   = SDL_SCANCODE_LANG3,                       ///< Katakana
        Lang4                   = SDL_SCANCODE_LANG4,                       ///< Hiragana
        Lang5                   = SDL_SCANCODE_LANG5,                       ///< Zenkaku/Hankaku
        Lang6                   = SDL_SCANCODE_LANG6,                       ///< reserved
        Lang7                   = SDL_SCANCODE_LANG7,                       ///< reserved
        Lang8                   = SDL_SCANCODE_LANG8,                       ///< reserved
        Lang9                   = SDL_SCANCODE_LANG9,                       ///< reserved

        AltErase                = SDL_SCANCODE_ALTERASE,                    ///< Erase-Eaze
        SysReq                  = SDL_SCANCODE_SYSREQ,
        Cancel                  = SDL_SCANCODE_CANCEL,                      ///< AC Cancel
        Clear                   = SDL_SCANCODE_CLEAR,
        Prior                   = SDL_SCANCODE_PRIOR,
        Return2                 = SDL_SCANCODE_RETURN2,
        Separator               = SDL_SCANCODE_SEPARATOR,
        Out                     = SDL_SCANCODE_OUT,
        Oper                    = SDL_SCANCODE_OPER,
        ClearAgain              = SDL_SCANCODE_CLEARAGAIN,
        CRSEL                   = SDL_SCANCODE_CRSEL,
        EXSEL                   = SDL_SCANCODE_EXSEL,

        Kp00                    = SDL_SCANCODE_KP_00,
        Kp000                   = SDL_SCANCODE_KP_000,
        ThousandsSeparator      = SDL_SCANCODE_THOUSANDSSEPARATOR,
        DecimalSeparator        = SDL_SCANCODE_DECIMALSEPARATOR,
        CurrencyUnit            = SDL_SCANCODE_CURRENCYUNIT,
        CurrencySubUnit         = SDL_SCANCODE_CURRENCYSUBUNIT,
        KpLeftParen             = SDL_SCANCODE_KP_LEFTPAREN,
        KpRightParen            = SDL_SCANCODE_KP_RIGHTPAREN,
        KpLeftBrace             = SDL_SCANCODE_KP_LEFTBRACE,
        KpRightBrace            = SDL_SCANCODE_KP_RIGHTBRACE,
        KpTab                   = SDL_SCANCODE_KP_TAB,
        KpBackspace             = SDL_SCANCODE_KP_BACKSPACE,
        KpA                     = SDL_SCANCODE_KP_A,
        KpB                     = SDL_SCANCODE_KP_B,
        KpC                     = SDL_SCANCODE_KP_C,
        KpD                     = SDL_SCANCODE_KP_D,
        KpE                     = SDL_SCANCODE_KP_E,
        KpF                     = SDL_SCANCODE_KP_F,
        KpXOR                   = SDL_SCANCODE_KP_XOR,
        KpPower                 = SDL_SCANCODE_KP_POWER,
        KpPercent               = SDL_SCANCODE_KP_PERCENT,
        KpLess                  = SDL_SCANCODE_KP_LESS,
        KpGreater               = SDL_SCANCODE_KP_GREATER,
        KpAmpersand             = SDL_SCANCODE_KP_AMPERSAND,
        KpDBLAmpersand          = SDL_SCANCODE_KP_DBLAMPERSAND,
        KpVerticalBar           = SDL_SCANCODE_KP_VERTICALBAR,
        KpDBLVerticalBar        = SDL_SCANCODE_KP_DBLVERTICALBAR,
        KpColon                 = SDL_SCANCODE_KP_COLON,
        KpHash                  = SDL_SCANCODE_KP_HASH,
        KpSpace                 = SDL_SCANCODE_KP_SPACE,
        KpAt                    = SDL_SCANCODE_KP_AT,
        KpExclam                = SDL_SCANCODE_KP_EXCLAM,
        KpMemStore              = SDL_SCANCODE_KP_MEMSTORE,
        KpMemRecall             = SDL_SCANCODE_KP_MEMRECALL,
        KpMemClear              = SDL_SCANCODE_KP_MEMCLEAR,
        KpMemAdd                = SDL_SCANCODE_KP_MEMADD,
        KpMemSubtract           = SDL_SCANCODE_KP_MEMSUBTRACT,
        KpMemMultiply           = SDL_SCANCODE_KP_MEMMULTIPLY,
        KpMemDivide             = SDL_SCANCODE_KP_MEMDIVIDE,
        KpPlusMinus             = SDL_SCANCODE_KP_PLUSMINUS,
        KpClear                 = SDL_SCANCODE_KP_CLEAR,
        KpClearEntry            = SDL_SCANCODE_KP_CLEARENTRY,
        KpBinary                = SDL_SCANCODE_KP_BINARY,
        KpOctal                 = SDL_SCANCODE_KP_OCTAL,
        KpDecimal               = SDL_SCANCODE_KP_DECIMAL,
        KpHexadecimal           = SDL_SCANCODE_KP_HEXADECIMAL,

        LCtrl                   = SDL_SCANCODE_LCTRL,
        LShift                  = SDL_SCANCODE_LSHIFT,
        LAlt                    = SDL_SCANCODE_LALT,                        ///< alt, option
        LGui                    = SDL_SCANCODE_LGUI,                        ///< windows, command (apple), meta
        RCtrl                   = SDL_SCANCODE_RCTRL,
        RShift                  = SDL_SCANCODE_RSHIFT,
        RAlt                    = SDL_SCANCODE_RALT,                        ///< alt gr, option
        RGui                    = SDL_SCANCODE_RGUI,                        ///< windows, command (apple), meta

        Mode                    = SDL_SCANCODE_MODE,

        AudioNext               = SDL_SCANCODE_AUDIONEXT,
        AudioPrev               = SDL_SCANCODE_AUDIOPREV,
        AudioStop               = SDL_SCANCODE_AUDIOSTOP,
        AudioPlay               = SDL_SCANCODE_AUDIOPLAY,
        AudioMute               = SDL_SCANCODE_AUDIOMUTE,
        MediaSelect             = SDL_SCANCODE_MEDIASELECT,
        WWW                     = SDL_SCANCODE_WWW,                         ///< AL Internet Browser
        Mail                    = SDL_SCANCODE_MAIL,
        Calculator              = SDL_SCANCODE_CALCULATOR,                  ///< AL Calculator
        Computer                = SDL_SCANCODE_COMPUTER,
        ACSearch                = SDL_SCANCODE_AC_SEARCH,                   ///< AC Search
        ACHome                  = SDL_SCANCODE_AC_HOME,                     ///< AC Home
        ACBack                  = SDL_SCANCODE_AC_BACK,                     ///< AC Back
        ACForward               = SDL_SCANCODE_AC_FORWARD,                  ///< AC Forward
        ACStop                  = SDL_SCANCODE_AC_STOP,                     ///< AC Stop
        ACRefresh               = SDL_SCANCODE_AC_REFRESH,                  ///< AC Refresh
        ACBookmarks             = SDL_SCANCODE_AC_BOOKMARKS,                ///< AC Bookmarks

        BrightnessDown          = SDL_SCANCODE_BRIGHTNESSDOWN,
        BrightnessUp            = SDL_SCANCODE_BRIGHTNESSUP,
        DisplaySwitch           = SDL_SCANCODE_DISPLAYSWITCH,               ///< display mirroring/dual display switch, video mode switch
        KBDIllumToggle          = SDL_SCANCODE_KBDILLUMTOGGLE,
        KBDIllumDown            = SDL_SCANCODE_KBDILLUMDOWN,
        KBDIllumUp              = SDL_SCANCODE_KBDILLUMUP,
        Eject                   = SDL_SCANCODE_EJECT,
        Sleep                   = SDL_SCANCODE_SLEEP,                       ///< SC System Sleep

        App1                    = SDL_SCANCODE_APP1,
        App2                    = SDL_SCANCODE_APP2,

        AudioRewind             = SDL_SCANCODE_AUDIOREWIND,
        AudioFastForward        = SDL_SCANCODE_AUDIOFASTFORWARD,

        SoftLeft                = SDL_SCANCODE_SOFTLEFT,
        SoftRight               = SDL_SCANCODE_SOFTRIGHT,
        Call                    = SDL_SCANCODE_CALL,                        ///< Used for accepting phone calls.
        EndCall                 = SDL_SCANCODE_ENDCALL,                     ///< Used for rejecting phone calls.
    };

    /**
     * @brief Equality comparison between a Scancode and an SDL_Scancode.
     *
     * @param a The Scancode to compare.
     * @param b The SDL_Scancode to compare.
     * @return True if the Scancode is equal to the integer, false otherwise.
     */
    constexpr bool operator==(Scancode a, SDL_Scancode b)
    {
        return static_cast<SDL_Scancode>(a) == b;
    }

    /**
     * @brief Equality comparison between an SDL_Scancode and a Scancode.
     *
     * @param a The SDL_Scancode to compare.
     * @param b The Scancode to compare.
     * @return True if the integer is equal to the Scancode, false otherwise.
     */
    constexpr bool operator==(SDL_Scancode a, Scancode b)
    {
        return b == a;
    }

    /**
     * @brief Inequality comparison between a Scancode and an SDL_Scancode.
     *
     * @param a The Scancode to compare.
     * @param b The SDL_Scancode to compare.
     * @return True if the Scancode is not equal to the integer, false otherwise.
     */
    constexpr bool operator!=(Scancode a, SDL_Scancode b)
    {
        return !(a == b);
    }

    /**
     * @brief Inequality comparison between an SDL_Scancode and a Scancode.
     *
     * @param a The SDL_Scancode to compare.
     * @param b The Scancode to compare.
     * @return True if the integer is not equal to the Scancode, false otherwise.
     */
    constexpr bool operator!=(SDL_Scancode a, Scancode b)
    {
        return !(a == b);
    }

    /**
     * @brief Enumeration of key modifiers.
     *
     * This enum defines identifiers for key modifiers such as Shift, Ctrl, Alt, Num Lock, Caps Lock, and Scroll Lock.
     */
    enum class KeyModifier : Uint16
    {
        None                    = KMOD_NONE,
        LShift                  = KMOD_LSHIFT,
        RShift                  = KMOD_RSHIFT,
        LCtrl                   = KMOD_LCTRL,
        RCtrl                   = KMOD_RCTRL,
        LAlt                    = KMOD_LALT,
        RAlt                    = KMOD_RALT,
        LGui                    = KMOD_LGUI,
        RGui                    = KMOD_RGUI,
        Num                     = KMOD_NUM,
        Caps                    = KMOD_CAPS,
        Mode                    = KMOD_MODE,
        Scroll                  = KMOD_SCROLL,

        Ctrl                    = KMOD_CTRL,
        Shift                   = KMOD_SHIFT,
        Alt                     = KMOD_ALT,
        Gui                     = KMOD_GUI,
    };

    /**
     * @brief Equality comparison between a KeyModifier and an SDL_Keymod.
     *
     * @param a The KeyModifier to compare.
     * @param b The SDL_Keymod to compare.
     * @return True if the KeyModifier is equal to the integer, false otherwise.
     */
    constexpr bool operator==(KeyModifier a, SDL_Keymod b)
    {
        return static_cast<SDL_Keymod>(a) == b;
    }

    /**
     * @brief Equality comparison between an SDL_Keymod and a KeyModifier.
     *
     * @param a The SDL_Keymod to compare.
     * @param b The KeyModifier to compare.
     * @return True if the integer is equal to the KeyModifier, false otherwise.
     */
    constexpr bool operator==(SDL_Keymod a, KeyModifier b)
    {
        return b == a;
    }

    /**
     * @brief Inequality comparison between a KeyModifier and an SDL_Keymod.
     *
     * @param a The KeyModifier to compare.
     * @param b The SDL_Keymod to compare.
     * @return True if the KeyModifier is not equal to the integer, false otherwise.
     */
    constexpr bool operator!=(KeyModifier a, SDL_Keymod b)
    {
        return !(a == b);
    }

    /**
     * @brief Inequality comparison between an SDL_Keymod and a KeyModifier.
     *
     * @param a The SDL_Keymod to compare.
     * @param b The KeyModifier to compare.
     * @return True if the integer is not equal to the KeyModifier, false otherwise.
     */
    constexpr bool operator!=(SDL_Keymod a, KeyModifier b)
    {
        return !(a == b);
    }

    /**
     * @brief Get a snapshot of the current state of the keyboard.
     *
     * This function retrieves a snapshot of the current state of the keyboard as an array of scancodes.
     * The array contains a 1 if a key is pressed and 0 if it is not. The indexes into this array correspond to SDL_Scancode values.
     * The pointer returned is a pointer to an internal SDL array, and it remains valid for the entire lifetime of the application.
     * The caller should not free the memory associated with this pointer.
     *
     * @param lenArray An optional pointer to store the length of the state array. If provided, the length will be written to this pointer.
     * @return A pointer to the keyboard state array.
     */
    inline const Scancode* GetKeyboardState(int* lenArray = nullptr)
    {
        return reinterpret_cast<const Scancode*>(SDL_GetKeyboardState(lenArray));
    }

    /**
     * @brief Check if a specific key is currently pressed.
     *
     * This function checks if the specified scancode key is currently pressed.
     *
     * @param key The scancode of the key to check.
     * @return `true` if the key is pressed; otherwise, `false`.
     */
    inline bool IsKeyPressed(Scancode key)
    {
        const Uint8* keystate = SDL_GetKeyboardState(nullptr);
        return keystate[static_cast<Uint16>(key)] == 1;
    }

    /**
     * @brief Check if a specific key modifier is currently pressed.
     *
     * This function checks if the specified key modifier (Shift, Ctrl, Alt, Num Lock, Caps Lock, Scroll Lock) is currently pressed.
     *
     * @param modifier The key modifier to check.
     * @return `true` if the modifier key is pressed; otherwise, `false`.
     */
    inline bool IsModifierKeyPressed(KeyModifier modifier)
    {
        return (SDL_GetModState() & static_cast<Uint16>(modifier)) != 0;
    }

    /**
     * @brief Check if the Shift key is currently pressed.
     *
     * This function checks if the Shift key is currently pressed.
     *
     * @return `true` if the Shift key is pressed; otherwise, `false`.
     */
    inline bool IsShiftPressed()
    {
        return IsModifierKeyPressed(KeyModifier::Shift);
    }

    /**
     * @brief Check if the Ctrl key is currently pressed.
     *
     * This function checks if the Ctrl key is currently pressed.
     *
     * @return `true` if the Ctrl key is pressed; otherwise, `false`.
     */
    inline bool IsCtrlPressed()
    {
        return IsModifierKeyPressed(KeyModifier::Ctrl);
    }

    /**
     * @brief Check if the Alt key is currently pressed.
     *
     * This function checks if the Alt key is currently pressed.
     *
     * @return `true` if the Alt key is pressed; otherwise, `false`.
     */
    inline bool IsAltPressed()
    {
        return IsModifierKeyPressed(KeyModifier::Alt);
    }

    /**
     * @brief Check if Num Lock is currently on.
     *
     * This function checks if Num Lock is currently activated.
     *
     * @return `true` if Num Lock is on; otherwise, `false`.
     */
    inline bool IsNumLockOn()
    {
        return IsModifierKeyPressed(KeyModifier::Num);
    }

    /**
     * @brief Check if Caps Lock is currently on.
     *
     * This function checks if Caps Lock is currently activated.
     *
     * @return `true` if Caps Lock is on; otherwise, `false`.
     */
    inline bool IsCapsLockOn()
    {
        return IsModifierKeyPressed(KeyModifier::Caps);
    }

    /**
     * @brief Check if Scroll Lock is currently on.
     *
     * This function checks if Scroll Lock is currently activated.
     *
     * @return `true` if Scroll Lock is on; otherwise, `false`.
     */
    inline bool IsScrollLockOn()
    {
        return IsModifierKeyPressed(KeyModifier::Scroll);
    }

}}

#endif //NEXUS_INPUTS_KEYBOARD_HPP
