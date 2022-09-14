#pragma once

namespace BE
{
	template <typename ComponentType>
	void ECSCoordinator::AddComponent(EntityID _eid, ComponentType&& _data)
	{
		//Update the entity's component bitset
		m_entityMgr->AddComponent(_eid, m_componentMgr->GetComponentID<ComponentType>());

		//Put the actual data into the component array
		m_componentMgr->AddComponent<ComponentType>(_eid, std::move(_data));

		//Update all the trackers to keep track of this entity
		m_trackerMgr->OnEntityComponentChanged(_eid, m_entityMgr->GetEntityComponentBitset(_eid));
	}

	template <typename ComponentType>
	void ECSCoordinator::RemoveComponent(EntityID _eid)
	{
		if constexpr (std::is_same<ComponentType, Transform>::value)
		{
			LogCoreError("Cannot remove Transform component");
		}
		else if constexpr (std::is_same<ComponentType, EntityInfo>::value)
		{
			LogCoreError("Cannot remove EntityInfo component");
		}
		else
		{
			//Update the entity's component bitset
			m_entityMgr->RemoveComponent(_eid, m_componentMgr->GetComponentID<ComponentType>());

			//Remove the actual data from the component array
			m_componentMgr->RemoveComponent<ComponentType>(_eid);

			//Update all the trackers to keep track of this entity
			m_trackerMgr->OnEntityComponentChanged(_eid, m_entityMgr->GetEntityComponentBitset(_eid));
		}
	}
}