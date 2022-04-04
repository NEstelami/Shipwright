#include "TextFactory.h"

namespace Ship
{
    Text* TextFactory::ReadText(BinaryReader* reader)
    {
        Text* txt = new Text();

        Version version = (Version)reader->ReadUInt32();

        switch (version)
        {
        case Version::Deckard:
        {
            TextV0 txtFac = TextV0();
            txtFac.ParseFileBinary(reader, txt);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return txt;
    }

    Text* TextFactory::ReadTextXML(tinyxml2::XMLElement* reader)
    {
        Text* txt = new Text();

        Version version = (Version)reader->IntAttribute("Version");

        switch (version)
        {
        case Version::Deckard:
        {
            TextV0 txtFac = TextV0();
            txtFac.ParseFileXML(reader, txt);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return txt;
    }
}