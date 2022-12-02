#pragma once

#include <string>
#include <unordered_map>
#include <bitset>

#include <windows.h>

#include "INIReader.h"

struct CursorPosition {
	long x, y;
};

class InputHandler
{
public:
	InputHandler();

	void Update();

	const bool IsActionKeyPressed(size_t action_key) const;
	const CursorPosition& GetMouseCoordinates();
	const std::vector<std::string>& GetFields() const;

private:
	void LoadConfiguration();

	bool IsKeyDown(size_t vk_key);

	void MapSymbol(std::string strSymbol, size_t nSymbol);
	void MapAllSymbols();
	void MapInputEvent(std::size_t nSymbol, size_t nCommand);
	void Remap();

	std::string m_strMapFilePath;

	typedef std::unordered_map<std::string, size_t> TCommandMap;
	typedef std::unordered_map<std::string, size_t> TSymbolMap;
	typedef std::unordered_map<size_t, size_t> TInputEventMap;
	typedef std::unordered_map<std::string, std::string> TCommandSymbolMap;

	TCommandMap m_commandMap;
	TSymbolMap m_symbolMap;
	TInputEventMap m_inputEventMap;
	TCommandSymbolMap m_commandSymbolMap;

	std::vector<bool> m_InputState;
	CursorPosition m_cursorPosition;
	HWND m_hwnd;

	std::vector<std::string> m_commandVector;
};

