#include "TestException.h"
#include <sstream>

TestException::TestException(int line, const char* file) noexcept :
	line(line),
	file(file) {}

int TestException::GetLine() const noexcept { return line; }
const std::string& TestException::GetFile() const noexcept { return file; }
const char* TestException::getType() const noexcept { return "Test Exception"; }

std::string TestException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[FILE] " << file << std::endl
		<< "[LINE] " << line << std::endl;
	return oss.str();
}

const char* TestException::what() const noexcept {
	std::ostringstream oss;
	oss << getType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}