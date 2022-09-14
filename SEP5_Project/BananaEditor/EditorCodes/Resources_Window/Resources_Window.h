/******************************************************************************/
/*!
\file		Resources_Window.h
\par        Project:
\author		Pham Thao Mien
\date   	October 7, 2021
\brief		Get all game resources into ImGui

\copyright	Copyright (C) 2021 DigiPen Institute of Technology.
			Reproduction or disclosure of this file or its contents without the
			prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#ifndef RESOURCES_WINDOW_H
#define RESOURCES_WINDOW_H

#include "../ImGuiWindow/ImGuiWindow.h"

namespace BananaEditor
{
	class Resources_Window : public ImGuiWindow
	{
	private:
		static Resources_Window* p_resource;

		std::filesystem::path m_currentDir;
		static std::string filename;
		static std::string dragdrop_path;
	public:	
		Resources_Window();
		~Resources_Window();

		static const std::string& getFilename() { return filename; }
		static const std::string getdragdrop_path() { return dragdrop_path; }

		static inline Resources_Window* Get() { return p_resource; }

		Resources_Window* init();
		void update() override;
		void Shutdown() override;
	};
};

#endif