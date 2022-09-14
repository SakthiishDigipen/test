#pragma once
#ifndef	ECS_COORDINATOR_H
#define ECS_COORDINATOR_H

#include "../ISystem.h"
#include "ECSComponent/ECSComponentMgr.h"
#include "ECSEntity/ECSEntityMgr.h"
#include "ECSTracker/ECSTrackerMgr.h"

namespace BE
{
	class ECSCoordinator : public Singleton<ECSCoordinator>, public ISystem
	{
		//template <typename T>
		//using Custom_Unique_Ptr = std::unique_ptr<T, std::function<void(T*)>>;
		
		//Each of the ECS managers
		Singleton<ECSEntityMgr>::SingletonPtr& m_entityMgr;
		Singleton<ECSComponentMgr>::SingletonPtr& m_componentMgr;
		Singleton<ECSTrackerMgr>::SingletonPtr& m_trackerMgr;

		/*
			Constructor and Destructor
		*/
		ECSCoordinator();
		~ECSCoordinator();

		//Used internally to officially remove entity from the ECS during ECS's update time
		void EraseEntity(EntityID _eid);

	public:
		/*
			Entity functions
		*/
		//Creates an entity with a known ID, or leave parameter empty for randomly generated ID
		EntityID CreateEntity(EntityID _eid = EntityID{});

		//Flag this entity as going to be removed during ECS's turn to update. Also destroys all the children
		void DestroyEntity(EntityID _eid);

		//Clone an entity with a given ID
		EntityID CloneEntity(EntityID _eid);

		//Get entity's component bitset
		inline const CompBitset& GetEntityCompBitset(EntityID _eid) const			{ return m_entityMgr->GetEntityComponentBitset(_eid); }

		//Get all entities that are currently in use
		inline const auto& GetAllEntities() const									{ return m_entityMgr->GetAllEntities(); }

		//Get the number of entities that are currently in use
		inline size_t GetEntitiesCount() const										{ return m_entityMgr->GetEntitiesCount(); }



		/*
			Tags functions
		*/
		//Create a new tag with a known ID, or leave 2nd parameter empty for randomly generated ID
		inline TagID CreateTag(std::string&& _tag, TagID _id = TagID{})				{ return m_entityMgr->CreateTag(std::move(_tag), _id); }

		//Destroy tag by name
		void DestroyTag(const std::string& _tag);

		//Get the tag string using the ID
		inline TagID GetTagID(const std::string& _tag) const						{ return m_entityMgr->GetTagID(_tag); }

		//Rename tag - if tag id doesn't exist, then just create a new one
		inline TagID RenameTag(const std::string& _oldTag, std::string&& _newTag)	{ return m_entityMgr->RenameTag(_oldTag, std::move(_newTag)); }

		//Checks if a tag is loaded
		inline bool IsTagCreated(const std::string& _tag) const						{ return m_entityMgr->IsTagCreated(_tag); }

		//Get all the tags that are in use
		inline const auto& GetAllTags() const										{ return m_entityMgr->GetAllTags(); }

		//Get the tag name by ID
		inline const std::string& GetTag(TagID id) const							{ return m_entityMgr->GetTag(id); }

		//Serialise and Deserialise all the tags - must call these before the EntityInfo's serialise/deserialise
		//Basically save the TagID mapping to Tag String
		inline void SerialiseAllTags()												{ m_entityMgr->SerialiseAllTags(); }
		inline void DeserialiseAllTags()											{ m_entityMgr->DeserialiseAllTags(); }

		/*
			Components Functions 
		*/
		//Add a component to an entity
		template <typename ComponentType>
		void AddComponent(EntityID _eid, ComponentType&& _data = ComponentType{});

		//Removes a component from an entity
		template <typename ComponentType>
		void RemoveComponent(EntityID _eid);

