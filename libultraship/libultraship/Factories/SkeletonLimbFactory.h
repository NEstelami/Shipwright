#include "../SkeletonLimb.h"
#include "Utils/BinaryReader.h"

namespace Ship
{
    class SkeletonLimbFactory
    {
    public:
        static SkeletonLimb* ReadSkeletonLimb(BinaryReader* reader);
        static SkeletonLimb* ReadSkeletonLimbXML(tinyxml2::XMLElement* reader);
    };
}