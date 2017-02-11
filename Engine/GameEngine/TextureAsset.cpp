#include "Core.h"
#include "TextureAsset.h"
#include <SFML\Graphics.hpp>

IMPLEMENT_DYNAMIC_CLASS(TextureAsset);

void TextureAsset::load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument)
{
	Asset::load(fileElement, metaDocument);

	texture = new sf::Texture();
	THROW_RUNTIME_ERROR(texture->loadFromFile(rawAssetPath.c_str()) == false, "Unable to load: " + rawAssetPath);
}

