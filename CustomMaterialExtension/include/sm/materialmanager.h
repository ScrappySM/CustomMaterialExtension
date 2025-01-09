#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdint.h>
#include <string>
#include <vector>

#include "offsets.h"

namespace SM {
	enum MaterialCategory : uint8_t {
		Asset = 0x0,
		Character = 0x1,
		Block = 0x2,
		Part = 0x3,
		PartFullTransform = 0x4,
		Text = 0x5,
		Slant = 0x6,
		SlantFullTransform = 0x7,
		Billboard = 0x8,
		Line = 0x9,
	};

	struct MaterialManager {
	public:
		/// <summary>
		/// Loads material definitions from the given set and shader path
		/// </summary>
		/// <param name="category">
		/// The type of material to load
		/// </param>
		/// <param name="setPath">
		///	The path to the material set JSON file
		/// </param>
		/// <param name="shaderPath">
		/// The path to the shader file (.hlsl)
		/// </param>
		/// <param name="flags">
		/// Flags to pass to the material loader
		/// </param>
		/// <param name="skipOptimization">
		/// Whether to skip optimization
		/// </param>
		/// <param name="additionalIncludes">
		/// Additional include paths to pass to the material loader, can be an empty vector
		/// </param>
		static void LoadMaterialDefinitions(
			MaterialCategory category,
			const std::string& setPath,
			const std::string& shaderPath,
			uint8_t flags = 0,
			bool skipOptimization = false,
			const std::vector<std::string>& additionalIncludes = {}
		) {
			static auto baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
			static auto materialManager = reinterpret_cast<struct MaterialManager*>(baseAddress + Offsets::MaterialManager);
			static auto loadMaterialDefinitions = reinterpret_cast<void(__thiscall*)(MaterialManager*, MaterialCategory, const std::string&, const std::string&, uint8_t, bool, const std::vector<std::string>&)>(baseAddress + Offsets::MaterialManagerLoad);

			loadMaterialDefinitions(materialManager, category, setPath, shaderPath, flags, skipOptimization, additionalIncludes);
		}
	};
} // namespace SM
