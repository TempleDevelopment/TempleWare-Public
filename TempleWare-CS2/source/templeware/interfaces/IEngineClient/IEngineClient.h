#pragma once

// used: callvfunc
#include "..\..\utils\memory\vfunc\vfunc.h"
#include <type_traits>
#include "..\..\utils\schema\schema.h"
class IEngineClient
{
public:
	void send_cmd(const char* szCmd) {
		return M::vfunc<void, 48>(this, 0, szCmd, 0x7FFEF001);
	}

	int maxClients()
	{
		return M::vfunc<int, 36U>(this);
	}

	bool in_game()
	{
		return M::vfunc<bool, 37U>(this);
	}

	bool connected()
	{
		return M::vfunc<bool, 38U>(this);
	}

	int get_local_player() {
		int nIndex = -1;
		M::vfunc<void, 52U>(this, std::ref(nIndex), 0);
		return nIndex + 1;
	}
public:
	inline bool valid() {
		return connected() && in_game();
	}
};
