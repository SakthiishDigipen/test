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
#include "UndoSystem.h"

namespace BananaEditor
{
	void CommandManager::executeCmd(std::shared_ptr<ICommand> command)
	{
		//mRedoStack = commandStack_t(); // clear the redo stack
		command->execute();
		mUndoStack.push(command);
	}

	void CommandManager::undo()
	{
		if (mUndoStack.size() <= 0)
		{
			return;
		}
		mUndoStack.top()->undo();          // undo most recently executed command
		//mRedoStack.push(mUndoStack.top()); // add undone command to undo stack
		mUndoStack.pop();                  // remove top entry from undo stack
	}
}
