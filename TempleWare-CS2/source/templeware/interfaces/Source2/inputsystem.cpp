#include "inputsystem.h"
#include "../interfaces.h"

typedef void (*SetRelativeMouseModeFn)(void*, bool);
typedef bool (*GetRelativeMouseModeFn)(void*);
typedef void (*SDLWindowsGrab)(void*, bool);

void IInputSystem::SetRelativeMouseMode(bool enable) {
	SetRelativeMouseModeFn func = reinterpret_cast<SetRelativeMouseModeFn>((*reinterpret_cast<void***>(this))[76]);
	func(this, enable);
}
void IInputSystem::SetWindowsGrab(bool enable) {
	SDLWindowsGrab func = reinterpret_cast<SDLWindowsGrab>((*reinterpret_cast<void***>(this))[73]);
	func(this, enable);
}