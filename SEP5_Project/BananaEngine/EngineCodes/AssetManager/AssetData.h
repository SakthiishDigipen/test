#pragma once
#ifndef	ASSET_DATA_H
#define ASSET_DATA_H

#include <Precompiled/pch.h>
#include <Debugger/Debugger.h>

namespace BE
{
	//A unique identifier to the various asset data
	using AssetID = BEID;
	const AssetID NULL_ASSET = BEID(0);

	//Base class for storing asset data
	class IAssetDataContainer
	{
	protected:
		IAssetDataContainer() = default;
		virtual ~IAssetDataContainer() {}

		virtual void RemoveData(AssetID _aID) = 0;

		friend class AssetManager;
	};

	template <typename AssetType>
	class AssetDataContainer : public IAssetDataContainer
	{
		//Contains the actual asset data
		std::vector<AssetType> m_data{};

		//Maps between Asset ID and m_data index for actual data
		std::unordered_map<AssetID, size_t> m_assetIDToIndexMap{};
		std::unordered_map<size_t, AssetID> m_indexToAssetIDMap{};



		/*
			Constructor and Destructor
		*/
		AssetDataContainer();
		~AssetDataContainer();



		/*
			Data handling
		*/
		//Inserts data into the array
		void InsertData(AssetID _aID, AssetType&& _data);

		//Removes data from the array
		void RemoveData(AssetID _aID) override;

		//Retrieves data from the array
		AssetType& GetData(AssetID _aID);

		//Disabling copy constructor/assignment
		AssetDataContainer& operator=(const AssetDataContainer&) = delete;
		AssetDataContainer(const AssetDataContainer&) = delete;

		//Only asset manager can access the data here
		friend class AssetManager;
	};
}

#include "AssetData.hpp"

#endif