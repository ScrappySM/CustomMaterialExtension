#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace SM::Offsets {
	static constexpr uintptr_t MaterialManager = 0x1269848;
	static constexpr uintptr_t DirectoryManager = 0x1267810;
	static constexpr uintptr_t Contraption = 0x1267538;

	static constexpr uintptr_t MaterialManagerLoad = 0x0806FC0;
	static constexpr uintptr_t ShapesetLoadFunc = 0x599120;
};
