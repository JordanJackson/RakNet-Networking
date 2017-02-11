#include "Core.h"
#include "AudioAsset.h"

IMPLEMENT_DYNAMIC_CLASS(AudioAsset);

void AudioAsset::load(XMLElement* fileElement, tinyxml2::XMLDocument& metaDocument)
{
	THROW_RUNTIME_ERROR(true, "Audio Asset is not Supoprted");
}
