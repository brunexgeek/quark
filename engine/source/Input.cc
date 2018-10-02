#include <engine/Input.hh>
#include <cstring>
#include <iostream>


Input::Input()
{
	memset(keyboard, 0, NUM_KEYS * sizeof(uint8_t));
	memset(mouseButton, 0, NUM_MOUSEBUTTONS * sizeof(bool));

	channel = new SDLChannel();
}


Input::~Input()
{
}


void Input::grabCursor(bool enabled)
{
	channel->grabCursor(enabled);
}


bool Input::isCursorGrabbed() const
{
	return channel->isCursorGrabbed();
}


void Input::update()
{
	mouseDelta.x = mouseDelta.y = 0;

	InputEvent event;
	while (channel->poolEvent(event))
	{
		if(event.type == QUARK_MOUSEMOTION)
		{
			setMouseX(event.mouse.motion.x, event.mouse.motion.xrel);
			setMouseY(event.mouse.motion.y, event.mouse.motion.yrel);
		}
		else
		if (event.type == QUARK_MOUSEBUTTONDOWN)
			setMouseButton(event.mouse.button.button, true);
		else
		if (event.type == QUARK_MOUSEBUTTONUP)
			setMouseButton(event.mouse.button.button, false);
		else
		if(event.type == QUARK_KEYDOWN)
			setKey(event.keyboard.key.scancode, true);
		else
		if(event.type == QUARK_KEYUP)
			setKey(event.keyboard.key.scancode, false);
	}
}