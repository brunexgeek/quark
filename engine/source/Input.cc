/*
 * Copyright (C) 2014 Benny Bobaganoosh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <engine/Input.hh>
#include <SDL2/SDL.h>
#include <cstring>
#include <iostream>


Input::Input()
{
	//memset(m_inputs, 0, NUM_KEYS * sizeof(bool));
	//memset(m_downKeys, 0, NUM_KEYS * sizeof(bool));
	//memset(m_upKeys, 0, NUM_KEYS * sizeof(bool));
	memset(keyboard, 0, NUM_KEYS * sizeof(uint8_t));
	memset(mouseButton, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	mousePosition.x = mousePosition.y = 0;
	mouseDelta.x = mouseDelta.y = 0;

	SDL_ShowCursor(SDL_ENABLE);
}


Input::~Input()
{
	SDL_ShowCursor(SDL_ENABLE);
	SDL_CaptureMouse(SDL_TRUE);
}


void Input::grabCursor(bool enabled)
{
	if(enabled)
	{
		//SDL_ShowCursor(SDL_DISABLE);
		//if (SDL_CaptureMouse(SDL_TRUE) < 0) std::cout << "Mouse grab unsupported!\n";
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		//SDL_ShowCursor(SDL_ENABLE);
		//SDL_CaptureMouse(SDL_FALSE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}


bool Input::isCursorGrabbed() const
{
	return SDL_GetRelativeMouseMode() != 0;
}

void Input::setMousePosition(const Vector2f& pos) const
{
	//SDL_WarpMouseInWindow(m_window->GetSDLWindow(), (int)pos.GetX(), (int)pos.GetY());
//	SDLSetMousePosition((int)pos.GetX(), (int)pos.GetY());
}


void Input::update()
{
	mouseDelta.x = mouseDelta.y = 0;

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		/*if(e.type == SDL_QUIT)
		{
			m_isCloseRequested = true;
		}*/

		if(e.type == SDL_MOUSEMOTION)
		{
			setMouseX(e.motion.x, e.motion.xrel);
			setMouseY(e.motion.y, e.motion.yrel);
		}

		if(e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;

			setKey(value, true);
		}
		if(e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;

			setKey(value, false);
		}
		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;

			setMouseButton(value, true);
		}
		if(e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;

			setMouseButton(value, false);
		}
	}
}