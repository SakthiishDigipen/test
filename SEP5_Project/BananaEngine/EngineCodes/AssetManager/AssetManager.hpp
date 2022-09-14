#pragma once

namespace BE
{
	template <typename AssetType, typename... CtorArgs>
	AssetLoadStatus AssetManager::LoadAsset(AssetID _assetID, const std::string& _filePath, CtorArgs&&... _args)
	{
		//Converts AssetType to its Asset Type ID
		AssetTypeID aType = GetAssetTypeID<AssetType>();
		AssertCore(m_assetTypesList.count(aType) != 0, "Asset Type \"%s\" was not registered", typeid(AssetType).name());

		//See if the asset ID already exists
		if (IsAssetLoaded(_assetID))
		{
			LogCoreError("Duplicate Asset ID %llu found, asset not created", _assetID);
			return AssetLoadStatus::exist;
		}

		//See if the file path has already been loaded
		if (IsAssetLoaded(_filePath))
		{
			LogCoreError("Already loaded the asset \"%s\"", _filePath.c_str());
			return AssetLoadStatus::exist;
		}

		try
		{
			//Attempt to create the asset data
			AssetType data( _filePath, std::forward<CtorArgs>(_args)... );

			//Convert filepath to using /
			std::string fp{ _filePath };
			ConvertFilePathSlash(fp);

			//Update the mapping
			m_assetID_to_fileName.emplace(_assetID, fp);
			m_fileName_to_assetID.emplace(fp, _assetID);
			m_assetID_to_type.emplace(_assetID, aType);

			//Move data into the m_data container
			GetAllAssetsOfType<AssetType>()->InsertData(_assetID, std::move(data));

			return AssetLoadStatus::ok;
		}
		//If asset loading fail, throw an exception in AssetType's constructor
		catch (const std::exception& e)
		{
			LogCoreError("Failed to create the asset \"%s\": %s.", _filePath.c_str(), e.what());
			return AssetLoadStatus::fail;
		}

	}

	template <typename AssetType, typename... CtorArgs>
	AssetLoadStatus AssetManager::LoadAsset(const std::string& _filePath, CtorArgs&&... _args)
	{
		//Converts AssetType to its Asset Type ID
		AssetTypeID aType = GetAssetTypeID<AssetType>();
		AssertCore(m_assetTypesList.count(aType) != 0, "Asset Type \"%s\" was not registered", typeid(AssetType).name());

		//Generate a random ID
		AssetID aID{};
		return LoadAsset<AssetType>(aID, _filePath, std::forward<CtorArgs>(_args)...);
	}

	template <typename AssetType>
	AssetType& AssetManager::GetAsset(AssetID _assetID)
	{
		return GetAllAssetsOfType<AssetType>()->GetData(_assetID);
	}

	template <typename AssetType>
	AssetType& AssetManager::GetAsset(const std::string& _fileName)
	{
		return GetAsset<AssetType>(GetAssetID(_fileName));
	}

	template <typename AssetType>
	bool AssetManager::IsAssetLoadedForType(AssetID _assetID) 
	{
		//Check if the asset ID even exists in the global asset list first
		if (IsAssetLoaded(_assetID))
		{
			//Converts AssetType to its Asset Type ID
			AssetTypeID aType = GetAssetTypeID<AssetType>();
			AssertCore(m_assetTypesList.count(aType) != 0, "Asset Type \"%s\" was not registered", typeid(AssetType).name());

			//Get the container that the asset data is stored in
			AssetDataContainer<AssetType>* dataCont = dynamic_cast<AssetDataContainer<AssetType>*>(m_assetData.at(aType));
			return dataCont->m_assetIDToIndexMap.count(_assetID) != 0;
		}
		else
			return false;

	}

	template <typename AssetType>
	bool AssetManager::IsAssetLoadedForType(const std::string& _fileName) 
	{
		//Check if the file name even exists in the global asset list first
		if (IsAssetLoaded(_fileName))
		{
			std::string fp = _fileName;
			ConvertFilePathSlash(fp);

			//Get the asset ID for this file name
			AssetID aID = m_fileName_to_assetID.at(fp);
			return IsAssetLoadedForType<AssetType>(aID);
		}
		else
			return false;
	}

	template <typename AssetType>
	AssetDataContainer<AssetType>* AssetManager::GetAllAssetsOfType()
	{
		AssetTypeID aType = GetAssetTypeID<AssetType>();
		AssertCore(m_assetTypesList.count(aType) != 0, "Asset Type \"%s\" was not registered", typeid(AssetType).name());
		return dynamic_cast<AssetDataContainer<AssetType>*>(m_assetData.at(aType));
	}

	template <typename AssetType>
	typename AssetManager::AssetTypeID AssetManager::RegisterAssetType()
	{
		static_assert(std::is_move_assignable<AssetType>::value, "Static Assertion failed: Asset Type must be move assignment - T& operator=(T&&)");
		static_assert(std::is_move_constructible<AssetType>::value, "Static Assertion failed: Asset Type must be move constructible - T(T&&)");

		AssetTypeID aType = GetAssetTypeID<AssetType>();
		AssertCore(m_assetTypesList.count(aType) == 0, "Asset Type \"%s\" was already registered", typeid(AssetType).name());

		//Truncating the namespace and class/struct from the type name
		std::string aName{ typeid(AssetType).name() };
		size_t i = aName.find_last_of(" :");

		//Store the asset ID - asset type name, and assetID - asset data array mapping
		m_assetTypesList.emplace(aType, aName.substr(i + 1, aName.size() - i - 1));
		m_assetData.emplace(aType, new AssetDataContainer<AssetType>);

		return aType;
	}

	template <typename AssetType>
	typename AssetManager::AssetTypeID AssetManager::GetAssetTypeID()
	{
		static AssetTypeID newID = m_nextID++;
		return newID;
	}
}