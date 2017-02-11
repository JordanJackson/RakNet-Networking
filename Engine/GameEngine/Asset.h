#ifndef _ASSET_H_
#define _ASSET_H_
#pragma once

#include "Object.h"

class Asset : public Object
{
	DECLARE_ABSTRACT_DERIVED_CLASS(Asset, Object)

protected:
	STRCODE FID;
	STRCODE UID;
	std::string rawAssetPath;

public:
	STRCODE getAssetId() { return UID; }
	void setFID(STRCODE fileID) { FID = fileID; };
	void setUID(STRCODE uniqueID) { UID = uniqueID; };

protected:
	virtual void load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument);

	friend class AssetManager;
};

#endif

