#include "pch.h"
#include "BananaSerialisation.h"
static const std::string S_EntityID = "EntityID";

using namespace rapidjson;
using namespace rttr;
inline bool write_atomic_types_to_json(const type& t, const variant& var, PrettyWriter<StringBuffer>& writer)
{
	if (t.is_arithmetic())
	{
		if (t == type::get<bool>())
			writer.Bool(var.to_bool());
		else if (t == type::get<char>())
			writer.Bool(var.to_bool());
		else if (t == type::get<int8_t>())
			writer.Int(var.to_int8());
		else if (t == type::get<int16_t>())
			writer.Int(var.to_int16());
		else if (t == type::get<int32_t>())
			writer.Int(var.to_int32());
		else if (t == type::get<int64_t>())
			writer.Int64(var.to_int64());
		else if (t == type::get<uint8_t>())
			writer.Uint(var.to_uint8());
		else if (t == type::get<uint16_t>())
			writer.Uint(var.to_uint16());
		else if (t == type::get<uint32_t>())
			writer.Uint(var.to_uint32());
		else if (t == type::get<uint64_t>())
			writer.Uint64(var.to_uint64());
		else if (t == type::get<float>())
			writer.Double(var.to_double());
		else if (t == type::get<double>())
			writer.Double(var.to_double());

		return true;
	}
	else if (t.is_enumeration())
	{
		bool ok = false;
		auto result = var.to_string(&ok);
		if (ok)
		{
		//	writer.String(var.to_string());
		}
		else
		{
			ok = false;
			auto value = var.to_uint64(&ok);
			if (ok)
				writer.Uint64(value);
			else
				writer.Null();
		}

		return true;
	}
	else if (t == type::get<std::string>())
	{
		//writer.String(var.to_string());
		return true;
	}

	return false;
}
namespace BE
{
	namespace Serialisation
	{
		inline void LoadEntity(const rapidjson::Value& curr)
		{
			BEID id = ECS->CreateEntity();
			auto map = ECS->GetAllComponentTypes();
			//For each registered component check if the entity has that component
			for (const auto& component : map)
			{
				//Skips the "class " part of string
				const char* type = component.second.c_str() + 6;
				if (curr.HasMember(type))
				{
					//Get the member and call the load object function of that component with the member as a arguement
					//const rapidjson::Value& v2 = curr[type];
					//ISerializable& is = engine.AddSerializedComponent(component.first.c_str(), id);
					//is.LoadObject(v2);
				}
			}
			////If the current entity has a behaviour attached to it
			//if (curr.HasMember("Behaviour"))
			//{
			//	//Find the name of the behaviour and attach the correct behaviour to the entity
			//	const rapidjson::Value& v2 = curr["Behaviour"];
			//	std::string temp{ "class " };
			//	temp.append(v2.FindMember("NameOfThisBehaviour")->value.GetString());
			//	engine.AddBehaviour(temp, id);
			//	//Call the load object on this behaviour passing the current json object to the function
			//	std::shared_ptr<IBehaviour> bptr;
			//	engine.GetBehaviour(id, bptr);
			//	dynamic_cast<ISerializable*>(bptr.get())->LoadObject(v2);
			//}
			////If the current entity has systems it is enrolled to
			//if (curr.HasMember("Systems"))
			//{
			//	//Enroll it into all the systems that the json contains
			//	for (rapidjson::Value::ConstValueIterator sys = curr["Systems"].GetArray().Begin(); sys != curr["Systems"].GetArray().End(); ++sys)
			//	{
			//		std::string str("class ");
			//		str.append(sys->GetString());
			//		engine.EnrollIntoSystem(id, str);
			//	}
			//}
		}

		void LoadEntities(const std::string& fileName)
		{
			UNREFERENCED_PARAMETER(fileName);
			rapidjson::Value temp;
			LoadEntity(temp);
		}

