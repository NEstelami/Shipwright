#include "Skeleton.h"

namespace Ship
{
    void SkeletonV0::ParseFileBinary(BinaryReader* reader, Resource* res)
    {
        Skeleton* skel = (Skeleton*)res;
        
        ResourceFile::ParseFileBinary(reader, skel);

        skel->type = (SkeletonType)reader->ReadInt8();
        skel->limbType = (LimbType)reader->ReadInt8();

        skel->limbCount = reader->ReadUInt32();
        skel->dListCount = reader->ReadUInt32();

        skel->limbTableType = (LimbType)reader->ReadInt8();

        uint32_t limbTblCnt = reader->ReadUInt32();
        skel->limbTable.reserve(limbTblCnt);

        for (uint32_t i = 0; i < limbTblCnt; i++)
        {
            std::string limbPath = reader->ReadString();

            skel->limbTable.push_back(limbPath);
        }
    }

    void SkeletonV0::ParseFileXML(tinyxml2::XMLElement* reader, Resource* res)
    {
        Skeleton* skel = (Skeleton*)res;

        std::string skeletonType = reader->Attribute("Type");
        //std::string skeletonLimbType = reader->Attribute("LimbType");
        int numLimbs = reader->IntAttribute("LimbCount");
        int numDLs = reader->IntAttribute("DisplayListCount");

        if (skeletonType == "Flex")
            skel->type = SkeletonType::Flex;
        else if (skeletonType == "Curve")
            skel->type = SkeletonType::Curve;
        else if (skeletonType == "Normal")
            skel->type = SkeletonType::Normal;

        skel->type = SkeletonType::Flex;
        skel->limbType = LimbType::LOD;

        //if (skeletonLimbType == "Standard")
            //skel->limbType = LimbType::Standard;
        //else if (skeletonLimbType == "LOD")
            //skel->limbType = LimbType::LOD;
        //else if (skeletonLimbType == "Curve")
            //skel->limbType = LimbType::Curve;
        //else if (skeletonLimbType == "Skin")
            //skel->limbType = LimbType::Skin;
        //else if (skeletonLimbType == "Legacy")
            //Sskel->limbType = LimbType::Legacy;

        auto child = reader->FirstChildElement();

        skel->limbCount = numLimbs;
        skel->dListCount = numDLs;

        while (child != nullptr)
        {
            std::string childName = child->Name();

            if (childName == "SkeletonLimb")
            {
                std::string limbName = child->Attribute("Path");
                skel->limbTable.push_back(limbName);
            }

            child = child->NextSiblingElement();
        }
    }
}