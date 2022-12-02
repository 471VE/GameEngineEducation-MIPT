#pragma once

#include "Common.h"
#include "IInputHandlerBackend.h"

#include <memory>

class INPUTHANDLER_API InputHandler {
	public:
		InputHandler();
		void Update();
		const bool IsActionKeyPressed(size_t action_key) const;
		const CursorPosition& GetMouseCoordinates();
		const std::vector<std::string>& GetFields() const;

	private:
		std::unique_ptr<IInputHandlerBackend> m_inputHandlerBackend;
};