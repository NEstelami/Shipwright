#include "../Text.h"
#include "Utils/BinaryReader.h"

namespace Ship
{
    class TextFactory
    {
    public:
        static Text* ReadText(BinaryReader* reader);
        static Text* ReadTextXML(tinyxml2::XMLElement* reader);
    };
}