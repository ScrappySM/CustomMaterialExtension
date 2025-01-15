#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <MinHook.h>

#include <fmt/format.h>

#include <thread>
#include <iostream>
#include <filesystem>

#include "offsets.h"
#include "contraption.h"
#include "directorymanager.h"
#include "materialmanager.h"

namespace SM {
	typedef void(__thiscall* ShapesetLoadFunc_t)(void*, const std::string&, int);
	ShapesetLoadFunc_t oShapesetLoadFunc = nullptr;

	typedef void(__thiscall* MaterialManagerLoad_t)(void*, MaterialCategory, const std::string&, const std::string&, uint8_t, bool, const std::vector<std::string>&);
	MaterialManagerLoad_t oMaterialManagerLoad = nullptr;

	static void __fastcall hkShapesetLoadFunc(void* thisptr, const std::string& pathIn, int a2) {
		std::string key = DirectoryManager::SeparateKey(pathIn);
		std::string contentPath = DirectoryManager::GetVariablePath(key);

		if (key.find("CONTENT") != std::string::npos) {
			Contraption::Log("CustomMaterialExtension found material set at: " + contentPath, Colour::GOLD, LogType::Render);
			if (IsDebuggerPresent()) __debugbreak();

			// Check for `Render/Materials`
			std::filesystem::path path = contentPath;
			path /= "Render/Materials";

			if (std::filesystem::exists(path)) {
				for (auto& it : std::filesystem::directory_iterator(path)) {
					if (!it.is_regular_file()) {
						continue;
					}

					if (it.path().extension() != ".json") {
						continue;
					}

					/*std::string materialPath = it.path().string();
					std::string materialPathNoExt = materialPath.substr(0, materialPath.rfind('.'));
					std::string materialShader = materialPathNoExt + ".hlsl";*/

					Contraption::Log(key + " -> " + contentPath, Colour::YELLOW, LogType::Render);
					std::string jsonFile = it.path().filename().string();
					Contraption::Log("Found material set: " + jsonFile, Colour::YELLOW, LogType::Render);

					std::string jsonPath = key + "/Render/Materials/" + jsonFile;

					//MaterialManager::LoadMaterialDefinitions(MaterialCategory::Block, jsonPath, "$GAME_DATA/Shaders/hlsl/main_part.hlsl");
					//MaterialManager::LoadMaterialDefinitions((MaterialCategory)3, "$GAME_DATA/Render/Materials/part_materials.json", "$GAME_DATA/Shaders/hlsl/main_part.hlsl", 0, true, {});

					//MaterialManager::LoadMaterialDefinitions((MaterialCategory)3, jsonPath, "$GAME_DATA/Shaders/hlsl/main_part.hlsl", 0, true, {});
					// TODO: Find a better solution
					/*std::thread([jsonPath]() {
						while (!Contraption::GetInstance() || Contraption::GetInstance()->gameStateIndex == 1) {
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}

						std::this_thread::sleep_for(std::chrono::seconds(1));

						MaterialManager::LoadMaterialDefinitions((MaterialCategory)3, jsonPath, "$GAME_DATA/Shaders/hlsl/main_part.hlsl", 0, true, {});
						}).detach();*/
				}
			}
		}

		oShapesetLoadFunc(thisptr, pathIn, a2);
	}

	static void __fastcall hkMaterialManagerLoad(void* thisptr, MaterialCategory category, const std::string& setPath, const std::string& shaderPath, uint8_t flags, bool skipOptimization, const std::vector<std::string>& additionalIncludes) {
		Contraption::Log(fmt::format("MaterialManager::Load({}, '{}', '{}', {}, {}, {})", (int)category, setPath, shaderPath, flags, skipOptimization, additionalIncludes.size()), Colour::GOLD, LogType::Render);
		oMaterialManagerLoad(thisptr, category, setPath, shaderPath, flags, skipOptimization, additionalIncludes);
	}

	static void InitHooks() {
		static auto baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

#define MHFUNC(res) if (res != MH_OK) { std::cerr << "Failed to hook" << ": " << MH_StatusToString(res) << std::endl; return; }
		MHFUNC(MH_Initialize());

		static auto shapesetLoadFunc = reinterpret_cast<ShapesetLoadFunc_t>(baseAddress + Offsets::ShapesetLoadFunc);
		MHFUNC(MH_CreateHook(reinterpret_cast<LPVOID>(shapesetLoadFunc), hkShapesetLoadFunc, reinterpret_cast<LPVOID*>(&oShapesetLoadFunc)));
		MHFUNC(MH_EnableHook(reinterpret_cast<LPVOID>(shapesetLoadFunc)));

		static auto materialManagerLoadFunc = reinterpret_cast<void(__thiscall*)(void*, const std::string&, int)>(baseAddress + Offsets::MaterialManagerLoad);
		MHFUNC(MH_CreateHook(reinterpret_cast<LPVOID>(materialManagerLoadFunc), hkMaterialManagerLoad, reinterpret_cast<LPVOID*>(&oMaterialManagerLoad)));
		MHFUNC(MH_EnableHook(reinterpret_cast<LPVOID>(materialManagerLoadFunc)));

		// MaterialManager::Load(3, '$GAME_DATA/Render/Materials/part_materials.json', '$GAME_DATA/Shaders/hlsl/main_part.hlsl', 0, true, 0)
		//MaterialManager::LoadMaterialDefinitions((MaterialCategory)3, "$GAME_DATA/Render/Materials/part_materials.json", "$GAME_DATA/Shaders/hlsl/main_part.hlsl", 0, true, {});

		// MaterialManager::Load(3, '$CONTENT_be9dd720-4ea4-4fb5-81a8-616d96992bf6/Render/Materials/test.json', '$GAME_DATA/Shaders/hlsl/main_part.hlsl', 0, true, 0)
		MaterialManager::LoadMaterialDefinitions((MaterialCategory)3, "$CONTENT_be9dd720-4ea4-4fb5-81a8-616d96992bf6/Render/Materials/test.json", "$GAME_DATA/Shaders/hlsl/main_part.hlsl", 0, true, {});

		Contraption::Log("CustomMaterialExtension loaded", Colour::GREEN, LogType::Print);
	}
} // namespace SM