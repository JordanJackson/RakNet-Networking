#ifndef _FONTASSET_H_
#define _FONTASSET_H_

#pragma once
#include "Asset.h"

namespace sf
{
	class Font;
};

class FontAsset : public Asset
{
public:
	DECLARE_DYNAMIC_DERIVED_CLASS(FontAsset, Asset)

public:
	sf::Font* getFont() { return font; }

protected:
	void load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument) override;


private:
	sf::Font* font;
};


#endif _FONTASSET_H_

