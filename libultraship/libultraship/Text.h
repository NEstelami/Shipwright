#pragma once

#include "Resource.h"

namespace Ship
{
	class MessageEntry
	{
	public:
		uint16_t id;
		uint8_t textboxType;
		uint8_t textboxYPos;
		std::string msg;
	};

	class TextV0 : public ResourceFile
	{
	public:
		void ParseFileXML(tinyxml2::XMLElement* reader, Resource* res) override;
		void ParseFileBinary(BinaryReader* reader, Resource* res) override;
	};

	class Text : public Resource
	{
	public:
		std::vector<MessageEntry> messages;
	};
}