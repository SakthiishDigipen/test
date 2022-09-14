#include "pch.h"
#include "ECS.h"

namespace BE
{
	ECSCoordinator::ECSCoordinator() 
		: m_entityMgr{ ECSEntityMgr::CreateInstance() },
		  m_componentMgr{ ECSComponentMgr::CreateInstance() },
		  m_trackerMgr{ ECSTrackerMgr::CreateInstance() }
	{
		//Register all required components
		RegisterComponent<Transform>();
		RegisterComponent<EntityInfo>();

		//Register all required systems here
		//CompBitset cbs;
		//cbs.set(GetComponentID<Transform>());
		//cbs.set(GetComponentID<EntityInfo>());
		//RegisterTracker<PhysicsSystem>(cbs);

		//Subscribe to events

		//When scene is unloaded, it needs to unload all entities
		EventMgr->SubscribeWPrio(SceneUnloadedEvent, this, ECSCoordinator::OnSceneUnload_Part1, 3);
		EventMgr->SubscribeWPrio(SceneUnloadedEvent, this, ECSCoordinator::OnSceneUnload_Part2, 1);

		//When assets have been loaded, it needs to load the entities
		EventMgr->Subscribe(AssetsLoadedEvent, this, ECSCoordinator::OnAssetsLoaded);

		//When scene is restart, it needs to reload all entities
		EventMgr->Subscribe(SceneRestartEvent, this, ECSCoordinator::OnSceneRestart);

		LogCore("Created ECS System");
	}

	ECSCoordinator::~ECSCoordinator()
	{
		LogCore("Destroyed ECS System");
	}

	EntityID ECSCoordinator::CreateEntity(EntityID _eid)
	{
		//In case there's a change in ID during the creation process
		EntityID id = m_entityMgr->CreateEntity(_eid);

		//Everything requires transform and entity info component
		this->AddComponent<Transform>(id);
		this->AddComponent<EntityInfo>(id);

		return id;
	}

	void ECSCoordinator::DestroyEntity(EntityID _eid)
	{
		//Set "destroy flag" to true, this is to signal the ECS that during it's update, remove this entity completely.
		this->GetComponent<EntityInfo>(_eid).SetFlag(EntityFlag::destroy, true);

		//Run the same process for the children.
		//Destroy the last child by popping back - as the GetChildre()'s size will decrease while children are destroyed
		while(ECS->GetComponent<Transform>(_eid).GetChildrenCount() > 0)
		{
			DestroyEntity(ECS->GetComponent<Transform>(_eid).GetChildren().back());
		}

		//If the deleted entity has parent, detach it
		if (ECS->GetComponent<Transform>(_eid).HasParent())
		{
			EntityID parentID = ECS->GetComponent<Transform>(_eid).GetParent();
			ECS->GetComponent<Transform>(parentID).DetachChild(_eid);
		}
	}

	void ECSCoordinator::EraseEntity(EntityID _eid)
	{
		//Notify that an entity has been destroyed
		EntityDestroyedEvent ed(_eid);
		EventMgr->Dispatch(ed);

		m_componentMgr->OnEntityDestroyed(_eid);
		m_trackerMgr->OnEntityDestroyed(_eid);
		m_entityMgr->DestroyEntity(_eid);
	}

	EntityID ECSCoordinator::CloneEntity(EntityID _eid)
	{
		EntityID newID = this->CreateEntity();

		//Copy transform and entity info
		m_componentMgr->GetComponent<Transform>(newID) = m_componentMgr->GetComponent<Transform>(_eid);
		m_componentMgr->GetComponent<EntityInfo>(newID) = m_componentMgr->GetComponent<EntityInfo>(_eid);

		//Other stuff to clone
	


		//Also clone the children!
		const auto& children = m_componentMgr->GetComponent<Transform>(_eid).GetChildren();

		//For each children in children list
		for (EntityID child : children)
		{
			//Clone that child
			EntityID clonedChild = CloneEntity(child);

			//Insert the cloned child child to the cloned entity
			m_componentMgr->GetComponent<Transform>(newID).AttachChild(newID, clonedChild);
		}

		return newID;
	}


	void ECSCoordinator::DestroyTag(const std::string& _tag)
	{
		//For each entity, remove the tag from the tag list
		for (EntityID e : m_entityMgr->GetAllEntities())
			m_componentMgr->GetComponent<EntityInfo>(e).RemoveTag(_tag);

		//Destroy the tag from the tag collection
		m_entityMgr->DestroyTag(_tag);
	}

	std::vector<EntityID> ECSCoordinator::FindEntitiesByName(const std::string& _substr, size_t count, const std::optional<bool>& _isActive)
	{
		std::vector<EntityID> result;
		result.reserve(128);

		for (EntityID id : m_entityMgr->GetAllEntities())
		{
			//Limit the result to this number of entities
			if (result.size() >= count)
				break;

			const EntityInfo& info = m_componentMgr->GetComponent<EntityInfo>(id);

			//If entity's name contains _substr, and 
			//either the isActive is not specfied, or it matches the requested active status
			if (info.GetName().find(_substr) < info.GetName().size() && 
				(!_isActive || info.HasFlag(EntityFlag::active) == *_isActive))
				result.push_back(id);
		}

		return result;
	}

