/*
/  ----------------------------------------------
/  Window.h
/
/  This header / class file provides a decent 
/  enough abstraction of the Win32API window 
/  class register process.
/  
/  Additional Window Message Handling System for 
/  actions and events that take place in the 
/  specific instance of the window. Further 
/  delegated amongst specific handling classes; 
/  e.g. Keyboard.cpp and Mouse.cpp.
/  
/  Additional functionality for multiple windows.
/  
/  Additional functionality for handling 
/  exceptions.
/  ----------------------------------------------
*/
#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "PCWindows.h"
#include <sstream>
#include <optional>
#include <memory>
#include <iostream>
#include <tchar.h>
#include <hidusage.h>

namespace GID {
	namespace DSU {
		
	}
}
