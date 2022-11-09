#include "../Skeleton.h"
#include "Utils/BinaryReader.h"

namespace Ship
{
    class SkeletonFactory
    {
    public:
        static Skeleton* ReadSkeleton(BinaryReader* reader);
        static Skeleton* ReadSkeletonXML(tinyxml2::XMLElement* reader);
    };
}