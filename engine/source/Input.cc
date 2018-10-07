#include <engine/Input.hh>
#include <cstring>
#include <iostream>


namespace quark {

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


uint16_t Input::getModifiers() const
{
	uint16_t modifiers = 0;
	if (keyboard[Input::KEY_LSHIFT] != 0)       modifiers |= Input::MOD_LSHIFT;
	if (keyboard[Input::KEY_RSHIFT] != 0)       modifiers |= Input::MOD_RSHIFT;
	if (keyboard[Input::KEY_LCTRL] != 0)        modifiers |= Input::MOD_LCTRL;
	if (keyboard[Input::KEY_RCTRL] != 0)        modifiers |= Input::MOD_RCTRL;
	if (keyboard[Input::KEY_LALT] != 0)         modifiers |= Input::MOD_LALT;
	if (keyboard[Input::KEY_RALT] != 0)         modifiers |= Input::MOD_RALT;
	if (keyboard[Input::KEY_LGUI] != 0)         modifiers |= Input::MOD_LGUI;
	if (keyboard[Input::KEY_RGUI] != 0)         modifiers |= Input::MOD_RGUI;
	if (keyboard[Input::KEY_NUMLOCKCLEAR] != 0) modifiers |= Input::MOD_NUM;
	if (keyboard[Input::KEY_CAPSLOCK] != 0)     modifiers |= Input::MOD_CAPS;
	if (keyboard[Input::KEY_MODE] != 0)         modifiers |= Input::MOD_MODE;
	return modifiers;
}

/*
void Input::setKey(uint16_t code, bool state, uint16_t modifiers)
{
	if (code >= NUM_KEYS) return;
	if (!state)
	{
		keyboard[code] = 0;
	}
	else
	{
		if (modifiers != 0)
			keyboard[code] = (uint16_t) (1 | modifiers);
		else
			keyboard[code] |= 1;
	}
	std::cout << "Key: " << code << " is " << modifiers << std::endl;
}*/


}