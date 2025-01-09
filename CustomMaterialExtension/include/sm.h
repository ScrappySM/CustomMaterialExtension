#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdint.h>
#include <string>
#include <vector>

enum MaterialCategory : uint8_t {
	MaterialCategory_Asset = 0x0,
	MaterialCategory_Character = 0x1,
	MaterialCategory_Block = 0x2,
	MaterialCategory_Part = 0x3,
	MaterialCategory_PartFullTransform = 0x4,
	MaterialCategory_Text = 0x5,
	MaterialCategory_Slant = 0x6,
	MaterialCategory_SlantFullTransform = 0x7,
	MaterialCategory_Billboard = 0x8,
	MaterialCategory_Line = 0x9,
};

namespace SM {
	static constexpr uintptr_t MaterialManagerOffset = 0x1269848;
	static constexpr uintptr_t MaterialManagerLoadOffset = 0x0806FC0;

	struct MaterialManager {};

	void LoadMaterialDefinitions(
		MaterialCategory category,
		const std::string& setPath,
		const std::string& shaderPath,
		uint8_t flags, // Used when category is 1 and flags is 1
		bool skipOptimization,
		const std::vector<std::string>& additionalIncludes // Empty vec is fine
	) {
		auto baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
		auto materialManager = reinterpret_cast<MaterialManager*>(baseAddress + MaterialManagerOffset);

		auto loadMaterialDefinitions = reinterpret_cast<void(__thiscall*)(MaterialManager*, MaterialCategory, const std::string&, const std::string&, uint8_t, bool, const std::vector<std::string>&)>(baseAddress + MaterialManagerLoadOffset);

		loadMaterialDefinitions(materialManager, category, setPath, shaderPath, flags, skipOptimization, additionalIncludes);
	}
} // namespace SM