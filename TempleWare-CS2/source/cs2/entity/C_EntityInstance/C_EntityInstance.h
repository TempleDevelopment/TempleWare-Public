#pragma once
#include "..\..\..\..\source\templeware\utils\schema\schema.h"
#include "..\..\..\..\source\templeware\utils\memory\vfunc\vfunc.h"
#include "..\handle.h"

class CEntityInstance;
class CEntityIdentity
{
public:
	SCHEMA_ADD_OFFSET(std::uint32_t, index, 0x10);
	schema(const char*, m_designerName, "CEntityIdentity->m_designerName");
	schema( std::uint32_t, flags, "CEntityIdentity->m_flags");

	std::string GetSchemaName() {

		auto class_info = *(uintptr_t*)(std::uintptr_t(this) + 0x10);
		auto name = *(const char*)(std::uintptr_t(this) + 0x20);

		return std::to_string(name);  // Return the string constructed from the char pointer
	}

	[[nodiscard]] bool valid()
	{
		return index() != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int get_index()
	{
		if (!valid())
			return ENT_ENTRY_MASK;

		return index() & ENT_ENTRY_MASK;
	}

	[[nodiscard]] int get_serial_number()
	{
		return index() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* pInstance;
};

class CEntityInstance
{
public:

	void dump_class_info(SchemaClassInfoData_t** pReturn)
	{
		return M::vfunc<void, 42U>(this, pReturn);
	}


	[[nodiscard]] std::uint32_t get_entity_by_handle()
	{
		CEntityIdentity* identity = m_pEntityIdentity();
		if (identity == nullptr)
			return 0;


		return identity->get_index();
	}

	[[nodiscard]] CBaseHandle handle()
	{
		CEntityIdentity* identity = m_pEntityIdentity();
		if (identity == nullptr)
			return CBaseHandle();

		return CBaseHandle(identity->get_index(), identity->get_serial_number() - (identity->flags() & 1));
	}

	schema(CEntityIdentity*, m_pEntityIdentity, "CEntityInstance->m_pEntity");
};
