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

Input::Input() :
	m_mouseX(0),
	m_mouseY(0)
{
	memset(m_inputs, 0, NUM_KEYS * sizeof(bool));
	memset(m_downKeys, 0, NUM_KEYS * sizeof(bool));
	memset(m_upKeys, 0, NUM_KEYS * sizeof(bool));

	memset(m_mouseInput, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	memset(m_downMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	memset(m_upMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	SDL_ShowCursor(SDL_ENABLE);
}

void Input::grabCursor(bool enabled)
{
	if(enabled)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_ENABLE);
}


bool Input::isCursorGrabbed() const
{
	return SDL_ShowCursor(SDL_QUERY) != 0;
}

void Input::setMousePosition(const glm::vec2& pos) const
{
	//SDL_WarpMouseInWindow(m_window->GetSDLWindow(), (int)pos.GetX(), (int)pos.GetY());
//	SDLSetMousePosition((int)pos.GetX(), (int)pos.GetY());
}


void Input::update()
{
	/*for(int i = 0; i < Input::NUM_MOUSEBUTTONS; i++)
	{
		setMouseDown(i, false);
		setMouseUp(i, false);
	}

	for(int i = 0; i < Input::NUM_KEYS; i++)
	{
		setKeyDown(i, false);
		setKeyUp(i, false);
	}*/
    memset(m_inputs, 0, sizeof(m_inputs));
    memset(m_downKeys, 0, sizeof(m_downKeys));
    memset(m_upKeys, 0, sizeof(m_upKeys));
    memset(m_mouseInput, 0, sizeof(m_mouseInput));
    memset(m_downMouse, 0, sizeof(m_downMouse));
    memset(m_upMouse, 0, sizeof(m_upMouse));

	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		/*if(e.type == SDL_QUIT)
		{
			m_isCloseRequested = true;
		}*/

		if(e.type == SDL_MOUSEMOTION)
		{
			setMouseX(e.motion.x);
			setMouseY(e.motion.y);
		}

		if(e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;

			setKey(value, true);
			setKeyDown(value, true);
		}
		if(e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;

			setKey(value, false);
			setKeyUp(value, true);
		}
		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;

			setMouse(value, true);
			setMouseDown(value, true);
		}
		if(e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;

			setMouse(value, false);
			setMouseUp(value, true);
		}
	}
}