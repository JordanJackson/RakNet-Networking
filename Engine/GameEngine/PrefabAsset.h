#ifndef _PREFABASSET_H_
#define _PREFABASSET_H_

#pragma once
#include "Asset.h"

class GameObject;

class PrefabAsset : public Asset
{
public:
	DECLARE_DYNAMIC_DERIVED_CLASS(PrefabAsset, Asset)

public:
	GameObject* CreatePrefab();
	void load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument) override;

private:
	tinyxml2::XMLDocument*	prefabDoc;
};


#endif

