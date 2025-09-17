#include "module.h"
#include <Windows.h>

Module::Module(uintptr_t moduleAddress, const std::string &moduleName) : address(moduleAddress), name(moduleName) {}

void Modules::init() {
	registerModule("client.dll", "client");
	registerModule("scenesystem.dll", "scenesystem");
	registerModule("particles.dll", "particles");
	registerModule("materialsystem2.dll", "materialsystem2");
	registerModule("tier0.dll", "tier0");
	registerModule("engine2.dll", "engine2");
	registerModule("inputsystem.dll", "inputsystem");
}

// Completely acceptable solution because there simply just aren't that many modules :)
uintptr_t Modules::getModule(const std::string &moduleName) {
	for (const Module &m : modules) {
		if (m.name == moduleName) {
			return m.address;
		}
	}
	// No module found
	return 0;
}

bool Modules::registerModule(const std::string &aModuleName, const std::string &moduleName) {
	uintptr_t moduleHandle = reinterpret_cast<uintptr_t>(GetModuleHandle(aModuleName.c_str()));

	if (!moduleHandle)
		return false;

	Module module(moduleHandle, moduleName);
	modules.emplace_back(module);

	return true;
}

Modules modules;