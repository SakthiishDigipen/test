#ifndef BANANA_SERIALISATION_H
#define BANANA_SERIALISATION_H
#include <Precompiled/pch.h>
#include <Components/ECSComponent.h>
#include <Systems/ECS/ECS.h>
namespace BE
{
	namespace Serialisation
	{
		enum class MetaDataKey
		{
			Type,
			Serialize,
			Copy
		};
		enum class MetaDataValue
		{
			BEID,
			Vector3Float
		};
		static const std::string VectorType;
		static const std::string MetaType;
		void LoadEntities(const std::string& fileName);
		void SaveEntities(const std::string& output);
		template<typename T>
		void SerializeToValue(rapidjson::Value& v, rapidjson::Document& d)
		{
			rttr::type type = typeid(T).name();

		}
	}
};
#endif // !1
