#include "InputHandler.h"
#include "WinAPIBackend.h"

InputHandler::InputHandler() {
	m_inputHandlerBackend = std::make_unique<WinAPIBackend>();
	m_inputHandlerBackend->Init();
}

void InputHandler::Update() {
	m_inputHandlerBackend->Update();
}

const bool InputHandler::IsActionKeyPressed(size_t action_key) const {
	return m_inputHandlerBackend->IsActionKeyPressed(action_key);
}

const CursorPosition& InputHandler::GetMouseCoordinates() {
	return m_inputHandlerBackend->GetMouseCoordinates();
}

const std::vector<std::string>& InputHandler::GetFields() const {
	return m_inputHandlerBackend->GetFields();
}