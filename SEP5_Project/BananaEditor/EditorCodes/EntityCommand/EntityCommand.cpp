/******************************************************************************/
/*!
\file       UndoSystem.cpp
\par        Project:
\author     Pham Thao Mien
\date       12 November 2021
\brief      This header file contains the handling of changing of scene state
			when the tool bar functions are being clicked.

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "EntityCommand.h"
#include "Hierarchy_Window/Hierarchy_Window.h"

namespace BananaEditor
{
	void CreateEntityCommand::execute()
	{
		newEntity = BE::ECS->CreateEntity();
	}

	void CreateEntityCommand::undo()
	{
		Hierarchy_Window::isusing = false;
		BE::ECS->DestroyEntity(newEntity);
	}

	void CloneEntityCommand::setCloneID(BE::EntityID id)
	{
		CloneID = id;
	}

	void CloneEntityCommand::execute()
	{
		newEntity = BE::ECS->CloneEntity(CloneID);
	}

	void CloneEntityCommand::undo()
	{
		Hierarchy_Window::isusing = false;
		BE::ECS->DestroyEntity(newEntity);
	}
}