	std::vector<EntityID> ECSCoordinator::FindEntitiesByTag(const std::string& _tag, size_t count, const std::optional<bool>& _isActive)
	{
		std::vector<EntityID> result;
		result.reserve(128);

		for (EntityID id : m_entityMgr->GetAllEntities())
		{
			//Limit the result to this number of entities
			if (result.size() >= count)
				break;

			const EntityInfo& info = m_componentMgr->GetComponent<EntityInfo>(id);

			//If entity has the required tag, and 
			//either the isActive is not specfied, or it matches the requested active status
			if (info.HasTag(_tag) && (!_isActive || info.HasFlag(EntityFlag::active) == *_isActive))
				result.push_back(id);
		}

		return result;
	}

	std::vector<EntityID> ECSCoordinator::FindEntitiesByCondition(bool (*condition)(EntityID), size_t count, const std::optional<bool>& _isActive)
	{
		std::vector<EntityID> result;
		result.reserve(128);

		for (EntityID id : m_entityMgr->GetAllEntities())
		{
			//Limit the result to this number of entities
			if (result.size() >= count)
				break;

			const EntityInfo& info = m_componentMgr->GetComponent<EntityInfo>(id);

			//If entity has fulfills the condition, and 
			//either the isActive is not specfied, or it matches the requested active status
			if (condition(id) && (!_isActive || info.HasFlag(EntityFlag::active) == *_isActive))
				result.push_back(id);
		}

		return result;
	}


	//Add a component to an entity - non-templated
	void ECSCoordinator::AddComponent(const std::string& _componentName, EntityID _eid, IECSComponent&& _data)
	{
		//Update the entity's component bitset
		m_entityMgr->AddComponent(_eid, m_componentMgr->GetComponentID(_componentName));

		//Put the actual data into the component array
		m_componentMgr->AddComponentNT(_componentName, _eid, std::move(_data));

		//Update all the trackers to keep track of this entity
		m_trackerMgr->OnEntityComponentChanged(_eid, m_entityMgr->GetEntityComponentBitset(_eid));
	}

	//Removes a component from an entity - non-templated
	void ECSCoordinator::RemoveComponent(const std::string& _componentName, EntityID _eid)
	{
		if (_componentName == "Transform")
		{
			LogCoreError("Cannot remove Transform component");
		}
		else if (_componentName == "EntityInfo")
		{
			LogCoreError("Cannot remove EntityInfo component");
		}
		else
		{
			//Update the entity's component bitset
			m_entityMgr->RemoveComponent(_eid, m_componentMgr->GetComponentID(_componentName));

			//Remove the actual data from the component array
			m_componentMgr->RemoveComponentNT(_componentName, _eid);

			//Update all the trackers to keep track of this entity
			m_trackerMgr->OnEntityComponentChanged(_eid, m_entityMgr->GetEntityComponentBitset(_eid));
		}
	}



	void ECSCoordinator::Init()
	{

	}

	void ECSCoordinator::Awake()
	{

	}

	void ECSCoordinator::Update(double dt)
	{
		UNUSED(dt);

		//At end of frame, destroy all entities that are tagged to be removed.
		//Destroy backwards as GetAllEntities().size() will decrease over time.
		for (int i = static_cast<int>(GetAllEntities().size()) - 1; i >= 0; --i)
		{
			if (GetComponent<EntityInfo>(GetAllEntities()[i]).HasFlag(EntityFlag::destroy))
				EraseEntity(GetAllEntities()[i]);
		}
	}

	void ECSCoordinator::Restart()
	{
		//Destroy all entities backwards
		for (int i = static_cast<int>(GetAllEntities().size()) - 1; i >= 0; --i)
		{
			EraseEntity(GetAllEntities()[i]);
		}
	}

	void ECSCoordinator::Shutdown()
	{

	}

	void ECSCoordinator::OnSceneUnload_Part1(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneUnloaded, "Wrong event, expected SceneUnloadedEvent, passed in %s instead.", e.GetEventName());
		
		//Destroy all entities backwards
		for (int i = static_cast<int>(GetAllEntities().size()) - 1; i >= 0; --i)
			EraseEntity(GetAllEntities()[i]);
	}

	void ECSCoordinator::OnSceneUnload_Part2(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneUnloaded, "Wrong event, expected SceneUnloadedEvent, passed in %s instead.", e.GetEventName());

		//To do: Load loading scene entities here by reading the scene file
		SceneUnloadedEvent& sul = dynamic_cast<SceneUnloadedEvent&>(e);
		sul.GetLoadingScene().GetSceneFile();

		/*
			Read scene file here
		*/
	}


	void ECSCoordinator::OnSceneRestart(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneRestart, "Wrong event, expected SceneRestartEvent, passed in %s instead.", e.GetEventName());

		//Destroy all entities backwards
		for (int i = static_cast<int>(GetAllEntities().size()) - 1; i >= 0; --i)
			EraseEntity(GetAllEntities()[i]);

		//To do: Reading scene file here
		SceneRestartEvent& sr = dynamic_cast<SceneRestartEvent&>(e);
		sr.GetScene().GetSceneFile();

		/*
			Read scene file here
		*/

		//Say that loading is done
		SceneLoadedEvent sl;
		EventMgr->Dispatch(sl);
	}

	void ECSCoordinator::OnAssetsLoaded(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::assetsLoaded, "Wrong event, expected AssetsLoadedEvent, passed in %s instead.", e.GetEventName());
		
		//Destroy all loading scene entities backwards
		for (int i = static_cast<int>(GetAllEntities().size()) - 1; i >= 0; --i)
			EraseEntity(GetAllEntities()[i]);

		//To do: Reading scene file here
		AssetsLoadedEvent& al = dynamic_cast<AssetsLoadedEvent&>(e);
		al.GetScene().GetSceneFile();

		/*
			Read scene file here
		*/

		//Asset Manager side will need to unload loading screen assets
	}
}