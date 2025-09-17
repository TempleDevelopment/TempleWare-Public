#include "../../../hooks/hooks.h"
#include "../../../config/config.h"

void* __fastcall H::hkDrawSmokeArray(void* rcx, void* pSomePointer1, void* pSomePointer2, void* pSomePointer3, void* pSomePointer4, void* pSomePointer5) {
    if (Config::SmokeRemove) return nullptr;
    return DrawSmokeArray.GetOriginal()(rcx, pSomePointer1, pSomePointer2, pSomePointer3, pSomePointer4, pSomePointer5);
}