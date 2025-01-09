#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <MinHook.h>

#include <iostream>

#include "offsets.h"
#include "contraption.h"
#include "directorymanager.h"
#include "materialmanager.h"

namespace SM {
	typedef void(__thiscall* ShapesetLoadFunc_t)(void*, const std::string&, int);
	ShapesetLoadFunc_t oShapesetLoadFunc = nullptr;

	static void __fastcall hkShapesetLoadFunc(void* thisptr, const std::string& pathIn, int a2) {
		std::string key = DirectoryManager::SeparateKey(pathIn);
		std::string contentPath = DirectoryManager::GetVariablePath(key);

		if (key.find("CONTENT") != std::string::npos) {
			Contraption::Log("CustomMaterialExtension found material set at: " + contentPath + pathIn, Colour::GOLD, LogType::Render);
			if (IsDebuggerPresent()) __debugbreak();

			// TODO: Load material definitions
		}

		oShapesetLoadFunc(thisptr, pathIn, a2);
	}

	static void InitHooks() {
		static auto baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
		static auto shapesetLoadFunc = reinterpret_cast<ShapesetLoadFunc_t>(baseAddress + Offsets::ShapesetLoadFunc);

#define MHFUNC(res) if (res != MH_OK) { std::cerr << "Failed to hook" << ": " << MH_StatusToString(res) << std::endl; return; }
		MHFUNC(MH_Initialize());
		MHFUNC(MH_CreateHook(reinterpret_cast<LPVOID>(shapesetLoadFunc), hkShapesetLoadFunc, reinterpret_cast<LPVOID*>(&oShapesetLoadFunc)));
		MHFUNC(MH_EnableHook(reinterpret_cast<LPVOID>(shapesetLoadFunc)));

		Contraption::Log("CustomMaterialExtension loaded", Colour::GREEN, LogType::Print);
	}
} // namespace SM