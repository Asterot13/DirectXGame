#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	if(::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// Key is down
			if (m_keys_state[i] & 0x80)
			{
				std::map<InputListener*, InputListener*>::iterator it = m_map_listener.begin();

				while (it != m_map_listener.end())
				{
					it->second->onKeyDown(i);
					++it;
				}
			}
			else // Key is down
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::map<InputListener*, InputListener*>::iterator it = m_map_listener.begin();

					while (it != m_map_listener.end())
					{
						it->second->onKeyUp(i);
						++it;
					}
				}
			}
		}
		// store current keys state to old keys state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_map_listener.insert(std::make_pair<InputListener*, InputListener*>
		(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener* listener)
{
	std::map<InputListener*, InputListener*>::iterator it = m_map_listener.find(listener);

	if (it != m_map_listener.end())
	{
		m_map_listener.erase(it);
	}
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