		void SaveEntities(const std::string& output)
		{
			//Create a output stream to override the output file
			std::ofstream ofs(output, std::ofstream::out | std::ofstream::trunc);
			using namespace rapidjson;
			//Create a document to store the json
			Document d;
			//Set the root of the json and create temporary variables
			Value& rootValue = d.SetObject();
			Value entityArray, entityObj, entityMember, entityMemberKey, entityBehaviourName, entitySystemName;
			entityArray.SetArray();
			auto componentTypes = ECS->GetAllComponentTypes();
			for (BEID id : ECS->GetAllEntities())
			{
				entityObj.SetObject();
				for (auto itr: componentTypes)
				{
					if (ECS->HasComponent(itr.second, id))
					{
						entityMember.SetObject();
						entityMemberKey.SetString(itr.second.c_str(), static_cast<SizeType>(itr.second.length()),d.GetAllocator());
						ECS->GetComponent(itr.second, id).SaveObject(entityMember,d);
						entityObj.AddMember(entityMemberKey, entityMember, d.GetAllocator());
					}
				}
				entityMember.SetObject();
				entityMemberKey.SetString(S_EntityID.c_str(), static_cast<SizeType>(S_EntityID.length()));
				entityMember.SetUint64(id);
				entityObj.AddMember(entityMemberKey, entityMember, d.GetAllocator());
				entityArray.PushBack(entityObj, d.GetAllocator());
			}
			rootValue.AddMember("entities", entityArray, d.GetAllocator());
			OStreamWrapper osw(ofs);
			//Create a writer to write the document to the output stream
			PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
			d.Accept(writer);
			ofs.close();
#if 0
			auto map = engine.GetComponentMap();
			//Serialize all registered entities that want to be saved
			for (eID id : engine.GetSystem<sIdentifierSystem>().entities)
			{
				entityObj.SetObject();
				//Check what components this entity has attached to it and call the save object function
				for (const auto& component : map)
				{

					auto pair = engine.GetSerializedComponent(component.first, id);
					if (pair.first)
					{
						entityMember.SetObject();
						entityMemberKey.SetString(component.first.c_str() + 6, static_cast<SizeType>(component.first.length() - 6));
						pair.second->SaveObject(entityMember, d);
						entityObj.AddMember(entityMemberKey, entityMember, d.GetAllocator());
					}
				}
				//Check if a behaviour is attached, if attached call save object on the behaviour
				std::shared_ptr<IBehaviour> temp;
				if (engine.GetBehaviour(id, temp))
				{

					ISerializable* serializableInterface = dynamic_cast<ISerializable*>(temp.get());
					if (serializableInterface)
					{
						entityMember.SetObject();
						entityMemberKey.SetString("Behaviour", d.GetAllocator());
						const char* behaviourName = typeid(*temp).name() + 6;
						entityBehaviourName.SetString(behaviourName, static_cast<SizeType>(strlen(behaviourName)));
						entityMember.AddMember("NameOfThisBehaviour", entityBehaviourName, d.GetAllocator());
						serializableInterface->SaveObject(entityMember, d);
						entityObj.AddMember(entityMemberKey, entityMember, d.GetAllocator());
					}
				}
				//Get a vector of strings that contains the name of the system entity is enrolled into
				auto vector = engine.GetEntityEnrollmentStatus(id);
				entityMember.SetArray();
				//Save the strings into the json
				entityMemberKey.SetString("Systems", d.GetAllocator());
				for (auto itr : vector)
				{
					entitySystemName.SetString(itr + 6, static_cast<SizeType>(strlen(itr + 6)));
					entityMember.PushBack(entitySystemName, d.GetAllocator());
				}
				entityObj.AddMember(entityMemberKey, entityMember, d.GetAllocator());
				entityArray.PushBack(entityObj, d.GetAllocator());
			}
			rootValue.AddMember("entities", entityArray, d.GetAllocator());
			OStreamWrapper osw(ofs);
			//Create a writer to write the document to the output stream
			PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
			d.Accept(writer);
			ofs.close();
#endif
		}
	}
}

