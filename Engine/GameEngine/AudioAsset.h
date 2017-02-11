#ifndef _AUDIOASSET_H_
#define _AUDIOASSET_H_

#pragma once
#include "Asset.h"

class AudioAsset : public Asset
{
public:
	DECLARE_DYNAMIC_DERIVED_CLASS(AudioAsset, Asset)

protected:
	void load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument) override;

private:
};


#endif _AUDIOASSET_H_

