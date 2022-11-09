#include "SkeletonLimbFactory.h"

namespace Ship
{
    SkeletonLimb* SkeletonLimbFactory::ReadSkeletonLimb(BinaryReader* reader)
    {
        SkeletonLimb* limb = new SkeletonLimb();

        Version version = (Version)reader->ReadUInt32();

        switch (version)
        {
        case Version::Deckard:
        {
            SkeletonLimbV0 Limb = SkeletonLimbV0();
            Limb.ParseFileBinary(reader, limb);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return limb;
    }
    SkeletonLimb* SkeletonLimbFactory::ReadSkeletonLimbXML(tinyxml2::XMLElement* reader)
    {

        SkeletonLimb* skelLimb = new SkeletonLimb();

        Version version = (Version)reader->IntAttribute("Version");

        switch (version)
        {
        case Version::Deckard:
        {
            SkeletonLimbV0 skelLimbFac = SkeletonLimbV0();
            skelLimbFac.ParseFileXML(reader, skelLimb);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return skelLimb;
    }
}