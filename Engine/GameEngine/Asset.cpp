#include "Core.h"
#include "Asset.h"

IMPLEMENT_ABSTRACT_CLASS(Asset)

void Asset::load(XMLElement* assetElement, tinyxml2::XMLDocument& metaDocument)
{
	rawAssetPath = assetElement->GetText();
}
