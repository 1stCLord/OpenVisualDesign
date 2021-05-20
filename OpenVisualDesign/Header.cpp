#include "Header.h"
#include "Callable.h"
#include "Utils.hpp"
#include <fstream>
#include <cctype>
#include <cassert>
#include <ranges>

namespace OVD
{
	Header::Header(const std::string& filename) : filename(filename)
	{
		std::ifstream file;
		file.open(filename);
		if (file.is_open())
		{
			file.seekg(0, std::ios::end);
			std::streampos length = file.tellg();
			std::string header_string = std::string(length, 0);
			file.seekg(0, std::ios::beg);
			file.read((char*)header_string.data(), length);

			parse(header_string, scopes);
		}
	}

	void Header::parse(const std::string& header_string, std::vector<Scope>& scopes)
	{
		uint8_t scope_depth = 0;
		size_t current_position = 0, last_position = 0;
		std::vector<std::pair<uint8_t,std::string>> current_scope_name;
		while (current_position != std::string::npos)
		{
			last_position = current_position;
			current_position = header_string.find_first_of("{}", current_position);

			switch (header_string[current_position])
			{
			case '{':
			{
				std::string_view substring = std::string_view(header_string.data() + last_position, header_string.data() + current_position);
				std::string scope_name;
				Scope::get_scope_type(substring, scope_name);
				current_scope_name.push_back({ scope_depth,scope_name });
				++scope_depth;
				break;
			}
			case '}':
				current_scope_name.pop_back();
				--scope_depth;
				break;
			}
			++current_position;
		}
	}
}