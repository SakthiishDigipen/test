#include "pch.h"
#include "SceneManager.h"

namespace BE
{
	SceneManager::SceneManager()
	{
		//Init the values
		m_currScene = NULL_SCENE_ID;
		m_nextScene = NULL_SCENE_ID;
		m_flags = 0;

		//Add null scene and restart scene into the scene list 
		m_sceneList.emplace(NULL_SCENE_ID, NULL_SCENE);
		m_sceneList.emplace(LOADING_SCENE_ID, LOADING_SCENE);
		m_sceneIDList.emplace(NULL_SCENE.GetSceneName(), NULL_SCENE_ID);
		m_sceneIDList.emplace(LOADING_SCENE.GetSceneName(), LOADING_SCENE_ID);

		//Subscribing to events

		//
		EventMgr->Subscribe(SceneLoadedEvent, this, SceneManager::OnSceneLoaded);

		LogCore("Created Scene Manager");
	}

	SceneManager::~SceneManager()
	{
		LogCore("Destroyed Scene Manager");
	}



	//Creates a new scene given the scene constructor parameters. SceneID 0 is reserved for invalid.
	Scene& SceneManager::CreateScene(std::string&& _name, std::string&& _file, SceneID _id)
	{
		//Cannot be 0 or 1
		if (_id == NULL_SCENE_ID || _id == LOADING_SCENE_ID)
		{
			LogCoreError("Cannot create scene with ID %llu or %llu, they are reserved.", NULL_SCENE_ID, LOADING_SCENE_ID);
			return m_sceneList.at(_id);
		}

		//Cannot create scene with "Invalid" or "Restart" as the name
		if (_name == NULL_SCENE.GetSceneName() || _name == LOADING_SCENE.GetSceneName())
		{
			LogCoreError("Cannot create scene named %s or %s, they are reserved.", \
						 NULL_SCENE.GetSceneName().c_str(), LOADING_SCENE.GetSceneName().c_str());

			return m_sceneList.at(m_sceneIDList.at(_name));
		}

		//Otherwise just create the scene or overwrite the values if it exists
		m_sceneList.emplace(_id, Scene(std::move(_name), std::move(_file), _id));
		m_sceneIDList.emplace(m_sceneList.at(_id).GetSceneName(), _id);

		return m_sceneList.at(_id);
	}

	//Creates a new scene by putting in the scene object itself. SceneID 0 is reserved for invalid.
	Scene& SceneManager::CreateScene(Scene&& _scene)
	{
		//Cannot be 0 or 1
		if (_scene.GetSceneID() == NULL_SCENE_ID || _scene.GetSceneID() == LOADING_SCENE_ID)
		{
			LogCoreError("Cannot create scene with ID %llu or %llu, they are reserved.", NULL_SCENE_ID, LOADING_SCENE_ID);
			return m_sceneList.at(_scene.GetSceneID());
		}

		//Cannot create scenne with "Invalid" or "Restart" as the name
		if (_scene.GetSceneName() == NULL_SCENE.GetSceneName() || _scene.GetSceneName() == LOADING_SCENE.GetSceneName())
		{
			LogCoreError("Cannot create scene named %s or %s, they are reserved.", \
				NULL_SCENE.GetSceneName().c_str(), LOADING_SCENE.GetSceneName().c_str());

			return m_sceneList.at(m_sceneIDList.at(_scene.GetSceneName()));
		}

		//Otherwise just create the scene or overwrite the values if it exists
		auto result = m_sceneIDList.emplace(_scene.GetSceneName(), _scene.GetSceneID());
		m_sceneList.emplace(_scene.GetSceneID(), std::move(_scene));
		return m_sceneList.at((*result.first).second);
	}

	//Removes a scene from the scene list, by Scene ID
	void SceneManager::DestroyScene(SceneID _id)
	{
		//Cannot destroy scene 0 or 1
		if (_id == NULL_SCENE_ID || _id == LOADING_SCENE_ID)
		{
			LogCoreError("Cannot destroy scene with ID %llu or %llu, they are reserved.", NULL_SCENE_ID, LOADING_SCENE_ID);
			return;
		}

		//Cannot destroy scene if it is currently the active or next scene
		if (_id == m_currScene || _id == m_nextScene)
		{
			LogCoreError("Cannot destroy a scene that is currently running or getting loaded next");
			return;
		}

		//If scene ID doesn't exist, then don't do anything
		if (m_sceneList.count(_id) == 0)
		{
			LogCoreError("Scene ID %llu does not exist", _id);
			return;
		}

		m_sceneIDList.erase(m_sceneList.at(_id).GetSceneName());
		m_sceneList.erase(_id);
	}

	//Removes a scene from the scene list, by scene name
	void SceneManager::DestroyScene(const std::string& _sceneName)
	{
		//If Scene doesn't exist, then don't do anything
		if (m_sceneIDList.count(_sceneName) == 0)
		{
			LogCoreError("Scene %s does not exist", _sceneName.c_str());
			return;
		}

		DestroyScene(m_sceneIDList.at(_sceneName));
	}

	//Set which file to read during loading scene
	Scene& SceneManager::SetLoadingScene(std::string&& _file)
	{
		m_sceneList[LOADING_SCENE_ID].SetSceneFile(std::move(_file));
		return m_sceneList.at(LOADING_SCENE_ID);
	}

