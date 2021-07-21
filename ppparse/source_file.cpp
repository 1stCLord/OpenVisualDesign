#include "source_file.h"
#include <fstream>
#include "comment.h"
#include "preprocessor_directive.h"
#include "scope.h"
#include "expression.h"

namespace ppparse
{

	source_file::source_file(const std::filesystem::path &path) : graph_node(nullptr, std::string_view())
	{
		std::ifstream file;
		file.open(path);
		if (file.is_open())
		{
			file.seekg(0, std::ios_base::end);
			std::streampos length = file.tellg();
			entire_file = std::string(length, 0);
			file.seekg(0, std::ios::beg);
			file.read((char*)entire_file.data(), length);
		};
		parse();
		printf("bloop");
	}

	void source_file::parse()
	{
		//break the source file into scopes and expresions
		size_t position = 0;
		do 
		{
			//keep finding comments and preprocessors until there's none left
			size_t previous_position = position;
			do
			{
				skip_whitespace(position);
				previous_position = position;
				comments_and_preprocessor_directives(position);
			} while (!is_end(position) && position != previous_position);

			parse_scope_or_expression(position);
		} while (!is_end(position));
	}

	void source_file::comments_and_preprocessor_directives(size_t &position)
	{
		char end_token = '\n';
		if (entire_file[position] == '#' || (entire_file[position] == '/' && entire_file[position + 1] == '/'))
		{
			size_t begin = position;
			do
			{
				position = entire_file.find('\n', position);
			} while (!is_end(position) && entire_file[position-1] == '\\');

			if (entire_file[begin] == '#')
				node_list.push_back(std::make_unique<preprocessor_directive>(this, std::string_view(entire_file.data() + begin, entire_file.data() + position)));
			else
				node_list.push_back(std::make_unique<comment>(this, std::string_view(entire_file.data() + begin, entire_file.data() + position)));
			++position;
		}
		else if (entire_file[position] == '/' && entire_file[position+1] == '*')
		{
			size_t begin = position;
			position = entire_file.find("*/", position);
			node_list.push_back(std::make_unique<comment>(this, std::string_view(entire_file.data() + begin, entire_file.data() + position)));
			++position;
		}
	}

	void source_file::top_level_scope(size_t &position)
	{
		uint16_t current_scope_depth = 1;
		do
		{
			position = entire_file.find_first_of("{}", position+1);
			switch (entire_file[position])
			{
			case '{':
				++current_scope_depth;
				break;
			case '}':
				--current_scope_depth;
				break;
			}
		} while (!is_end(position) && current_scope_depth > 0);
		++position;
	}

	void source_file::parse_scope_or_expression(size_t& position)
	{
		skip_whitespace(position);
		size_t last_position = position;
		position = entire_file.find_first_of("{;", position);
		size_t scope_begin = position;
		if (!is_end(position))
		{
			switch (entire_file[position])
			{
			case '{':
				top_level_scope(position);
				//last position to scope begin is the scope preface, scope_begin to position is a scope body
				node_list.push_back(std::make_unique<scope>(this, std::string_view(entire_file.data() + last_position, entire_file.data() + scope_begin), std::string_view(entire_file.data() + scope_begin, entire_file.data() + position)));
				break;
			case ';':
				//last position to position is an expression
				node_list.push_back(std::make_unique<expression>(this, std::string_view(entire_file.data() + last_position, entire_file.data() + position)));
				break;
			}
		}
	}

	void source_file::skip_whitespace(size_t& position)
	{
		while (!is_end(position) && std::isspace(entire_file[position]))++position;
	}

	bool source_file::is_end(size_t position)
	{
		return position == std::string::npos || position >= entire_file.size();
	}
}