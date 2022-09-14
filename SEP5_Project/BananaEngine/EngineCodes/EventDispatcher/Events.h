#pragma once
#ifndef	EVENTS_H
#define EVENTS_H

#include <Precompiled/pch.h>
#include <Debugger/Debugger.h>
#include <Systems/SceneManager/Scene.h>
#include <Systems/ECS/ECSConfig.h>

namespace BE
{
	//Event types that the engine will be processing throughout


	//The types of events that can be handled by the event manager
	enum class EventType
	{
		//Application
		quit = 0,			//For when the application needs to close
		restart,			//For when the application needs to restart

		//Windows
		windowInterrupt,	//For when window is interrupted/lost focus on/off
		toggleFullScreen,	//For when window is toggled fullscreen on/off

		//Inputs
		keyButton,			//For when a key button is pressed
		mouseButton,		//For when a mouse button is pressed
		mouseMove,			//For when a mouse button is moved
		mouseScroll,		//For when mouse scrollwheel is scrolled

		//Scenes
		sceneUnloaded,		//For when the current switch is unloaded, like switching to a new scene
		sceneLoaded,		//For when a new scene has finished loading,
		sceneRestart,		//For restarting scene

		//Assets loading
		assetsLoaded,		//For when all required assets have been loaded

		//Miscellaneous
		collision,			//For when collision happened
		entityDestroy,		//For when entity is destroyed

		//IMGUI
		toggleIMGUITextbox,	//For when typing in textbox in IMGUI

		//...
		last
	};

	//Base class for all event type structs
	class IEvent
	{
	protected:
		bool m_handled = false;		//If true, disable the event from getting propagated further
		
		IEvent() = default;
		virtual ~IEvent() = 0 {}

	public:
		//Handling of event. If want to stop event from propagating further, call MarkAsHandled
		inline void MarkAsHandled()					{ m_handled = true; }
		inline bool IsHandled() const				{ return m_handled; }

		//Getters
		virtual inline EventType GetEventType() const = 0;

		//For debug
		virtual inline const char* GetEventName() const = 0;							//Get the name of this event
		virtual void LogEvent() const				{ LogCore("%s", GetEventName()); }	//Logs the event into the debugger/console

		friend class EventDispatcher;
	};

	//	--------------------------------V Define all events here V--------------------------------------------

	//Add this line to the event class that you are creating. type is an enum value in EventType.
	#define EVENT_TYPE(type)	inline static EventType GetStaticType()  			{ return EventType::##type; } \
								inline EventType GetEventType() const override		{ return GetStaticType(); } \
								inline const char* GetEventName() const override	{ return "Event: "#type; }

	//GetStaticType() will return the event type for that class.
	//GetEventType() will return the event type for the event object calling it.
	//GetEventName() prints the event type as a string, for debugging. 
	//GetEventInfo() prints more detailed info for the event, for debugging.

	//	------------------------------------------------------------------------------------------------------

	//For when the application is quitting
	class QuitEvent : public IEvent
	{
	public:
		QuitEvent() = default;
		EVENT_TYPE(quit)
	};

	//For when restarting the application
	class RestartEvent : public IEvent
	{
	public:
		RestartEvent() = default;
		EVENT_TYPE(restart)
	};

	//For when a scene is getting unloaded and a new scene is going to load
	class SceneUnloadedEvent : public IEvent
	{
		Scene newScene;
		Scene loadingScene;

	public:
		SceneUnloadedEvent(const Scene& _newScene, const Scene& _loadingScene) : newScene(_newScene), loadingScene(_loadingScene) {}
		inline const Scene& GetNextScene() { return newScene; }
		inline const Scene& GetLoadingScene() { return loadingScene; }

		EVENT_TYPE(sceneUnloaded)
	};

	//For when a scene finished loading
	class SceneLoadedEvent : public IEvent
	{
	public:
		SceneLoadedEvent() = default;
		EVENT_TYPE(sceneLoaded)
	};

	//For when a scene needs to restart
	class SceneRestartEvent : public IEvent
	{
		Scene currScene;
	public:
		SceneRestartEvent(const Scene& _currScene) : currScene(_currScene) {}
		inline const Scene& GetScene() { return currScene; }
		EVENT_TYPE(sceneRestart)
	};

	//For when all the required assets for the new scene have been loaded
	class AssetsLoadedEvent : public IEvent
	{
		Scene loadedScene;
	public:
		AssetsLoadedEvent(Scene _scene) : loadedScene(_scene) {}
		inline const Scene& GetScene() { return loadedScene; }

		EVENT_TYPE(assetsLoaded)
	};

	//For when entity is destroyed
	class EntityDestroyedEvent : public IEvent
	{
		EntityID e;
	public:
		EntityDestroyedEvent(EntityID _e) : e(_e) {}
		inline const EntityID& GetDestroyedEntity() { return e; }

		EVENT_TYPE(entityDestroy)
	};
}

#endif