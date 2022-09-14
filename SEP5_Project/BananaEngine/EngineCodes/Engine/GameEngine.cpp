#include "pch.h"
#include "GameEngine.h"

namespace BE
{
	GameEngine::GameEngine()
	{
		//Init RNG
		Rand::InitRNG();

		//Load the engine configuration
		GameEngine::EngineConfig config = LoadConfig("../Data/config.txt");

		//Init the miscellaneous systems
		Debug::CreateInstance();
		EventDispatcher::CreateInstance();
		AssetManager::CreateInstance();

		//Init engine configs
		m_desiredFPS = config.m_maxFrameRate;
		m_deltaTime = 0.0;
		m_frameCount = 0;
		m_startTime = 0.0;
		m_endTime = 0.0;
		m_fixedDT = 1.0 / m_desiredFPS;
		m_accumulatedTime = 0.0;
		m_physSteps = 0;
		m_run = true;

		//Create an instance of the systems
		this->CreateSystem<GLFWSystem>();
		this->CreateSystem<ECSCoordinator>();
		this->CreateSystem<SceneManager>();
		//this->CreateSystem<VulkanSystem>();
	}

	GameEngine::~GameEngine()
	{
		GameEngine::EngineConfig ec{ m_desiredFPS, 
									/*m_windowWidth
									m_windowHeight
									m_isFullScreen
									m_musicVolume
									m_soundVolume
									m_appName*/ };

		SaveConfig(ec, "../Data/config.txt");
		LogCore("Destroyed GameEngine");
	}

	void GameEngine::Init()
	{
		m_frameCount = 0;

		m_startTime = 0.0;
		m_endTime = 0.0;

		m_accumulatedTime = 0.0;
		m_physSteps = 0;

		m_run = true;

		//Init all the systems
		for (auto& sys : m_systems)
			sys->Init();
	}

	void GameEngine::Awake()
	{
		m_frameCount = 0;

		m_startTime = 0.0;
		m_endTime = 0.0;

		m_accumulatedTime = 0.0;
		m_physSteps = 0;

		m_run = true;

		//Init all the systems
		for (auto& sys : m_systems)
			sys->Awake();
	}

	void GameEngine::Update()
	{

		/*------------ Start of new frame ----------------*/

		//Time at the start of current frame
		m_startTime = glfwGetTime();

		//Accumulate time for this frame
		m_accumulatedTime += m_deltaTime;
		m_physSteps = 0;

		//Until the frame has lasted longer than the fixed DT
		while (m_accumulatedTime >= m_fixedDT)
		{
			m_accumulatedTime -= m_fixedDT;
			++m_physSteps;
		}

		//Increase frame count
		++m_frameCount;

		//Update all the systems
		for (auto& sys : m_systems)
		{
#ifdef USE_EDITOR
			//Profiling - how much time each system is using to run update
			sys->StartSystemDt();
			sys->Update(m_deltaTime); 
			sys->EndSystemDt();
#else
			sys->Update(m_deltaTime, m_fixedDT, m_physSteps);
#endif
		}

		//Get the new delta time value for the next frame
		m_endTime = glfwGetTime();
		m_deltaTime = m_endTime - m_startTime;

	}

	void GameEngine::Restart()
	{
		for (auto& sys : m_systems)
			sys->Restart();
	}

	void GameEngine::Shutdown()
	{
		for (auto& sys : m_systems)
			sys->Shutdown();
	}

	GameEngine::EngineConfig GameEngine::LoadConfig(const char* _fileName)
	{
		//Result config will be stored here
		GameEngine::EngineConfig ec;

		if (_fileName)
		{
			//Open the config file for reading
			std::ifstream ifs(_fileName, std::ios::in);

			//If the file doesn't exists, then just load a default value
			if (!ifs.is_open())
			{
				LogCoreError("No config found, created default settings");
				return ec;
			}
			
			//This is going to absorb the text title
			std::string titleText{};
			titleText.reserve(20);

			//Loading engine info
			ifs >> titleText >> ec.m_maxFrameRate;
			ifs >> titleText >> ec.m_windowWidth;
			ifs >> titleText >> ec.m_windowHeight;
			ifs >> titleText >> ec.m_isFullScreen;
			ifs >> titleText >> ec.m_musicVolume;
			ifs >> titleText >> ec.m_soundVolume;

			//Loading the application name
			ifs >> titleText;
			char appName[MAX_STR_BUFFER];
			ifs.get();					//Absorbs the space between the titleText and the data
			ifs.getline(appName, MAX_STR_BUFFER);
			ec.m_appName = appName;

			ifs.close();
		}
		else
		{
			//default config if no file is provided
			LogCore("No engine config file provided, using default config instead");
		}

		return ec;
	}

	void GameEngine::SaveConfig(const GameEngine::EngineConfig& _ec, const char* _configFileName)
	{
		if (_configFileName)
		{
			std::ofstream ofs(_configFileName, std::ios::out);

			ofs << "Framerate: "    << _ec.m_maxFrameRate << '\n';
			ofs << "WindowWidth: "  << _ec.m_windowWidth  << '\n';
			ofs << "WindowHeight: " << _ec.m_windowHeight << '\n';
			ofs << "IsFullScreen: " << _ec.m_isFullScreen << '\n';
			ofs << "MusicVolume: "  << _ec.m_musicVolume  << '\n';
			ofs << "SoundVolume: "  << _ec.m_soundVolume  << '\n';
			ofs << "AppName: "      << _ec.m_appName      << '\n';

			ofs.close();
		}
		else
		{
			LogCoreWarning("Cannot give nullptr to file name in SaveConfig()");
		}
	}
}