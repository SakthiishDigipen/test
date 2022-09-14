#pragma once
#ifndef	APPLICATION_H
#define APPLICATION_H

#include <BananaEngine.h>
#include <BananaEditor.h>

class Application
{
	BE::Singleton<BE::GameEngine>::SingletonPtr& m_engine;
#ifdef USE_EDITOR
	BE::Singleton<BananaEditor::EditorSystem>::SingletonPtr& m_editor;
#endif

public:
	Application();
	~Application();

	void Init();
	void Run();
	void Shutdown();
};




#endif