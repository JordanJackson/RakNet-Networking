#ifndef _TEXTUREASSET_H_
#define _TEXTUREASSET_H_
#pragma once

#include "Asset.h"

namespace sf
{
	class Texture;
};

class TextureAsset : public Asset
{
public:
	DECLARE_DYNAMIC_DERIVED_CLASS(TextureAsset, Asset)

	sf::Texture* getTexture() { return texture; }

protected:
	void load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument) override;

private:
	sf::Texture* texture;
};

#endif

