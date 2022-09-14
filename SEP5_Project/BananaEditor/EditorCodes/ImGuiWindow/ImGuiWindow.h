/******************************************************************************/
/*!
\file		ImguiWindow.h
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2021
\brief		Interface for all ImGui windows

\copyright	Copyright (C) 2022 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef IMGUI_WINDOW_H
#define IMGUI_WINDOW_H

#include <BananaEngine.h>
#include "../../ImguiStuff/IconFont/IconsFontAwesome5.h"

namespace BananaEditor
{
	class ImGuiWindow
	{
	public:
		ImGuiWindow() {};
		virtual ~ImGuiWindow() {};
		virtual void update() {};
		virtual void begin(std::string name);
		virtual bool begin(std::string name, bool* p_open);
		virtual void end();
		virtual void Shutdown() {};
	};
};

#endif