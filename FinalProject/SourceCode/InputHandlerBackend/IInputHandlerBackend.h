#pragma once

#include "Common.h"

#include <string>
#include <vector>

struct CursorPosition {
	long x, y;
};

class INPUTHANDLERBACKEND_API IInputHandlerBackend {
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual const bool IsActionKeyPressed(size_t action_key) const = 0;
	virtual const CursorPosition& GetMouseCoordinates() = 0;
	virtual const std::vector<std::string>& GetFields() const = 0;
};