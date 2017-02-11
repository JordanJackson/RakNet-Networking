#include "Core.h"
#include "AssetManager.h"
#include "TextureAsset.h"
#include "FontAsset.h"
#include "PrefabAsset.h"

void AssetManager::initialize()
{
	ISystem::initialize();

	REGISTER_ABSTRACT_CLASS(Asset);
	REGISTER_DYNAMIC_CLASS(TextureAsset);
	REGISTER_DYNAMIC_CLASS(FontAsset);
	REGISTER_DYNAMIC_CLASS(PrefabAsset);
}

// The asset node is passed with FileID. The asset object shared ptr is created as per class name 
// and the data is loaded from meta file. Then the asset is added to map. 
void AssetManager::load(XMLElement* element, STRCODE fileID)
{
	LOG("Loading Assets " << fileID);

	XMLElement* assetElement = element->FirstChildElement("Asset");
	while (assetElement != NULL)
	{
		std::string rawAssetfile = assetElement->GetText();
		std::size_t found = rawAssetfile.find_last_of(".");
		std::string metaFileName = rawAssetfile.substr(0, found) + ".meta";

		// Loading Meta file and setting the UID of font asset
		tinyxml2::XMLDocument doc;
		doc.LoadFile(metaFileName.c_str());
		THROW_RUNTIME_ERROR(doc.ErrorID() != XML_SUCCESS, "Unable to Load asset: " + metaFileName);

		XMLElement* uuidElement = doc.FirstChildElement("UID");
		THROW_RUNTIME_ERROR(uuidElement == nullptr, "Unable to Load asset: " + metaFileName);
		STRCODE UID = getHashCode(uuidElement->GetText());

		XMLElement* typeElement = doc.FirstChildElement("Type");
		THROW_RUNTIME_ERROR(typeElement == nullptr, "Unable to Load asset: " + metaFileName);
		std::string assetType = typeElement->GetText();

		Asset* asset = ((Asset*)CreateObject(assetType.c_str()));
		THROW_RUNTIME_ERROR(asset == nullptr, "Unable to Create Asset: " + assetType);

		asset->setFID(fileID);
		asset->setUID(UID);
		asset->load(assetElement, doc);
		asset->initialize();

		assets[asset->UID] = asset;

		assetElement = assetElement->NextSiblingElement("Asset");
	}
}

// The fileID is passed and every Asset that has the fileID is erased.
void AssetManager::unload(STRCODE fileID)
{
	for (auto it = assets.cbegin(); it != assets.cend(); /* no increment */)
	{
		if ((*it).second->FID == fileID)
		{
			delete (*it).second;
			assets.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

// The Asset UID's hashcode is passed to function and it search the map for the UID and returns corresponding asset shared ptr
// Takes in hashcode of actual UID as parameter
Asset* AssetManager::getAsset(std::string AssetUID, STRCODE Type)
{
	return getAsset(getHashCode(AssetUID.c_str()), Type);
}

Asset* AssetManager::getAsset(STRCODE assetUID, STRCODE Type)
{
	std::map<STRCODE, Asset*>::iterator it = assets.find(assetUID);
	if (it != assets.end())
	{
		if (Type == NoName)
		{
			return (*it).second;
		}
		else if ((*it).second->isA(Type))
		{
			return (*it).second;
		}
	}
	return nullptr;
}

