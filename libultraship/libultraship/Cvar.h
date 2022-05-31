#ifndef _CVAR_H
#define _CVAR_H

#include <PR/ultra64/gbi.h>
#include "color.h"

#ifdef __cplusplus
typedef enum class CVarType 
{ 
    S32, 
    Float, 
    String,
    RGBA
} CVarType;

typedef struct CVar {
    const char* name;
    CVarType type;

    union {
        s32 valueS32;
        float valueFloat;
        const char* valueStr;
        Color_RGBA8 valueRGBA;
    } value;
} CVar;

CVar* CVar_Get(char* name);
#endif

#ifdef __cplusplus
extern "C"
{
#endif
//#include <ultra64.h>

s32 CVar_GetS32(const char* name, s32 defaultValue);
float CVar_GetFloat(const char* name, float defaultValue);
const char* CVar_GetString(const char* name, const char* defaultValue);
void CVar_SetS32(const char* name, s32 value);

Color_RGB8 CVar_GetRGB(const char* name, Color_RGB8 defaultValue);
Color_RGBA8 CVar_GetRGBA(const char* name, Color_RGBA8 defaultValue);
void CVar_SetRGBA(const char* name, Color_RGBA8 value);

void CVar_RegisterS32(const char* name, s32 defaultValue);
void CVar_RegisterFloat(const char* name, float defaultValue);
void CVar_RegisterString(const char* name, const char* defaultValue);
void CVar_RegisterRGBA(const char* name, Color_RGBA8 defaultValue);

#ifdef __cplusplus
};
#endif

#ifdef __cplusplus
#include <map>
#include <string>
#include <functional>
#include <memory>

extern "C" CVar * CVar_Get(const char* name);
extern std::map<std::string, std::unique_ptr<CVar>, std::less<>> cvars;
void CVar_SetFloat(const char* name, float value);
void CVar_SetString(const char* name, const char* value);
#endif
#endif
