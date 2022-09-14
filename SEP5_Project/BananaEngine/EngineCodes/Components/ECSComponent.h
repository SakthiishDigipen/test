#pragma once
#ifndef ECSCOMPONENT_H
#define ECSCOMPONENT_H
#include <Precompiled/pch.h>
#include <Systems/ECS/ECSConfig.h>
//#include <Serialisation/BananaSerialisation.h>
namespace BE
{
	//Base class that all components need to inherit
	class IECSComponent
	{
	protected:
		IECSComponent() = default;
		virtual ~IECSComponent() = default;

	public:
		virtual void LoadObject(const rapidjson::Value& v) = 0;
		virtual void SaveObject(rapidjson::Value&, rapidjson::Document&) = 0;
	};
}

#endif