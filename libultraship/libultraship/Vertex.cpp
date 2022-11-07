#include "Vertex.h"

namespace Ship
{
	void VertexV0::ParseFileBinary(BinaryReader* reader, Resource* res) {
		Vertex* vtx = (Vertex*)res;
		ResourceFile::ParseFileBinary(reader, res);

		uint32_t count = reader->ReadUInt32();
		vtx->vtxList.reserve(count);

		for (uint32_t i = 0; i < count; i++) {
			Vtx data;
			data.x = reader->ReadInt16();
			data.y = reader->ReadInt16();
			data.z = reader->ReadInt16();
			data.flag = reader->ReadUInt16();
			data.s = reader->ReadInt16();
			data.t = reader->ReadInt16();
			data.r = reader->ReadUByte();
			data.g = reader->ReadUByte();
			data.b = reader->ReadUByte();
			data.a = reader->ReadUByte();
			vtx->vtxList.push_back(data);
		}
	}
	void VertexV0::ParseFileXML(tinyxml2::XMLElement* reader, Resource* res)
	{
		Vertex* vtx = (Vertex*)res;

		auto child = reader->FirstChildElement();

		while (child != nullptr)
		{
			std::string childName = child->Name();

			if (childName == "Vtx")
			{
				Vtx data;
				data.x = child->IntAttribute("X");
				data.y = child->IntAttribute("Y");
				data.z = child->IntAttribute("Z");
				data.flag = 0;
				data.s = child->IntAttribute("S");
				data.t = child->IntAttribute("T");
				data.r = child->IntAttribute("R");
				data.g = child->IntAttribute("G");
				data.b = child->IntAttribute("B");
				data.a = child->IntAttribute("A");

				vtx->vtxList.push_back(data);
			}

			child = child->NextSiblingElement();
		}

		int bp = 0;
	}
}