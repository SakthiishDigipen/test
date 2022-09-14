#include "Application.h"

Application::Application() : m_engine{ BE::GameEngine::CreateInstance() }, m_editor{ BananaEditor::EditorSystem::CreateInstance() }
{

}

Application::~Application()
{

}

void Application::Init()
{
	m_engine->Init();

#if 0	//Change to use editor
	m_editor->Init();
#endif
}

void Application::Run()
{
	while (!BE::SceneMgr->GetSceneMgrFlag(BE::SceneMgrFlag::isQuiting))
	{
		m_engine->Update();
		m_editor->Update();
	}
}

void Application::Shutdown()
{
	m_engine->Shutdown();

#if 0	//Change to use editor
	m_editor->Shutdown();
#endif
}