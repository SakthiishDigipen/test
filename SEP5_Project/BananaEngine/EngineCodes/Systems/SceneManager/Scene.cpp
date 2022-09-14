#include "pch.h"
#include "Scene.h"

namespace BE
{
	Scene::Scene(std::string&& _name, std::string&& _file, SceneID _id)
		: m_id(_id), m_sceneName(std::move(_name))
	{
		m_sceneFile = std::move(_file);
		ConvertFilePathSlash(m_sceneFile);
	}

	bool Scene::operator==(const Scene& _otherScene) const
	{
		return (m_id		== _otherScene.m_id			&&
				m_sceneName == _otherScene.m_sceneName  &&
				m_sceneFile == _otherScene.m_sceneFile);
	}

	bool Scene::operator!=(const Scene& _otherScene) const
	{
		return !(*this == _otherScene);
	}
}