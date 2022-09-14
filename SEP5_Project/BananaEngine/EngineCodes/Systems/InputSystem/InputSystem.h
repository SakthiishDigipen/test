#pragma once
#ifndef	INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include <Utilities/Singleton/Singleton.h>
#include "../ISystem.h"

namespace BE
{
	class InputSystem : public Singleton<InputSystem>, public ISystem
	{
		InputSystem();
		~InputSystem();
	};
}

#define InputSystem InputSystem::GetInstance()
#endif