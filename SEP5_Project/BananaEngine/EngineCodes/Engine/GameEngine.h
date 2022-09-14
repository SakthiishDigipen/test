#pragma once
#ifndef	GAMEENGINE_H
#define GAMEENGINE_H

#include <Systems/AllSystems.h>


namespace BE
{
	class GameEngine : public Singleton<GameEngine>
	{
		//All the engine's system
		std::vector<ISystem*> m_systems;

		//Times and framerates
		double m_desiredFPS;		//What frames per second should the engine aim for
		double m_deltaTime;			//DT of the engine
		size_t m_frameCount;		//How many frames have passed by

		double m_startTime;			//The time at the start of the current frame loop, for DT calculation
		double m_endTime;			//The time at the end of the current frame loop, for DT calculation

		//Fixed DT stuff for physics
		double m_fixedDT;			//Fixed DT of the engine (1/desiredFPS) meant for physics calculation
		double m_accumulatedTime;	//How much time passed since start of this frame
		int    m_physSteps;			//Number of steps to go through for physics calculation (timeSteps)

		//Whether to run the application
		bool m_run;

		//Creates an engine with the given configFile name. Leave blank for default config
		GameEngine();
		~GameEngine();

		//For creating a new engine system, to be put into m_systems - Warning: Do not create the same system twice!
		template<typename T_Sys, typename... CtorParam>
		void CreateSystem(CtorParam&& ...args);

	public:
		//Functions to run
		/*------------- Client side to include these functions in their application class --------------------*/
		void Init();		//Initialise all the systems and the engine
		void Awake();		//Awake all the systems and the engine
		void Update();		//Updates all the systems, to be called every frame
		void Restart();		//For restarting all the systems
		void Shutdown();	//Uninit the systems and free the memory
		/*----------------------------------------------------------------------------------------------------*/

		//Tell the engine to quit
		inline void Quit()										{ m_run = false; }

		//Getters
		inline bool GetIsRunning() const						{ return m_run; }
		inline double GetDesiredFPS() const						{ return m_desiredFPS; }
		inline double GetDeltaTime() const						{ return m_deltaTime; }
		inline double GetFixedDeltaTime() const					{ return m_fixedDT; }
		inline double GetTimeElapsed() const					{ return glfwGetTime(); }
		inline double GetFramerate() const						{ return 1.0/m_deltaTime; }
		inline double GetFixedFramerate() const					{ return 1.0/m_fixedDT; }
		inline size_t GetCurrFrame() const						{ return m_frameCount; }
		inline int	  GetPhysSteps() const						{ return m_physSteps; }

		//------------------------------------------ Engine configuration -------------------------------------------

		struct EngineConfig
		{
			double		m_maxFrameRate = 60.0;				//what desired frame rate to run the engine in?
			int			m_windowWidth  = 1920;				//What window width to open up with (non-fullscreen)?
			int			m_windowHeight = 1080;				//What window height to open up with (non-fullscreen)?
			bool		m_isFullScreen = false;				//Whether the application will run full screen?
			float		m_musicVolume  = 1.0f;				//The global volume of the music (for audio)
			float		m_soundVolume  = 1.0f;				//The global volume of the sound effects (for audio)
			std::string	m_appName	   = "Banana Engine";	//What name to give for the application?
		};

		//Engine configuration
		EngineConfig LoadConfig(const char* _fileName = nullptr);					//Load an engine configuration. Leave blank for default config
		void SaveConfig(const EngineConfig& _ec, const char* _configFileName);		//Save the engine configuration

		//-----------------------------------------------------------------------------------------------------------
		
		friend class Singleton<GameEngine>;
	};

	#define Engine			GameEngine::GetInstance()
}

#include "GameEngine.hpp"

#endif