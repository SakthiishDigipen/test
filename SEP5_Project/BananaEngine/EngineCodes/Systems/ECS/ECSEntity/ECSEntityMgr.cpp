#include "pch.h"
#include "ECSEntityMgr.h"

namespace BE
{
	ECSEntityMgr::ECSEntityMgr()
	{
		m_allEntities.reserve(MAX_ENTITIES);
	}

	ECSEntityMgr::~ECSEntityMgr()
	{

	}

	EntityID ECSEntityMgr::CreateEntity(EntityID _eid)
	{
		AssertCore(m_allEntities.size() < MAX_ENTITIES, "Reached the maximum number of entities: %d.", MAX_ENTITIES);
		AssertCore(_eid != NULL_ENTITY, "Cannot create entity with ID 0");

		//If entity ID already exists, then generate a new one
		while (m_IDToIndexMap.count(_eid) != 0)
		{
			LogCoreWarning("Duplicated EntityID created, generating new EntityID");
			_eid = EntityID{};
		}

		//Get the index to the last element of the all entities vector
		size_t i = m_allEntities.size();

		//Save the uuid, index mapping
		m_IDToIndexMap[_eid] = i;
		m_indexToIDMap[i] = _eid;

		//Add this entity to the end of all entities
		m_allEntities.push_back(_eid);

		return _eid;
	}

	void ECSEntityMgr::DestroyEntity(EntityID _eid)
	{
		//Need to check that entities does exist
		AssertCore(m_IDToIndexMap.count(_eid) != 0, "Entity %llu does not exist", _eid);

		//Determine the last entity that will take over the memory position of the entity to be destroyed
		EntityID lastEntity = m_allEntities.back();
		size_t indexToLast = m_allEntities.size() - 1;
		size_t indexToReplace = m_IDToIndexMap.at(_eid);

		//Transfer the component bitset data for the last entity to replace the one to be destroyed
		m_entityComponentBitset[indexToReplace] = m_entityComponentBitset[indexToLast];
		m_entityComponentBitset[indexToLast].reset();

		//Move the last entity to the spot of the destroyed entity
		m_allEntities[indexToReplace] = lastEntity;
		m_allEntities.pop_back();

		//Update the mapping
		m_IDToIndexMap[lastEntity] = indexToReplace;
		m_indexToIDMap[indexToReplace] = lastEntity;

		//Destroy the old entity mapping
		m_IDToIndexMap.erase(_eid);
		m_indexToIDMap.erase(indexToLast);
	}


	void ECSEntityMgr::AddComponent(EntityID _eid, ComponentID _cid)
	{
		AssertCore(m_IDToIndexMap.count(_eid) != 0, "Entity %llu does not exist", _eid);
		m_entityComponentBitset[m_IDToIndexMap.at(_eid)].set(_cid, true);
	}

	void ECSEntityMgr::RemoveComponent(EntityID _eid, ComponentID _cid)
	{
		AssertCore(m_IDToIndexMap.count(_eid) != 0, "Entity %llu does not exist", _eid);
		m_entityComponentBitset[m_IDToIndexMap.at(_eid)].set(_cid, false);
	}

	bool ECSEntityMgr::HasComponent(EntityID _eid, ComponentID _cid) const
	{
		AssertCore(m_IDToIndexMap.count(_eid) != 0, "Entity %llu does not exist", _eid);
		return m_entityComponentBitset[m_IDToIndexMap.at(_eid)].test(_cid);
	}

	const CompBitset& ECSEntityMgr::GetEntityComponentBitset(EntityID _eid) const
	{
		AssertCore(m_IDToIndexMap.count(_eid) != 0, "Entity %llu does not exist", _eid);
		return m_entityComponentBitset[m_IDToIndexMap.at(_eid)];
	}


	TagID ECSEntityMgr::CreateTag(std::string&& _tag, TagID _id)
	{
		//Ensure no duplicate TagIDs and Tag names
		for (const auto& pair : m_allTags)
		{
			//If the tag already exists, just return the ID of that tag
			if (pair.first == _tag)
			{
				LogCoreWarning("Tag \"%s\" was already created, it's TagID is %llu", _tag.c_str(), m_allTags.at(_tag));
				return m_allTags.at(_tag);
			}

			//If found a duplicate TagID
			if (pair.second == _id)
			{
				LogCoreWarning("Duplicated TagID %llu found, Creating Tag \"%s\" with a new ID", _id, _tag.c_str());
				
				//Create a new ID instead
				_id = TagID{};
				break;
			}
		}

		//Create a new tag
		m_allTags.emplace(std::move(_tag), _id);
		return _id;
	}

	void ECSEntityMgr::DestroyTag(const std::string& _tag)
	{
		m_allTags.erase(_tag);
	}

	TagID ECSEntityMgr::GetTagID(const std::string& _tag) const
	{
		AssertCore(IsTagCreated(_tag), "Tag \"%s\" is not created in the Tags Collection. Use ECS Coordinator's CreateTag() before adding tags to any entities", _tag.c_str());
		return m_allTags.at(_tag);
	}

	TagID ECSEntityMgr::RenameTag(const std::string& _oldTag, std::string&& _newTag)
	{
		//If duplicate tag string is found, then don't do anything. Return TagID of the duplicated one
		if (IsTagCreated(_newTag))
		{
			LogCoreWarning("Tag \"%s\" already exists. No renaming happened.", _newTag.c_str());
			return m_allTags.at(_newTag);
		}

		TagID id;

		//If the ID exist, then replace the old name
		if (m_allTags.count(_oldTag) != 0)
		{
			//Get the old TagID first
			id = m_allTags.at(_oldTag);

			//Delete the old one
			m_allTags.erase(_oldTag);
		}
		//If the ID doesn't exist, then just create a new one
		else
		{
			id = TagID{};
			LogCoreWarning("No tag called \"%s\" exists, creating new one with TagID %llu", _oldTag.c_str(), id);
		}
		this->CreateTag(std::move(_newTag), id);

		return id;
	}

	bool ECSEntityMgr::IsTagCreated(const std::string& _tag) const
	{
		return m_allTags.count(_tag) != 0;
	}

	const std::string& ECSEntityMgr::GetTag(TagID id) const
	{
		for (const auto& pair : m_allTags)
		{
			if (pair.second == id)
				return pair.first;
		}

		static std::string errString{};
		LogCoreError("No tag with ID %llu is found", id);
		return errString;
	}

	void ECSEntityMgr::SerialiseAllTags()
	{
		//Save the tag ID to tag string mapping
	}

	void ECSEntityMgr::DeserialiseAllTags()
	{
		//Load the tag ID to tag string mapping
	}
}