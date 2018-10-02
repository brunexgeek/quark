#ifndef ENGINE_INPUTCHANNEL_HH
#define ENGINE_INPUTCHANNEL_HH


#include <stdint.h>


#define QUARK_

struct KeyInfo
{
    uint16_t scancode;
    uint16_t modifier;
};

struct KeyboardEvent
{
    uint32_t type;        /// QUARK_KEYDOWN or QUARK_KEYUP
    KeyInfo key;         /// The key that was pressed or released
    bool repeat;          /// Non-zero if this is a key repeat
};

struct MouseMotionEvent
{
    uint32_t type;       /// QUARK_MOUSEMOTION
    uint32_t which;      /// The mouse instance id
    uint32_t state;      /// The current button state
    int32_t x;           /// X coordinate, relative to window
    int32_t y;           /// Y coordinate, relative to window
    int32_t xrel;        /// The relative motion in the X direction
    int32_t yrel;        /// The relative motion in the Y direction
};

struct MouseButtonEvent
{
    uint32_t type;        /// QUARK_MOUSEBUTTONDOWN or QUARK_MOUSEBUTTONUP
    uint32_t which;       /// The mouse instance id
    uint8_t button;       /// The mouse button index
    uint8_t clicks;       /// 1 for single-click, 2 for double-click, etc.
    int32_t x;            /// X coordinate, relative to window
    int32_t y;            /// Y coordinate, relative to window
};

struct MouseWheelEvent
{
    uint32_t type;        /// QUARK_MOUSEWHEEL
    uint32_t which;       /// The mouse instance id
    int32_t x;            /// The amount scrolled horizontally, positive to the right and negative to the left
    int32_t y;            /// The amount scrolled vertically, positive away from the user and negative toward the user
    uint32_t direction;   /// Set to one of the SDL_MOUSEWHEEL_* defines. When FLIPPED the values in X and Y will be opposite. Multiply by -1 to change them back
};

union MouseEvent
{
    MouseButtonEvent button;
    MouseMotionEvent motion;
    MouseWheelEvent wheel;
};

union InputEvent
{
    uint32_t type;
    KeyboardEvent keyboard;
    MouseEvent mouse;
};

enum EventType
{
    QUARK_INPUTEVENT = 0,

    QUARK_MOUSEBUTTONDOWN = 0x20,
    QUARK_MOUSEBUTTONUP,
    QUARK_MOUSEMOTION,
    QUARK_MOUSEWHEEL,

    QUARK_KEYDOWN = 0x40,
    QUARK_KEYUP
};


class InputChannel
{
    public:
        ~InputChannel() {}
        virtual bool poolEvent( InputEvent &event ) = 0;
        virtual void grabCursor(bool value) = 0;
		virtual bool isCursorGrabbed() const = 0;
};


/**
 * Input channel that pump events from SDL.
 */
class SDLChannel : public InputChannel
{
    public:
        SDLChannel();
        ~SDLChannel();
        bool poolEvent( InputEvent &event );
        void grabCursor(bool value);
		bool isCursorGrabbed() const;
};

#endif // ENGINE_INPUTCHANNEL_HH