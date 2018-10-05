#include <engine/InputChannel.hh>
#include <SDL2/SDL.h>
#include <iostream>


namespace quark {

SDLChannel::SDLChannel()
{
    SDL_InitSubSystem(SDL_INIT_EVENTS);
    SDL_ShowCursor(SDL_ENABLE);
}


SDLChannel::~SDLChannel()
{
    SDL_ShowCursor(SDL_ENABLE);
	SDL_CaptureMouse(SDL_TRUE);
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}


bool SDLChannel::poolEvent(
    InputEvent &event )
{
	SDL_Event ee;
	if (SDL_PollEvent(&ee))
	{
		if (ee.type == SDL_MOUSEMOTION)
		{
            event.mouse.motion.type = QUARK_MOUSEMOTION;
            event.mouse.motion.which = ee.motion.which;
            event.mouse.motion.state = ee.motion.state;
            event.mouse.motion.x = ee.motion.x;
            event.mouse.motion.y = ee.motion.y;
            event.mouse.motion.xrel = ee.motion.xrel;
            event.mouse.motion.yrel = ee.motion.yrel;
		}
		else
		if (ee.type == SDL_MOUSEBUTTONDOWN || ee.type == SDL_MOUSEBUTTONUP)
        {
            if (ee.type == SDL_MOUSEBUTTONDOWN)
                event.mouse.button.type = QUARK_MOUSEBUTTONDOWN;
            else
                event.mouse.button.type = QUARK_MOUSEBUTTONUP;
            event.mouse.button.which = ee.button.which;
            event.mouse.button.clicks = ee.button.clicks;
            event.mouse.button.x = ee.button.x;
            event.mouse.button.y = ee.button.y;
        }
		else
		if (ee.type == SDL_KEYDOWN || ee.type == SDL_KEYUP)
		{
            if (ee.type == SDL_KEYDOWN)
                event.keyboard.type = QUARK_KEYDOWN;
            else
                event.keyboard.type = QUARK_KEYUP;
            event.keyboard.repeat = ee.key.repeat != 0;
            event.keyboard.key.scancode = ee.key.keysym.scancode;
            event.keyboard.key.modifier = ee.key.keysym.mod;
        }
        return true;
	}

    return false;
}


void SDLChannel::grabCursor(bool enabled)
{
	SDL_SetRelativeMouseMode( (enabled) ? SDL_TRUE : SDL_FALSE );
}


bool SDLChannel::isCursorGrabbed() const
{
	return SDL_GetRelativeMouseMode() != 0;
}

}