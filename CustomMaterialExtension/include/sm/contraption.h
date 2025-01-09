#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include "offsets.h"

namespace SM {
	/// <summary>
	/// What colour to log in
	/// </summary>
	enum Colour : WORD {
		DARKGREEN = 2,
		BLUE = 3,
		PURPLE = 5,
		GOLD = 6,
		WHITE = 7,
		DARKGRAY = 8,
		DARKBLUE = 9,
		GREEN = 10,
		CYAN = 11,
		RED = 12,
		PINK = 13,
		YELLOW = 14,
	};

	/// <summary>
	/// What type of log to make
	/// </summary>
	enum LogType {
		Default = 1 << 0,
		Profile = 1 << 1,
		Resource = 1 << 2,
		Shader = 1 << 3,
		Buffer = 1 << 4,
		Render = 1 << 5,
		Network = 1 << 6,
		System = 1 << 7,
		Terrain = 1 << 8,
		World = 1 << 9,
		Lua = 1 << 11,
		Print = 1 << 12,
		Audio = 1 << 10,
		UGC = 1 << 13,
		Steam = 1 << 14,
		Error = -1,
		Warning = 1 << 30,
		None = 0
	};

	class Console {
	public:
		virtual ~Console() { /* Implemented by game */ }
		virtual void Log(const std::string& message, Colour colour = Colour::WHITE, LogType type = LogType::Default) { /* Implemented by game */ }
		virtual void LogNoReturn(const std::string& message, Colour colour = Colour::WHITE, LogType type = LogType::Default) { /* Implemented by game */ }
	};

	/// <summary>
	/// The game engine singleton
	/// </summary>
	class Contraption {
	public:
		/* 0x0000 */ HWND hWnd;
	private:
		/* 0x0008 */ char pad_0x0008[0x50];
	public:
		Console* console;

	private:
		inline static Console* GetConsole() {
			Contraption* contraption = *reinterpret_cast<Contraption**>(reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) + Offsets::Contraption);
			return contraption->console;
		}

	public:
		/// <summary>
		/// Logs a message to the game's console
		/// </summary>
		/// <param name="message">What to log</param>
		/// <param name="colour">What colour (default is white)</param>
		/// <param name="type">What type of log (default is default)</param>
		inline static void Log(const std::string& message, Colour colour = Colour::WHITE, LogType type = LogType::Default) {
			GetConsole()->Log(message, colour, type);
		}
	};
} // namespace SM
