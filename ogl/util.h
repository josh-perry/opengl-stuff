#pragma once
#include <string>
#include <fstream>

std::string load_string_from_file(const char* filename)
{
	std::ifstream shader_file { filename };
	std::string contents
	{
		std::istreambuf_iterator<char>(shader_file),
		std::istreambuf_iterator<char>()
	};

	return contents;
}
