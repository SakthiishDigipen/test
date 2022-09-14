#include "pch.h"
#include "EntityInfo.h"
#include <Systems/ECS/ECS.h>

namespace BE
{
	EntityInfo::EntityInfo() 
		: m_name{ "New Entity" }, m_tags{}, m_flags{}
	{
		//Reserve some space for the entity name
		m_name.reserve(32);

		//Turn on active flag
		this->SetFlag(EntityFlag::active, true);
	}

	//Add tag to the entity
	void EntityInfo::AddTag(const std::string& _tag)
	{		
		//If tag is not added to this entity, then add that tag
		if (!HasTag(_tag))
			m_tags.push_back(ECS->GetTagID(_tag));
		else
			LogCoreError("Entity already has the Tag \"%s\".", _tag.c_str());
	}

	//Remove tag from the entity
	void EntityInfo::RemoveTag(const std::string& _tag)
	{
		//If tag is added to this entity, then remove that tag
		decltype(m_tags)::const_iterator it;

		if (HasTag(_tag, &it))
			m_tags.erase(it);
		else
			LogCoreError("Entity does not have the Tag \"%s\".", _tag.c_str());
	}

	//Check if entity has the tag
	bool EntityInfo::HasTag(const std::string& _tag, const std::optional<std::vector<TagID>::const_iterator*>& _where) const
	{
		//If tag doesnt exist then it wont exist on any entity
		if (!ECS->IsTagCreated(_tag))
			return false;

		auto it = std::find(m_tags.begin(), m_tags.end(), ECS->GetTagID(_tag));

		//If specified a pointer to iterator to store where _tag is located in m_tags, then assign the value
		if (_where)
			**_where = it;

		return it != m_tags.end();
	}




	void EntityInfo::LoadObject(const rapidjson::Value&)
	{
		//Preferably use the setters to deserialise
	}

	void EntityInfo::SaveObject(rapidjson::Value&, rapidjson::Document&)
	{
		//Serialise
	}

}