#include "DisplayList.h"
#include "PR/ultra64/gbi.h"
#include <Lib/StrHash64.h>

namespace Ship
{
	void DisplayListV0::ParseFileBinary(BinaryReader* reader, Resource* res)
	{
		DisplayList* dl = (DisplayList*)res;

		ResourceFile::ParseFileBinary(reader, res);

		while (reader->GetBaseAddress() % 8 != 0)
			reader->ReadInt8();

		while (true)
		{
			uint32_t w0 = reader->ReadUInt32();
			uint32_t w1 = reader->ReadUInt32();

			if (sizeof(uintptr_t) < 8) {
				dl->instructions.push_back(((uint64_t)w0 << 32) | w1);

				uint8_t opcode = w0 >> 24;

				// These are 128-bit commands, so read an extra 64 bits...
				if (opcode == G_SETTIMG_OTR || opcode == G_DL_OTR || opcode == G_VTX_OTR || opcode == G_BRANCH_Z_OTR || opcode == G_MARKER || opcode == G_MTX_OTR) {
					w0 = reader->ReadUInt32();
					w1 = reader->ReadUInt32();

					dl->instructions.push_back(((uint64_t)w0 << 32) | w1);
				}

				if (opcode == G_ENDDL)
					break;
			}
			else {
				dl->instructions.push_back(w0);
				dl->instructions.push_back(w1);

				uint8_t opcode = (uint8_t)(w0 >> 24);

				if (opcode == G_SETTIMG_OTR || opcode == G_DL_OTR || opcode == G_VTX_OTR || opcode == G_BRANCH_Z_OTR || opcode == G_MARKER || opcode == G_MTX_OTR)
				{
					w0 = reader->ReadUInt32();
					w1 = reader->ReadUInt32();

					dl->instructions.push_back(w0);
					dl->instructions.push_back(w1);
				}

				if (opcode == G_ENDDL)
					break;
			}
		}
	}

