#pragma once
#include <cstdint>
#include "..\C_EntityInstance\C_EntityInstance.h"
#include "../../../templeware/utils/memory/memorycommon.h"
#include "../../../templeware/utils/math/vector/vector.h"
#include "..\..\..\..\source\templeware\utils\schema\schema.h"
#include "..\..\..\..\source\templeware\utils\memory\vfunc\vfunc.h"
#include "..\handle.h"

class C_AggregateSceneObjectData
{
private:
	char pad_0000[0x38]; 
public:
	std::uint8_t r;
	std::uint8_t g; 
	std::uint8_t b; 
private:
	char pad_0038[0x9];
};

class C_AggregateSceneObject
{
private:
	char pad_0000[0x120];
public:
	int m_nCount;
private:
	char pad_0120[0x4];
public:
	C_AggregateSceneObjectData* m_pData; 
};

class CSceneLightObject {
public:
	char pad_0000[0xE4]; // 0x0
	float r, g, b; // 0xE4
};
