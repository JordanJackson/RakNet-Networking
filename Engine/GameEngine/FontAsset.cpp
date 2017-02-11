#include "Core.h"
#include "FontAsset.h"
#include <SFML\Graphics.hpp>

IMPLEMENT_DYNAMIC_CLASS(FontAsset);

void FontAsset::load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument)
{
	Asset::load(fileElement, metaDocument);

	font = new sf::Font();
	THROW_RUNTIME_ERROR(font->loadFromFile(rawAssetPath.c_str()) == false, "Unable to load: " + rawAssetPath);
}