		//Checks if entity has specified component
		template <typename ComponentType>
		inline bool HasComponent(EntityID _eid) const									{ return m_entityMgr->HasComponent(_eid, m_componentMgr->GetComponentID<ComponentType>()); }

		//Getting component for an entity
		template <typename ComponentType>
		inline ComponentType& GetComponent(EntityID _eid)								{ return m_componentMgr->GetComponent<ComponentType>(_eid); }

		//Registering a new component for use
		template <typename ComponentType>
		inline ComponentID RegisterComponent()											{ return m_componentMgr->RegisterComponent<ComponentType>(); }

		//Get the ID of the component
		template <typename ComponentType>
		inline ComponentID GetComponentID()												{ return m_componentMgr->GetComponentID<ComponentType>(); }

		//Get all the components that are registered - editor usage
		inline const auto& GetAllComponentTypes() const									{ return m_componentMgr->GetAllComponentTypes(); }

		//Add a component to an entity - non-templated
		void AddComponent(const std::string& _componentName, EntityID _eid, IECSComponent&& _data);

		//Removes a component from an entity - non-templated
		void RemoveComponent(const std::string& _componentName, EntityID _eid);

		//Checks if an entity has a component
		inline bool HasComponent(const std::string& _componentName, EntityID _eid) const		{ return m_entityMgr->HasComponent(_eid, m_componentMgr->GetComponentID(_componentName)); }

		//Getting component for an entity
		inline IECSComponent& GetComponent(const std::string& _componentName, EntityID _eid)	{ return m_componentMgr->GetComponentNT(_componentName, _eid); }


		/*
			Tracker functions
		*/
		//Registers a new tracker for use to track entities with required components. Args is ctor param for SystemName
		template <typename TrackerName, typename... Args>
		inline TrackerID RegisterTracker(const CompBitset& _cbs, Args&& ...args)	{ return m_trackerMgr->RegisterTracker<TrackerName>(_cbs, std::forward<Args>(args)...); }

		//Get access to the tracker itself - contains the set of entities with required components
		template <typename TrackerName>
		inline TrackerName* const GetTracker()										{ return m_trackerMgr->GetTracker<TrackerName>(); }

		//Gets a reference to all the tracked entities by the tracker
		template <typename TrackerName>
		inline const auto& GetTrackedEntities()										{ return m_trackerMgr->GetTrackedEntities<TrackerName>(); }


		/*
			Seacrhing functions
		*/
		//Search for all entities that has _substr in their name, 
		//No. of entities limited by count, optionally filtered by active status
		std::vector<EntityID> FindEntitiesByName(const std::string& _substr, size_t count = MAX_ENTITIES, const std::optional<bool>& _isActive = std::nullopt);

		//Search for all entities that has a specified tag in their tag list,
		//No. of entities limited by count, optionally filtered by active status
		std::vector<EntityID> FindEntitiesByTag(const std::string& _tag, size_t count = MAX_ENTITIES, const std::optional<bool>& _isActive = std::nullopt);

		//Search for all entities fulfils the unary condition,
		//No. of entities limited by count, optionally filtered by active status
		std::vector<EntityID> FindEntitiesByCondition(bool (*condition)(EntityID), size_t count = MAX_ENTITIES, const std::optional<bool>& _isActive = std::nullopt);



		/*
			System functions
		*/
		
		ISYSTEM_FUNCTIONS;

		/*
			Event handling
		*/

		//When scene is unloaded, first need to unload entities
		void OnSceneUnload_Part1(IEvent& e);

		//When scene is unloaded, then need to load loading scene entities
		void OnSceneUnload_Part2(IEvent& e);

		//When scene is restarting, need to reload all entities
		void OnSceneRestart(IEvent& e);

		//When assets loaded for new scene, need to start loading entities for new scene
		void OnAssetsLoaded(IEvent& e);

		friend class Singleton<ECSCoordinator>;
	};

	#define ECS ECSCoordinator::GetInstance()
}


#include "ECS.hpp"

#endif