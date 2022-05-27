#pragma once
#include "Mouse.h"

class Inputtable {

public:

	typedef struct INPUT_CREATION_DESC {

	} INPUT_CREATION_DESC;

private:

	virtual void input(const std::vector<char>& keys, const std::vector<Mouse::Event>& mouse) noexcept = 0;

};