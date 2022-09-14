#pragma once
#ifndef	ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <Precompiled/pch.h>
#include <Utilities/Singleton/Singleton.h>
#include <Utilities/BEID/BEID.h>
#include <EventDispatcher/EventDispatcher.h>

#include "AssetData.h"

//Different file types here
#include "AssetTypes/TestFile.h"

namespace BE
{
	enum class AssetLoadStatus
	{
		ok = 0,	//Loaded successfully
		fail,	//Fails to load
		exist	//Already exists in the asset manager
	};


	//Asset manager
	class AssetManager : public Singleton<AssetManager>
	{
		/*
			Asset types tracking - Manages the types of IDs there are
		*/
		//An ID representing the asset file type
		using AssetTypeID = size_t;

		//For registering asset type
		AssetTypeID m_nextID = 0;
		
		//The types of assets in use
		std::unordered_map<AssetTypeID, std::string> m_assetTypesList{};



		/*
			Asset ID tracking - Manages the Asset IDs that are loaded into the engine
		*/

		//Asset's file name associated to Asset ID
		std::unordered_map<AssetID, std::string> m_assetID_to_fileName{};

		//Asset ID associated to Asset's file name
		std::unordered_map<std::string, AssetID> m_fileName_to_assetID{};

		//Asset Type ID associated to the Asset ID
		std::unordered_map<AssetID, AssetTypeID> m_assetID_to_type{};



		/*
			Asset data management - Manages the actual asset data
		*/
		std::unordered_map<AssetTypeID, IAssetDataContainer*> m_assetData;


		
		//Constructor and Destructor
		AssetManager();
		~AssetManager();



	public:
		/*
			Loading/Unloading assets
		*/
		//Loads asset into the asset manager with a known asset ID.
		template <typename AssetType, typename... CtorArgs>
		AssetLoadStatus LoadAsset(AssetID _assetID, const std::string& _filePath, CtorArgs&&... _args);

		//Loads asset into the asset manager with a randomly generated asset ID.
		template <typename AssetType, typename... CtorArgs>
		AssetLoadStatus LoadAsset(const std::string& _filePath, CtorArgs&&... _args);

		/*
			NOTE:
			- T is an asset type class that wraps the asset data used by the ECS components or UI.
			- T's constructor first parameter must be the asset's filepath (_filePath)
			- T's constructor subsequent parameters are _args, if any.
			- Do any specialised things (like loading sound data into fmod system) inside T's constructor.
			- If asset loading fails (eg: file not found), throw an std::exception in the constructor, Asset manager will catch it.
			- If the provided AssetID already exists, a new AssetID will be issued.
			- If an asset file has already been loaded, no new loading happens.
			- T should ideally be move constructible/assignable, there will be move construction/assignment used.
			- Make sure to put the correct T value for the type of asset you are loading
		*/

		//Unloads asset from asset manager
		void UnloadAsset(AssetID _assetID, bool _forceRemove = false);

		//Unload all assets
		void UnloadAllAssets(bool _forceRemove = false);

		//Unload assets that are not used by any entity or UI
		void UnloadUnusedAssets();

		/*
			NOTE:
			- Do any specialised things (like releasing fmod sound data) inside T's destructor.
			- _forceRemove means to remove the asset even if any ECS entity/UI are using it
			- Make sure to put the correct T value for the asset type you are unloading
		*/



		/*
			Assets data querying
		*/
		//Get the data of the asset 
		template <typename AssetType>
		AssetType& GetAsset(AssetID _assetID);

		template <typename AssetType>
		AssetType& GetAsset(const std::string& _fileName);

		//Check whether the asset is loaded for its type
		template <typename AssetType>
		bool IsAssetLoadedForType(AssetID _assetID);

		template <typename AssetType>
		bool IsAssetLoadedForType(const std::string& _fileName);

		//Get all the assets by types
		template <typename AssetType>
		AssetDataContainer<AssetType>* GetAllAssetsOfType();

		//Check whether the asset is loaded for any type - by asset ID
		bool IsAssetLoaded(AssetID _assetID) const;

		//Check whether the asset is loaded for any type - by file name
		bool IsAssetLoaded(const std::string& _fileName) const;




		/*
			Assets querying
		*/
		//Get asset ID using it's file name - compares the full file path
		AssetID GetAssetID(const std::string& _fileName) const;

		//Get asset (full) file path using it's asset ID.
		const std::string& GetAssetFileName(AssetID _assetID) const;

		//Get asset (truncated) file path using it's asset ID
		std::string GetAssetTruncatedFileName(AssetID _assetID) const;

		//Get the type of asset that asset ID is (get as string)
		const std::string& GetAssetTypeOf(AssetID _assetID) const;

		//Get the type of asset that asset ID is (get as ID)
		AssetTypeID GetAssetTypeIDOf(AssetID _assetID) const;

		/*
			Asset manager functions
		*/
		//Register a new type of asset
		template <typename AssetType>
		AssetTypeID RegisterAssetType();

		//Get the ID of the asset type
		template <typename AssetType>
		AssetTypeID GetAssetTypeID();

		//Get asset names
		inline const auto& GetFileTypes() const { return m_assetTypesList; }

		//Get all loaded asset IDs
		inline const auto& GetAllLoadedAssets() const { return m_assetID_to_fileName; }



		/*
			Additional specialised functions - like reloading scripts, convert textures to DDS etc
		*/
		//???
		


		/*
			Serialisation/Deserialisation of asset ID and the associated file name 
		*/
		void Serialise();
		void Deserialise();

		/*
			Event handling
		*/
		//Loads assets required for loading screen
		void OnSceneUnload_Part1(IEvent& e);

		//Unload assets from previous scene, and start loading assets sfor new scene
		void OnSceneUnload_Part2(IEvent& e);

		friend class Singleton<AssetManager>;
	};

	#define AssetMgr AssetManager::GetInstance()
}

#include "AssetManager.hpp"

#endif