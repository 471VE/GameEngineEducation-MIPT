#pragma once

#include "Common.h"
#include "IInputHandlerBackend.h"

#include <memory>
#include <unordered_map>
#include <windows.h>

class INPUTHANDLERBACKEND_API WinAPIBackend : public IInputHandlerBackend {
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual const bool IsActionKeyPressed(size_t action_key) const override;
	virtual const CursorPosition& GetMouseCoordinates() override;
	virtual const std::vector<std::string>& GetFields() const override;

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