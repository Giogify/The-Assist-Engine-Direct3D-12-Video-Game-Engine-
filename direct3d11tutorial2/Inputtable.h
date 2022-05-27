#pragma once
#include "Mouse.h"

class Inputtable {

public:

	typedef struct INPUT_CREATION_DESC {

	} INPUT_CREATION_DESC;

private:

	virtual void input(std::vector<char>& keys, std::vector<Mouse::Event>& mouse) noexcept = 0;

};