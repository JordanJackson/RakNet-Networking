#include "Core.h"
#include "PrefabAsset.h"
#include "GameObjectManager.h"

IMPLEMENT_DYNAMIC_CLASS(PrefabAsset);

void PrefabAsset::load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument)
{
	Asset::load(fileElement, metaDocument);

	prefabDoc = new tinyxml2::XMLDocument();
	prefabDoc->LoadFile(rawAssetPath.c_str());
	THROW_RUNTIME_ERROR(prefabDoc->ErrorID() != XML_SUCCESS, "Unable to Load Prefab");
}

GameObject* PrefabAsset::CreatePrefab()
{
	return GameObjectManager::Instance().CreateGameObject(prefabDoc);
}