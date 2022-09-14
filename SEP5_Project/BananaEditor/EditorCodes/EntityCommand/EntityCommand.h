/******************************************************************************/
/*!
\file       UndoSystem.h
\par        Project: 
\author     Pham Thao Mien
\date       12 November 2021
\brief      

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef ENTITY_COMMAND_H
#define ENTITY_COMMAND_H

#include "../UndoSystem/UndoSystem.h"

namespace BananaEditor
{
	class CreateEntityCommand : public ICommand
	{
		BE::EntityID newEntity;
	public:
		void execute() override;
		void undo() override;
	};

	class CloneEntityCommand : public ICommand
	{
		BE::EntityID CloneID;
		BE::EntityID newEntity;
	public:
		void setCloneID(BE::EntityID);
		void execute() override;
		void undo() override;
	};

	template<class T>
	class AddComponentCommand : public ICommand
	{
		BE::EntityID entityID;
		//T component;
	public:
		
		void setID(BE::EntityID id)
		{
			entityID = id;
		}

		void execute() override
		{
			BE::ECS->AddComponent(entityID, T{});
		}
	
		void undo() override
		{
			BE::ECS->RemoveComponent<T>(entityID);
		}
	};
};
#endif