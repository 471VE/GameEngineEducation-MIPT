#include "InputHandler.h"

#include <windows.h>
#include <algorithm>
#include <cassert>

InputHandler::InputHandler()
{
	m_strMapFilePath = "../../../Assets/Configs/actionmap.ini";
	std::replace(m_strMapFilePath.begin(), m_strMapFilePath.end(), '\\', '/');

	MapAllSymbols();
	LoadConfiguration();
	Remap();

	m_hwnd = GetActiveWindow();
}

bool InputHandler::IsKeyDown(size_t vk_key)
{
	if (GetAsyncKeyState(vk_key) & 0x8000)
		return true;

	return false;
}

void InputHandler::MapSymbol(std::string strSymbol, size_t nSymbol)
{
	m_symbolMap[strSymbol] = nSymbol;
}

void InputHandler::MapInputEvent(std::size_t nSymbol, size_t nCommand)
{
	m_inputEventMap[nSymbol] = nCommand;
}

void InputHandler::Remap()
{
	for (auto& it : m_commandSymbolMap)
	{
		MapInputEvent(m_symbolMap[it.second], m_commandMap[it.first]);
	}
}


void InputHandler::Update()
{
	for (auto& it : m_inputEventMap)
	{
		m_InputState[it.second] = IsKeyDown(it.first);
	}
}

const bool InputHandler::IsActionKeyPressed(size_t action_key) const
{
	return bool(m_InputState[action_key]);
}

const CursorPosition& InputHandler::GetMouseCoordinates() {
	GetCursorPos((LPPOINT)(&m_cursorPosition));
	ScreenToClient(m_hwnd, (LPPOINT)(&m_cursorPosition));
	return m_cursorPosition;
}


void InputHandler::LoadConfiguration()
{
	INIReader reader(m_strMapFilePath.c_str());
	assert(reader.ParseError() >= 0);
	auto fields = reader.GetFields("Keyboard");
	m_commandVector.clear();
	m_InputState.resize(fields.size());

	size_t nCommand = 0;
	for (auto& field : fields)
	{
		std::string strCommand = field;
		std::string strSymbol = reader.Get("Keyboard", field, "");

		m_commandSymbolMap[strCommand] = strSymbol;
		m_commandMap[strCommand] = nCommand;
		m_commandVector.push_back(field);
		nCommand++;
	}
}

const std::vector<std::string>& InputHandler::GetFields() const {
	return m_commandVector;
}