	void DisplayListV0::ParseFileXML(tinyxml2::XMLElement* reader, Resource* res)
	{
		DisplayList* dl = (DisplayList*)res;

		auto child = reader->FirstChildElement();

		while (child != nullptr)
		{
			std::string childName = child->Name();

			Gfx g;

			if (childName == "PipeSync")
			{
				g = gsDPPipeSync();
			}
			else if (childName == "Texture")
			{
				g = gsSPTexture(child->IntAttribute("S"), child->IntAttribute("T"), child->IntAttribute("Level"),
					child->IntAttribute("Tile"), child->IntAttribute("On"));
			}
			else if (childName == "SetPrimColor")
			{
				g = gsDPSetPrimColor(child->IntAttribute("MinLevel"), child->IntAttribute("LodFrac"), child->IntAttribute("R"),
					child->IntAttribute("G"), child->IntAttribute("B"), child->IntAttribute("A"));
			}
			else if (childName == "SetCycleType")
			{
				uint32_t param = 0;

				if (child->Attribute("G_CYC_1CYCLE", 0))
					param |= G_CYC_1CYCLE;

				if (child->Attribute("G_CYC_2CYCLE", 0))
					param |= G_CYC_2CYCLE;

				if (child->Attribute("G_CYC_COPY", 0))
					param |= G_CYC_COPY;

				if (child->Attribute("G_CYC_FILL", 0))
					param |= G_CYC_FILL;

				g = gsDPSetCycleType(param);
			}
			else if (childName == "PipelineMode")
			{
				uint32_t param = 0;

				if (child->Attribute("G_PM_1PRIMITIVE", 0))
					param |= G_PM_1PRIMITIVE;

				if (child->Attribute("G_PM_NPRIMITIVE", 0))
					param |= G_PM_NPRIMITIVE;

				g = gsDPPipelineMode(param);
			}
			else if (childName == "TileSync")
			{
				g = gsDPTileSync();
			}
			else if (childName == "Triangle1")
			{
				int v00 = child->IntAttribute("V00");
				int v01 = child->IntAttribute("V01");
				int v02 = child->IntAttribute("V02");

				g = gsSP1TriangleOTR(child->IntAttribute("V00"), child->IntAttribute("V01"), child->IntAttribute("V02"), child->IntAttribute("Flag0"));
				g.words.w0 &= 0xFF000000;
				g.words.w0 |= v00;
				g.words.w1 |= v01 << 16;
				g.words.w1 |= v02 << 0;
			}
			else if (childName == "SetCombineLERP")
			{
				const char* a0 = child->Attribute("A0", 0);
				const char* b0 = child->Attribute("B0", 0);
				const char* c0 = child->Attribute("C0", 0);
				const char* d0 = child->Attribute("D0", 0);

				const char* aa0 = child->Attribute("Aa0", 0);
				const char* ab0 = child->Attribute("Ab0", 0);
				const char* ac0 = child->Attribute("Ac0", 0);
				const char* ad0 = child->Attribute("Ad0", 0);

				const char* a1 = child->Attribute("A1", 0);
				const char* b1 = child->Attribute("B1", 0);
				const char* c1 = child->Attribute("C1", 0);
				const char* d1 = child->Attribute("D1", 0);

				const char* aa1 = child->Attribute("Aa1", 0);
				const char* ab1 = child->Attribute("Ab1", 0);
				const char* ac1 = child->Attribute("Ac1", 0);
				const char* ad1 = child->Attribute("Ad1", 0);

				// OTRTODO: Figure out how to implement this without it looking like hell...
				//g = gsDPSetCombineLERP(a0, b0, c0, d0, aa0, ab0, ac0, ad0, a1, b1, c1, d1, aa1, ab1, ac1, ad1);
				
				// For now this is temp...
				g = gsDPSetCombineLERP(1, TEXEL0, PRIMITIVE, TEXEL0, 0, 0, 0, 1, COMBINED, 0, SHADE, 0, 0, 0, 0, COMBINED);
			}
			else if (childName == "LoadSync")
			{
				g = gsDPLoadSync();
			}
			else if (childName == "LoadBlock")
			{
				uint32_t tile = child->IntAttribute("Tile");
				uint32_t uls = child->IntAttribute("Uls");
				uint32_t ult = child->IntAttribute("Ult");
				uint32_t lrs = child->IntAttribute("Lrs");
				uint32_t dxt = child->IntAttribute("Dxt");

				g = gsDPLoadBlock(tile, uls, ult, lrs, dxt);
			}
			else if (childName == "Triangles2")
			{
				g = gsSP2Triangles(child->IntAttribute("V00"), child->IntAttribute("V01"), child->IntAttribute("V02"),
					child->IntAttribute("Flag0"), child->IntAttribute("V10"), child->IntAttribute("V11"), child->IntAttribute("V12"), child->IntAttribute("Flag1"));
			}
			else if (childName == "LoadVertices")
			{
				std::string fName = child->Attribute("Path");
				g = { gsSPVertex(0, child->IntAttribute("Count"), child->IntAttribute("Index")) };

				g.words.w0 &= 0x00FFFFFF;
				g.words.w0 += (G_VTX_OTR2 << 24);
				g.words.w1 = (uintptr_t)malloc(fName.size() + 1);
				strcpy((char*)g.words.w1, fName.data());

				dl->instructions.push_back(g.words.w0);
				dl->instructions.push_back(g.words.w1);

				g.words.w0 = child->IntAttribute("Count");
				g.words.w1 = child->IntAttribute("Index");
			}
			else if (childName == "SetTextureImage")
			{
				std::string fName = child->Attribute("Path");
				uint32_t fmt = child->IntAttribute("Format");
				uint32_t siz = child->IntAttribute("Size");
				uint32_t width = child->IntAttribute("Width");

				g = { gsDPSetTextureImage(fmt, siz, width + 1, 0) };
				g.words.w0 &= 0x00FFFFFF;
				g.words.w0 += (G_SETTIMG_OTR2 << 24);
				g.words.w1 = (uintptr_t)malloc(fName.size() + 1);
				strcpy((char*)g.words.w1, fName.data());

				dl->instructions.push_back(g.words.w0);
				dl->instructions.push_back(g.words.w1);

				g = gsDPPipeSync();
			}
			else if (childName == "SetTile")
			{
				uint32_t fmt = child->IntAttribute("Format");
				uint32_t siz = child->IntAttribute("Size");
				uint32_t line = child->IntAttribute("Line");
				uint32_t tmem = child->IntAttribute("TMem");
				uint32_t tile = child->IntAttribute("Tile");
				uint32_t palette = child->IntAttribute("Palette");
				uint32_t cmt0 = child->IntAttribute("Cmt0");
				uint32_t cmt1 = child->IntAttribute("Cmt1");
				uint32_t maskS = child->IntAttribute("MaskS");
				uint32_t maskT = child->IntAttribute("MaskT");
				uint32_t shiftS = child->IntAttribute("ShiftS");
				uint32_t shiftT = child->IntAttribute("ShiftT");
				
				g = gsDPSetTile(fmt, siz, line, tmem, tile, palette, cmt0, maskT, shiftT, cmt1, maskS, shiftS);
			}
			else if (childName == "SetTileSize")
			{
				uint32_t t = child->IntAttribute("T");
				uint32_t uls = child->IntAttribute("Uls");
				uint32_t ult = child->IntAttribute("Ult");
				uint32_t lrs = child->IntAttribute("Lrs");
				uint32_t lrt = child->IntAttribute("Lrt");

				g = gsDPSetTileSize(t, uls, ult, lrs, lrt);
			}
			else if (childName == "LoadTextureBlock")
			{
				uint32_t fmt = child->IntAttribute("Format");
				uint32_t siz = child->IntAttribute("Size");
				uint32_t width = child->IntAttribute("Width");
				uint32_t height = child->IntAttribute("Height");
				uint32_t maskS = child->IntAttribute("MaskS");
				uint32_t maskT = child->IntAttribute("MaskT");
				uint32_t shiftS = child->IntAttribute("ShiftS");
				uint32_t shiftT = child->IntAttribute("ShiftT");
				uint32_t cms = 0;
				uint32_t cmt = 0;

				if (child->Attribute("CMS_TXMirror", 0))
					cms |= G_TX_MIRROR;

				if (child->Attribute("CMS_TXNoMirror", 0))
					cms |= G_TX_NOMIRROR;

				if (child->Attribute("CMS_TXWrap", 0))
					cms |= G_TX_WRAP;

				if (child->Attribute("CMS_TXClamp", 0))
					cms |= G_TX_CLAMP;

				if (child->Attribute("CMT_TXMirror", 0))
					cmt |= G_TX_MIRROR;

				if (child->Attribute("CMT_TXNoMirror", 0))
					cmt |= G_TX_NOMIRROR;

				if (child->Attribute("CMT_TXWrap", 0))
					cmt |= G_TX_WRAP;

				if (child->Attribute("CMT_TXClamp", 0))
					cmt |= G_TX_CLAMP;

				std::string fName = child->Attribute("Path");

				Gfx g2[7];

				if (siz == 0)
				{
					Gfx g3[7] = { gsDPLoadTextureBlock(0, fmt, G_IM_SIZ_4b, width, height, 0, cms, cmt, maskS, maskT, shiftS, shiftT) };
					memcpy(g2, g3, 7 * sizeof(Gfx));
				}
				else if (siz == 1)
				{
					Gfx g3[7] = { gsDPLoadTextureBlock(0, fmt, G_IM_SIZ_8b, width, height, 0, cms, cmt, maskS, maskT, shiftS, shiftT) };
					memcpy(g2, g3, 7 * sizeof(Gfx));
				}
				else if (siz == 2)
				{
					Gfx g3[7] = { gsDPLoadTextureBlock(0, fmt, G_IM_SIZ_16b, width, height, 0, cms, cmt, maskS, maskT, shiftS, shiftT) };
					memcpy(g2, g3, 7 * sizeof(Gfx));
				}
				else if (siz == 3)
				{
					Gfx g3[7] = { gsDPLoadTextureBlock(0, fmt, G_IM_SIZ_32b, width, height, 0, cms, cmt, maskS, maskT, shiftS, shiftT) };
					memcpy(g2, g3, 7 * sizeof(Gfx));
				}

				g = { gsDPSetTextureImage(fmt, siz, width + 1, 0) };
				g.words.w0 &= 0x00FFFFFF;
				g.words.w0 += (G_SETTIMG_OTR2 << 24);
				g.words.w1 = (uintptr_t)malloc(fName.size() + 1);
				strcpy((char*)g.words.w1, fName.data());

				dl->instructions.push_back(g.words.w0);
				dl->instructions.push_back(g.words.w1);

				for (int j = 1; j < 7; j++)
				{
					dl->instructions.push_back(g2[j].words.w0);
					dl->instructions.push_back(g2[j].words.w1);
				}

				g = gsDPPipeSync();
			}
			else if (childName == "EndDisplayList")
			{
				g = gsSPEndDisplayList();
			}
			else if (childName == "CullDisplayList")
			{
				uint32_t start = child->IntAttribute("Start");
				uint32_t end = child->IntAttribute("End");

				g = gsSPCullDisplayList(start, end);
			}
			else if (childName == "ClipRatio")
			{
				// OTRTODO:
					//uint32_t ratio = child->IntAttribute("Start");

					//g = gsSPClipRatio(ratio);
			}
			else if (childName == "JumpToDisplayList")
			{
				char* dlPath = (char*)child->Attribute("Path");

				char* dlPath2 = (char*)malloc(strlen(dlPath) + 1);
				strcpy(dlPath2, dlPath);

				g = gsSPBranchListOTR2(dlPath2);
			}
			else if (childName == "CallDisplayList")
			{
				char* dlPath = (char*)child->Attribute("Path");

				char* dlPath2 = (char*)malloc(strlen(dlPath) + 1);
				strcpy(dlPath2, dlPath);

				g = gsSPDisplayListOTR2(dlPath2);
			}
			else if (childName == "ClearGeometryMode" || childName == "SetGeometryMode")
			{
				uint64_t clearData = 0;

				if (child->Attribute("Shade", 0))
					clearData |= G_SHADE;

				if (child->Attribute("Lighting", 0))
					clearData |= G_LIGHTING;

				if (child->Attribute("ShadingSmooth", 0))
					clearData |= G_SHADING_SMOOTH;

				if (child->Attribute("ZBuffer", 0))
					clearData |= G_ZBUFFER;

				if (child->Attribute("TextureGen", 0))
					clearData |= G_TEXTURE_GEN;

				if (child->Attribute("TextureGenLinear", 0))
					clearData |= G_TEXTURE_GEN_LINEAR;

				if (child->Attribute("CullBack", 0))
					clearData |= G_CULL_BACK;

				if (child->Attribute("CullFront", 0))
					clearData |= G_CULL_FRONT;

				if (child->Attribute("CullBoth", 0))
					clearData |= G_CULL_BOTH;

				if (child->Attribute("Fog", 0))
					clearData |= G_FOG;

				if (child->Attribute("Clipping", 0))
					clearData |= G_CLIPPING;

				if (childName == "ClearGeometryMode")
					g = gsSPClearGeometryMode(clearData);
				else
					g = gsSPSetGeometryMode(clearData);
			}
			else if (childName == "SetRenderMode")
			{
				uint32_t mode1 = 0;
				uint32_t mode2 = 0;

				if (child->Attribute("Mode1_AAEN", 0))
					mode1 |= AA_EN;

				if (child->Attribute("Mode1_ZCMP", 0))
					mode1 |= Z_CMP;

				if (child->Attribute("Mode1_ZUPD", 0))
					mode1 |= Z_UPD;

				if (child->Attribute("Mode1_IMRD", 0))
					mode1 |= IM_RD;

				if (child->Attribute("Mode1_CVGDstWrap", 0))
					mode1 |= CVG_DST_WRAP;

				if (child->Attribute("Mode1_CVGDstFull", 0))
					mode1 |= CVG_DST_FULL;

				if (child->Attribute("Mode1_ZModeInter", 0))
					mode1 |= ZMODE_INTER;

				if (child->Attribute("Mode1_ZModeXLU", 0))
					mode1 |= ZMODE_XLU;

				if (child->Attribute("Mode1_ZModeDec", 0))
					mode1 |= ZMODE_DEC;

				if (child->Attribute("Mode1_CVGXAlpha", 0))
					mode1 |= CVG_X_ALPHA;

				if (child->Attribute("Mode1_AlphaCVGSel", 0))
					mode1 |= ALPHA_CVG_SEL;

				if (child->Attribute("Mode1_ForceBL", 0))
					mode1 |= FORCE_BL;

				if (child->Attribute("Mode2_AAEN", 0))
					mode2 |= AA_EN;

				if (child->Attribute("Mode2_ZCMP", 0))
					mode2 |= Z_CMP;

				if (child->Attribute("Mode2_ZUPD", 0))
					mode2 |= Z_UPD;

				if (child->Attribute("Mode2_IMRD", 0))
					mode2 |= IM_RD;

				if (child->Attribute("Mode2_CVGDstWrap", 0))
					mode2 |= CVG_DST_WRAP;

				if (child->Attribute("Mode2_CVGDstFull", 0))
					mode2 |= CVG_DST_FULL;

				if (child->Attribute("Mode2_ZModeInter", 0))
					mode2 |= ZMODE_INTER;

				if (child->Attribute("Mode2_ZModeXLU", 0))
					mode2 |= ZMODE_XLU;

				if (child->Attribute("Mode2_ZModeDec", 0))
					mode2 |= ZMODE_DEC;

				if (child->Attribute("Mode2_CVGXAlpha", 0))
					mode2 |= CVG_X_ALPHA;

				if (child->Attribute("Mode2_AlphaCVGSel", 0))
					mode2 |= ALPHA_CVG_SEL;

				if (child->Attribute("Mode2_ForceBL", 0))
					mode2 |= FORCE_BL;


				g = gsDPSetRenderMode(mode1, mode2);

			}
			else
			{
				printf("DisplayListXML: Unknown node %s\n", childName.c_str());
				g = gsDPPipeSync();
			}

			dl->instructions.push_back(g.words.w0);
			dl->instructions.push_back(g.words.w1);

			child = child->NextSiblingElement();
		}
	}
}