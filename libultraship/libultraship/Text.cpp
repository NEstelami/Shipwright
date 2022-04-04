#include "Text.h"

void Ship::TextV0::ParseFileXML(tinyxml2::XMLElement* reader, Resource* res)
{
	Text* txt = (Text*)res;

	auto child = reader->FirstChildElement();

	while (child != nullptr)
	{
		std::string childName = child->Name();

		if (childName == "TextEntry")
		{
			MessageEntry entry;
			entry.id = child->IntAttribute("ID");
			entry.textboxType = child->IntAttribute("TextboxType");
			entry.textboxYPos = child->IntAttribute("TextboxYPos");
			entry.msg = child->Attribute("Message");

			txt->messages.push_back(entry);
			int bp = 0;
		}

		child = child->NextSiblingElement();
	}
}

void Ship::TextV0::ParseFileBinary(BinaryReader* reader, Resource* res)
{
	Text* txt = (Text*)res;

	ResourceFile::ParseFileBinary(reader, txt);

	uint32_t msgCount = reader->ReadUInt32();
	txt->messages.reserve(msgCount);

	for (uint32_t i = 0; i < msgCount; i++)
	{
		MessageEntry entry;
		entry.id = reader->ReadUInt16();
		entry.textboxType = reader->ReadUByte();
		entry.textboxYPos = reader->ReadUByte();
		entry.msg = reader->ReadString();

		txt->messages.push_back(entry);
	}
}
