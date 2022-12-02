#pragma once

#pragma warning(disable:4251)

#ifdef INPUTHANDLER_EXPORTS
#define INPUTHANDLER_API __declspec(dllexport)
#else
#define INPUTHANDLER_API __declspec(dllimport)
#endif