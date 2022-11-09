#include "ResourceLoader.h"
#include "MaterialFactory.h"
#include "SceneFactory.h"
#include "CollisionHeaderFactory.h"
#include "DisplayListFactory.h"
#include "PlayerAnimationFactory.h"
#include "SkeletonFactory.h"
#include "SkeletonLimbFactory.h"
#include "AnimationFactory.h"
#include "VtxFactory.h"
#include "CutsceneFactory.h"
#include "ArrayFactory.h"
#include "PathFactory.h"
#include "TextFactory.h"
#include "TextureFactory.h"
#include "BlobFactory.h"
#include "MtxFactory.h"
#include "AudioFactory.h"
#include <Utils/MemoryStream.h>

namespace Ship
{
    Resource* ResourceLoader::LoadResource(std::shared_ptr<File> FileToLoad)
    {
        auto reader = std::make_shared<BinaryReader>(FileToLoad->buffer.get(), FileToLoad->dwBufferSize);
        Resource* result = nullptr;
        ResourceType resourceType;

        // Determine if file is binary or XML...
        uint8_t firstByte = reader->ReadInt8();

        if (firstByte == '<')
        {
            // XML
            reader->Seek(-1, SeekOffsetType::Current);

            std::string xmlStr = reader->ReadCString();
            
            tinyxml2::XMLDocument doc;
            tinyxml2::XMLError eResult = doc.Parse(xmlStr.data());

            // OTRTODO: Error checking

            tinyxml2::XMLElement* root = doc.FirstChildElement();

            std::string nodeName = root->Name();

            if (nodeName == "Text")
            {
                result = TextFactory::ReadTextXML(root);
                resourceType = ResourceType::Text;
            }
            else if (nodeName == "Vertex")
            {
                result = VertexFactory::ReadVtxXML(root);
                resourceType = ResourceType::Vertex;
            }
            else if (nodeName == "DisplayList")
            {
                result = DisplayListFactory::ReadDisplayListXML(root);
                resourceType = ResourceType::DisplayList;
            }
            else if (nodeName == "Skeleton")
            {
                result = SkeletonFactory::ReadSkeletonXML(root);
                resourceType = ResourceType::Skeleton;
            }
            else if (nodeName == "SkeletonLimb")
            {
                result = SkeletonLimbFactory::ReadSkeletonLimbXML(root);
                resourceType = ResourceType::SkeletonLimb;
            }
            else
            {
                // RESOURCE TYPE NOT SUPPORTED
            }
        }
        else
        {
            // Binary
            Endianness endianness = (Endianness)firstByte;

            for (int i = 0; i < 3; i++)
                reader->ReadInt8();

            reader->SetEndianness(endianness);
            
            resourceType = (ResourceType)reader->ReadUInt32();

            switch (resourceType)
            {
            case ResourceType::Material:
                result = MaterialFactory::ReadMaterial(reader.get());
                break;
            case ResourceType::Texture:
                result = TextureFactory::ReadTexture(reader.get());
                break;
            case ResourceType::Room:
                result = SceneFactory::ReadScene(reader.get());
                break;
            case ResourceType::CollisionHeader:
                result = CollisionHeaderFactory::ReadCollisionHeader(reader.get());
                break;
            case ResourceType::DisplayList:
                result = DisplayListFactory::ReadDisplayList(reader.get());
                break;
            case ResourceType::PlayerAnimation:
                result = PlayerAnimationFactory::ReadPlayerAnimation(reader.get());
                break;
            case ResourceType::Skeleton:
                result = SkeletonFactory::ReadSkeleton(reader.get());
                break;
            case ResourceType::SkeletonLimb:
                result = SkeletonLimbFactory::ReadSkeletonLimb(reader.get());
                break;
            case ResourceType::Vertex:
                result = VertexFactory::ReadVtx(reader.get());
                break;
            case ResourceType::Animation:
                result = AnimationFactory::ReadAnimation(reader.get());
                break;
            case ResourceType::Cutscene:
                result = CutsceneFactory::ReadCutscene(reader.get());
                break;
            case ResourceType::Array:
                result = ArrayFactory::ReadArray(reader.get());
                break;
            case ResourceType::Path:
                result = PathFactory::ReadPath(reader.get());
                break;
            case ResourceType::Text:
                result = TextFactory::ReadText(reader.get());
                break;
            case ResourceType::Blob:
                result = BlobFactory::ReadBlob(reader.get());
                break;
            case ResourceType::Matrix:
                result = MtxFactory::ReadMtx(reader.get());
                break;
            case ResourceType::Audio:
                result = AudioFactory::ReadAudio(reader.get());
                break;
            case ResourceType::AudioSample:
                result = AudioSampleFactory::ReadAudioSample(reader.get());
                break;
            case ResourceType::AudioSoundFont:
                result = AudioSoundFontFactory::ReadAudioSoundFont(reader.get());
                break;
            case ResourceType::AudioSequence:
                result = AudioSequenceFactory::ReadAudioSequence(reader.get());
                break;
            default:
                // RESOURCE TYPE NOT SUPPORTED
                break;
            }
        }

        if (result != nullptr) {
            result->file = FileToLoad;
            result->resType = resourceType;
        }
        else {
            if (FileToLoad != nullptr) {
                SPDLOG_ERROR("Failed to load resource of type {} \"{}\"", resourceType, FileToLoad->path);
            }
            else {
                SPDLOG_ERROR("Failed to load resource because the file did not load.");
            }
        }

        return result;
    }
}