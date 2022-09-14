#include "pch.h"
#include "AssetManager.h"

namespace BE
{
	AssetManager::AssetManager()
	{
		RegisterAssetType<TestFile>();

		EventMgr->SubscribeWPrio(SceneUnloadedEvent, this, AssetManager::OnSceneUnload_Part1, 2);
		EventMgr->SubscribeWPrio(SceneUnloadedEvent, this, AssetManager::OnSceneUnload_Part2, 0);

		LogCore("Created AssetManager System");
	}

	AssetManager::~AssetManager()
	{
		//Free all the containers
		for (const auto& container : m_assetData)
			delete container.second;

		LogCore("Destroyed AssetManager System");
	}



	void AssetManager::UnloadAsset(AssetID _assetID, bool _forceRemove)
	{
		//If asset doesn't exist, then don't do anything
		if (!IsAssetLoaded(_assetID))
		{
			LogCoreWarning("AssetID %llu was not loaded, cannot unload.", _assetID);
			return;
		}

		//Check if asset is still being used by any entity of UI
		if (!_forceRemove)
		{
			bool inUse = false;
			/*
				To do: check if asset id is used (use aID.first). If used, then just continue
			*/


			if (inUse)
				return;
		}


		//Get the type of asset is it from
		AssetTypeID aType = m_assetID_to_type.at(_assetID);

		//Remove assetID's data from the container
		m_assetData.at(aType)->RemoveData(_assetID);

		//Remove assetID from the mappings
		const std::string& fileName = m_assetID_to_fileName.at(_assetID);
		m_fileName_to_assetID.erase(fileName);
		m_assetID_to_fileName.erase(_assetID);
		m_assetID_to_type.erase(_assetID);
	}

	void AssetManager::UnloadAllAssets(bool _forceRemove)
	{
		auto begin = m_assetID_to_fileName.begin();
		auto end = m_assetID_to_fileName.end();

		while (begin != end)
		{
			UnloadAsset((begin++)->first, _forceRemove);
		}
	}

	void AssetManager::UnloadUnusedAssets()
	{
		//Unload all assets not being used by any entity or UI
		this->UnloadAllAssets(false);
	}

	bool AssetManager::IsAssetLoaded(AssetID _assetID) const
	{
		return m_assetID_to_fileName.count(_assetID) != 0;
	}

	bool AssetManager::IsAssetLoaded(const std::string& _fileName) const
	{
		std::string fp = _fileName;
		ConvertFilePathSlash(fp);
		return m_fileName_to_assetID.count(fp);
	}

	AssetID AssetManager::GetAssetID(const std::string& _fileName) const
	{
		//Convert to / slash
		std::string fp = _fileName;
		ConvertFilePathSlash(fp);

		//If file name is not found, then return 0
		if (m_fileName_to_assetID.count(fp) == 0)
		{
			LogCoreError("No asset with file name \"%s\" is loaded, returning NULL_ASSET instead", _fileName.c_str());
			return NULL_ASSET;
		}
		else
			return m_fileName_to_assetID.at(fp);
	}

	const std::string& AssetManager::GetAssetFileName(AssetID _assetID) const
	{
		//If file name is not found, then return 0
		if (m_assetID_to_fileName.count(_assetID) == 0)
		{
			LogCoreError("No asset with ID %llu is loaded, returning empty string instead", _assetID);
			static std::string errString{};
			return errString;
		}
		else
			return m_assetID_to_fileName.at(_assetID);
	}

	std::string AssetManager::GetAssetTruncatedFileName(AssetID _assetID) const
	{
		std::string result = GetAssetFileName(_assetID);
		TruncateFilePath(result);
		return result;
	}

	const std::string& AssetManager::GetAssetTypeOf(AssetID _assetID) const
	{
		return m_assetTypesList.at(this->GetAssetTypeIDOf(_assetID));
	}

	typename AssetManager::AssetTypeID AssetManager::GetAssetTypeIDOf(AssetID _assetID) const
	{
		AssertCore(m_assetID_to_type.count(_assetID) != 0, "AssetID %llu does not exist", _assetID);
		return m_assetID_to_type.at(_assetID);
	}

	void AssetManager::Serialise()
	{

	}

	void AssetManager::Deserialise()
	{

	}

	void AssetManager::OnSceneUnload_Part1(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneUnloaded, "Wrong event, expected SceneUnloadedEvent, passed in %s instead.", e.GetEventName());

		//To do: Load loading scene entities here by reading the scene file
		SceneUnloadedEvent& sul = dynamic_cast<SceneUnloadedEvent&>(e);
		sul.GetLoadingScene().GetSceneFile();

		/*
			Read scene file here - load loading screen assets
		*/
	}

	void AssetManager::OnSceneUnload_Part2(IEvent& e)
	{
		AssertCore(e.GetEventType() == EventType::sceneUnloaded, "Wrong event, expected SceneUnloadedEvent, passed in %s instead.", e.GetEventName());
		
		//Unload assets from previous scene
		UnloadUnusedAssets();

		//To do: Load loading scene entities here by reading the scene file
		SceneUnloadedEvent& sul = dynamic_cast<SceneUnloadedEvent&>(e);
		sul.GetNextScene().GetSceneFile();

		/*
			Read scene file here - load next scene assets, try make this part be async loading
		*/

		//Flag that assets have been loaded, ECS will then start loading entities for this scene
		AssetsLoadedEvent al(sul.GetNextScene());
		EventMgr->Dispatch(al);

		//Unload assets from loading scene
		UnloadUnusedAssets();

		//Flag that the scene has finished loading
		SceneLoadedEvent sl;
		EventMgr->Dispatch(sl);
	}
}