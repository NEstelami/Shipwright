#pragma once

#include <vector>
#include <string>
#include "Resource.h"
#include "Vec2f.h"
#include "Vec3f.h"
#include "Color3b.h"

namespace Ship
{
	class DisplayListV0 : public ResourceFile
	{
	public:
		void ParseFileBinary(BinaryReader* reader, Resource* res) override;
		void ParseFileXML(tinyxml2::XMLElement* reader, Resource* res) override;

		uint32_t GetCombineLERPValue(std::string valStr);
	};

    class DisplayList : public Resource
    {
    public:
		std::vector<uint64_t> instructions;
    };
}