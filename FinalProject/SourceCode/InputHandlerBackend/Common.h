#pragma once

#pragma warning(disable:4251)

#ifdef INPUTHANDLERBACKEND_EXPORTS
#define INPUTHANDLERBACKEND_API __declspec(dllexport)
#else
#define INPUTHANDLERBACKEND_API __declspec(dllimport)
#endif
