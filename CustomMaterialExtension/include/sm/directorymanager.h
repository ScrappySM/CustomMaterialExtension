#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <unordered_map>

#include "offsets.h"

namespace SM {
	class DirectoryManager {
	public:
		/// <summary>Gets the replacement for the given key</summary>
		/// <param name="key">The key to search for, e.g. `$GAME_DATA`</param>
		/// <returns>
		/// The replacement for the given key, or an empty string if the key is not found.
		/// E.g. `$GAME_DATA` might be replaced with `C:/Program Files (x86)/Steam/steamapps/common/Scrap Mechanic/Data/``
		/// </returns>
		static std::string GetVariablePath(const std::string& key) {
			auto instance = *reinterpret_cast<DirectoryManager**>(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + Offsets::DirectoryManager);
			static auto& variablePaths = instance->variablePaths;
			auto it = variablePaths.find(key);
			if (it == variablePaths.end())
				return "";

			return it->second;
		}

		/// <summary>
		/// Separates the key from the path
		/// </summary>
		/// <param name="path">The path to separate the key from, e.g. `$GAME_DATA/Textures/`</param>
		/// <returns>The key from the path, or an empty string if the path does not contain a key.</returns>
		static std::string SeparateKey(const std::string& path) {
			return path.substr(0, path.find_first_of('/'));
		}

	private:
		/* 0x0000 */ char pad_0x0000[8];
	public:
		/* 0x0008 */ std::unordered_map<std::string, std::string> variablePaths;
	};
} // namespace SM
