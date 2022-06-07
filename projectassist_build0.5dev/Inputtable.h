#pragma once
#include "Mouse.h"
#include "Keyboard.h"

class Inputtable {

public:

	typedef struct INPUT_CREATION_DESC {

	} INPUT_CREATION_DESC;

private:

	virtual int input(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept = 0;

};