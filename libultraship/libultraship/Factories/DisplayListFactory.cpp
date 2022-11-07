#include "DisplayListFactory.h"

namespace Ship
{
    DisplayList* DisplayListFactory::ReadDisplayList(BinaryReader* reader)
    {
        DisplayList* dl = new DisplayList();

        Version version = (Version)reader->ReadUInt32();

        switch (version)
        {
        case Version::Deckard:
        {
            DisplayListV0 DL = DisplayListV0();
            DL.ParseFileBinary(reader, dl);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return dl;

    }
    DisplayList* DisplayListFactory::ReadDisplayListXML(tinyxml2::XMLElement* reader)
    {
        DisplayList* dl = new DisplayList();

        Version version = (Version)reader->IntAttribute("Version");

        switch (version)
        {
        case Version::Deckard:
        {
            DisplayListV0 dlFac = DisplayListV0();
            dlFac.ParseFileXML(reader, dl);
        }
        break;
        default:
            // VERSION NOT SUPPORTED
            break;
        }

        return dl;
    }
}