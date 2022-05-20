#pragma once
#include <exception>
#include <string>

class TestException : public std::exception {
	
private:

	int line;
	std::string file;

protected:

	mutable std::string whatBuffer;

public:

	TestException(int, const char*) noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	virtual const char* getType() const noexcept;
	std::string GetOriginString() const noexcept;
	const char* what() const noexcept override;

};