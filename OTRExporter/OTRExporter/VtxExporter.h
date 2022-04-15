#pragma once

/* 
* Changelog:
* V0 (Deckard) - Initial implementation
*/


#include "ZResource.h"
#include "ZVtx.h"
#include "Exporter.h"
#include <Utils/BinaryWriter.h>

// Note: Ultimately this exporter goes *mostly* unused. Because of the lack of single vertex elements, the Array Exporter is used instead, 
// which calls our serialization function. So it remains here mostly as a placeholder and may be deleted altogether in the future.

class OTRExporter_Vtx : public OTRExporter
{
public:
	static void WriteVtx(ZVtx* vtx, BinaryWriter* writer);
	void SaveArr(ZResource* res, const fs::path& outPath, const std::vector<ZResource*>&, BinaryWriter* writer, bool writeHeader);
	virtual void Save(ZResource* res, const fs::path& outPath, BinaryWriter* writer, bool writeHeader) override;
};