	//Create all the scenes from a given file, use CreateScene inside to update the scene list
	void SceneManager::SerialiseAllScenes(const std::string& _file)
	{
		UNUSED(_file);
	}

	//Save all the scenes into a given file
	void SceneManager::DeserialiseAllScenes(const std::string& _file)
	{
		UNUSED(_file);
	}

	//Change to another scene by ID, cannot set to 0 or 1
	void SceneManager::LoadScene(SceneID _id)
	{
		//Cannot set to 0 or 1
		if (_id == NULL_SCENE_ID || _id == LOADING_SCENE_ID)
		{
			LogCoreError("Cannot change to scenes with ID %llu or %llu, they are reserved.", NULL_SCENE_ID, LOADING_SCENE_ID);
			return;
		}

		//If scene doesn't exist, then don't change
		if (m_sceneList.count(_id) == 0)
		{
			LogCoreError("Scene ID %llu was not loaded", _id);
			return;
		}

		m_nextScene = _id;
	}

	//Change to another scene by name
	void SceneManager::LoadScene(const std::string& _sceneName)
	{
		//Cannot change to scene scene with "Invalid" or "Restart" as the name
		if (_sceneName == NULL_SCENE.GetSceneName() || _sceneName == LOADING_SCENE.GetSceneName())
		{
			LogCoreError("Cannot change to scenes named %s or %s, they are reserved.", \
				NULL_SCENE.GetSceneName().c_str(), LOADING_SCENE.GetSceneName().c_str());

			return;
		}

		//If scene doesn't exist, then don't change
		if (m_sceneIDList.count(_sceneName) == 0)
		{
			LogCoreError("Scene named %s was not loaded", _sceneName.c_str());
			return;
		}

		m_nextScene = m_sceneIDList.at(_sceneName);
	}

	//Set the flag to restart scene
	void SceneManager::RestartScene()
	{
		m_flags.set((size_t)SceneMgrFlag::isRestart);
	}

	//Set the flag to quit application
	void SceneManager::QuitScene()
	{
		m_flags.set((size_t)SceneMgrFlag::isQuiting);
	}

	//Get scene in the scene list by ID
	Scene& SceneManager::GetSceneByID(SceneID _id)
	{
		//If scene exists
		if(m_sceneList.count(_id) != 0)
			return m_sceneList.at(_id);
		else
		{
			LogCoreError("Scene ID %llu was not loaded", _id);
			return m_sceneList.at(NULL_SCENE_ID);
		}
	}

	//Get scene in the scene list by scene name
	Scene& SceneManager::GetSceneByName(const std::string& _sceneName)
	{
		//If scene exists
		if (m_sceneIDList.count(_sceneName) != 0)
			return m_sceneList.at(m_sceneIDList.at(_sceneName));
		else
		{
			LogCoreError("Scene named %s was not loaded", _sceneName.c_str());
			return m_sceneList.at(NULL_SCENE_ID);
		}
	}

	void SceneManager::Init() 
	{

	}

	void SceneManager::Awake()
	{

	}

	void SceneManager::Update(double dt)
	{
		UNUSED(dt);

		//If current and next scene IDs are different, and loading has not yet started
		if (m_currScene != m_nextScene && !m_flags.test((size_t)SceneMgrFlag::isLoading))
		{
			/*
			Scene Manager sends "Scene unloaded" event
				- [4] Audio stops all the audio
				- [3] ECS unloads all existing entities
				- [2] Asset Mgr loads loading scene stuff
				- [1] ECS loads loading scene stuff
				- [0] Asset Mgr unloads all assets (previous level)
				- [0] Asset Mgr loads next level assets (async)

			Asset Manager sends a "Assets Loading finished" event
				- ECS unloads all loading scene entities
				- ECS loads new scene stuff
				- Asset Mgr unloads unused assets (loading scene)
				- Asset Manager sends "Scene Loaded" event
			*/
			
			//Starts the loading
			m_flags.set((size_t)SceneMgrFlag::isLoading);

			SceneUnloadedEvent sul(m_sceneList.at(m_nextScene), m_sceneList.at(LOADING_SCENE_ID));
			EventMgr->Dispatch(sul);

		}
		//If it's flagged to restart scene
		else if (m_flags.test((size_t)SceneMgrFlag::isRestart) && !m_flags.test((size_t)SceneMgrFlag::isLoading))
		{
			SceneRestartEvent sr(m_sceneList.at(m_currScene));
			EventMgr->Dispatch(sr);
		}
		//If it's flagged to quit application
		else if (m_flags.test((size_t)SceneMgrFlag::isQuiting) && !m_flags.test((size_t)SceneMgrFlag::isLoading))
		{
			QuitEvent q;
			EventMgr->Dispatch(q);
		}

	}

	void SceneManager::Restart()
	{

	}

	void SceneManager::Shutdown()
	{

	}

	void SceneManager::OnSceneLoaded(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneLoaded, "Wrong event, expected SceneLoadedEvent, passed in %s instead.", e.GetEventName());

		//Scene is no longer loading
		m_flags.set((size_t)SceneMgrFlag::isLoading, false);

		//next scene is now current scene
		m_currScene = m_nextScene;
	}
}