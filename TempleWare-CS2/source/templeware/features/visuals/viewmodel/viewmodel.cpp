#include "../../../hooks/hooks.h"
#include "../../../config/config.h"


void* __fastcall H::hkDrawViewModel(float* a1, float* offsets, float* fov) {
	auto ss = DrawViewModel.GetOriginal();
	void* pRet = ss(a1, offsets, fov);
	if (Config::viewmodell) {
		*offsets = Config::viewx;
		offsets[1] = Config::viewy;
		offsets[2] = Config::viewz;

		*fov = Config::ifov;
	}

	return pRet;
}