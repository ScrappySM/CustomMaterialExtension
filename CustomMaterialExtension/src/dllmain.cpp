#include <Windows.h>
#include <iostream>

#include "sm/hooks.h"

// DllMain function which gets called when the DLL is loaded
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		SM::InitHooks();
	}

	if (dwReason == DLL_PROCESS_DETACH) {
		MH_Uninitialize();
	}

	return TRUE;
}
