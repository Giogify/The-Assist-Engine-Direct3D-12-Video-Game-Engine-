#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include <vector>

class Inputtable {

public:

private:

	virtual int input(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept = 0;

};