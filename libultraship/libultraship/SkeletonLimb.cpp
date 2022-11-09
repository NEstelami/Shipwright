#include "SkeletonLimb.h"

namespace Ship
{
    void SkeletonLimbV0::ParseFileBinary(BinaryReader* reader, Resource* res)
    {
        SkeletonLimb* limb = (SkeletonLimb*)res;

        ResourceFile::ParseFileBinary(reader, limb);

        limb->limbType = (LimbType)reader->ReadInt8();
        limb->skinSegmentType = (ZLimbSkinType)reader->ReadInt8();
        limb->skinDList = reader->ReadString();

        limb->skinVtxCnt = reader->ReadUInt16();
        uint32_t skinCnt = reader->ReadUInt32();

        for (size_t i = 0; i < skinCnt; i++)
        {
            Struct_800A598C struc;

            struc.unk_4 = reader->ReadUInt16();

            int arrSize = reader->ReadInt32();

            for (int k = 0; k < arrSize; k++)
            {
                Struct_800A57C0 struc2;

                struc2.unk_0 = reader->ReadInt16();
                struc2.unk_2 = reader->ReadInt16();
                struc2.unk_4 = reader->ReadInt16();
                struc2.unk_6 = reader->ReadInt8();
                struc2.unk_7 = reader->ReadInt8();
                struc2.unk_8 = reader->ReadInt8();
                struc2.unk_9 = reader->ReadUByte();

                struc.unk_8_arr.push_back(struc2);
            }

            int arrSize2 = reader->ReadInt32();

            for (int k = 0; k < arrSize2; k++)
            {
                Struct_800A598C_2 struc2;

                struc2.unk_0 = reader->ReadUByte();
                struc2.x = reader->ReadInt16();
                struc2.y = reader->ReadInt16();
                struc2.z = reader->ReadInt16();
                struc2.unk_8 = reader->ReadUByte();

                struc.unk_C_arr.push_back(struc2);
            }

            limb->skinData.push_back(struc);
        }

        limb->skinDList2 = reader->ReadString();

        limb->legTransX = reader->ReadFloat();
        limb->legTransY = reader->ReadFloat();
        limb->legTransZ = reader->ReadFloat();
        
        limb->rotX = reader->ReadUInt16();
        limb->rotY = reader->ReadUInt16();
        limb->rotZ = reader->ReadUInt16();

        limb->childPtr = reader->ReadString();
        limb->siblingPtr = reader->ReadString();
        limb->dListPtr = reader->ReadString();
        limb->dList2Ptr = reader->ReadString();

        limb->transX = reader->ReadInt16();
        limb->transY = reader->ReadInt16();
        limb->transZ = reader->ReadInt16();

        limb->childIndex = reader->ReadUByte();
        limb->siblingIndex = reader->ReadUByte();
    }

    void SkeletonLimbV0::ParseFileXML(tinyxml2::XMLElement* reader, Resource* res)
    {
        SkeletonLimb* skelLimb = (SkeletonLimb*)res;

        std::string limbType = reader->Attribute("Type");
        
        // OTRTODO
        skelLimb->limbType = LimbType::LOD;

        //skelLimb->legTransX = reader->FloatAttribute("LegTransX");
        //skelLimb->legTransY = reader->FloatAttribute("LegTransY");
        //skelLimb->legTransZ = reader->FloatAttribute("LegTransZ");
        skelLimb->rotX = reader->IntAttribute("RotX");
        skelLimb->rotY = reader->IntAttribute("RotY");
        skelLimb->rotZ = reader->IntAttribute("RotZ");

        //skelLimb->transX = reader->IntAttribute("TransX");
        //skelLimb->transY = reader->IntAttribute("TransY");
        //skelLimb->transZ = reader->IntAttribute("TransZ");

        skelLimb->transX = (int)reader->FloatAttribute("LegTransX");
        skelLimb->transY = (int)reader->FloatAttribute("LegTransY");
        skelLimb->transZ = (int)reader->FloatAttribute("LegTransZ");

        skelLimb->childIndex = reader->IntAttribute("ChildIndex");
        skelLimb->siblingIndex = reader->IntAttribute("SiblingIndex");

        //skelLimb->childPtr = reader->Attribute("ChildLimb");
        //skelLimb->siblingPtr = reader->Attribute("SiblingLimb");
        skelLimb->dListPtr = reader->Attribute("DisplayList1");

        if (std::string(reader->Attribute("DisplayList1")) == "gEmptyDL")
            skelLimb->dListPtr = "";

        //skelLimb->dList2Ptr = reader->Attribute("DisplayList2");
    }
}