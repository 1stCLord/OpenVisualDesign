#include "graph_node.h"
#include "source_file.h"
#include "expression.h"
#include "scope.h"
#include "comment.h"
#include "keyword.h"
#include "preprocessor_directive.h"
#include <iostream>

namespace ppparse
{
	source_file* graph_node::get_owner()
	{
		graph_node* iterator = this;
		while (iterator->parent != nullptr)iterator = iterator->parent;
		return (source_file*)(iterator);
	}

	graph_node::graph_node(graph_node * const parent, const std::string_view& node_body) :
		parent(parent),
		node_body(node_body)
	{
		if(parent != nullptr)
			parent->children.push_back(this);
	}

	void graph_node::parse_block()
	{
		//break the source file into scopes and expresions
		size_t position = 0;
		do
		{
			//keep finding comments and preprocessors until there's none left
			size_t previous_position = position;
			do
			{
				skip_whitespace(position, node_body);
				previous_position = position;
				comments_and_preprocessor_directives(position, node_body);
			} while (!is_end(position, node_body) && position != previous_position);

			parse_scope_or_expression(position, node_body);
		} while (!is_end(position, node_body));
	}

	size_t graph_node::parse_section(size_t& position, const std::string_view& source, const std::string_view &open_token, const std::string_view &close_token)
	{
		if (is_end(position, source))std::string::npos;

		std::string_view initial_tokens = source.substr(position, open_token.size());
		if (initial_tokens == open_token)
		{
			return source.find(close_token,position+initial_tokens.size())+close_token.size();
		}
		return std::string::npos;
	}

	void graph_node::comments_and_preprocessor_directives(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;

		char end_token = '\n';
		if (source[position] == '#' || (source[position] == '/' && source[position + 1] == '/'))
		{
			size_t begin = position;
			do
			{
				position = source.find('\n', position+1);
			} while (!is_end(position, source) && source[position - 1] == '\\');

			if (source[begin] == '#')
				get_owner()->add(std::make_unique<preprocessor_directive>(this, std::string_view(source.data() + begin, source.data() + position)));
			else
				get_owner()->add(std::make_unique<comment>(this, std::string_view(source.data() + begin, source.data() + position)));
			++position;
		}
		else if (source[position] == '/' && source[position + 1] == '*')
		{
			size_t begin = position;
			position = source.find("*/", position);
			get_owner()->add(std::make_unique<comment>(this, std::string_view(source.data() + begin, source.data() + position)));
			position+=2;
		}
	}

	void graph_node::attributes(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;

		size_t result = parse_section(position, source, std::string_view("[["), std::string_view("]]"));
		if (result != std::string::npos)
		{
			std::cout << "attributes - " << source.substr(position, result - position) << "\n";
			//todo
			//get_owner()->add(std::make_unique<attribute>(this, std::string_view(source.data() + position, source.data() + result)));
			position = result;
		}
	}

	void graph_node::template_parameters(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		//todo collect 
		size_t result = parse_section(position, source, std::string_view("<"), std::string_view(">"));
		if (result != std::string::npos)
		{
			std::cout << "template params - " << source.substr(position, result - position) << "\n";
			position = result;
		}
	}

	void graph_node::keywords(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t end_position = source.find_first_of(" \n\r\t\v\f", position);
		std::string_view token = std::string_view(source.data() + position, source.data() + end_position);
		if (std::find(all_keywords.cbegin(), all_keywords.cend(), token) != all_keywords.cend())
		{
			get_owner()->add(std::make_unique<keyword>(this, token));
			position = end_position;
			std::cout << "keyword - " << token << "\n";
		}
	}

	void graph_node::function_parameters(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t result = parse_section(position, source, std::string_view("("), std::string_view(")"));
		if (result != std::string::npos)
		{
			std::cout << "function params - " << source.substr(position, result - position) << "\n";
			position = result;
		}
	}

	void graph_node::consume_symbol(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t end_position = source.find_first_of(" \n\r\t\v\f(<[", position);
		std::string_view token = std::string_view(source.data() + position, source.data() + end_position);
		//todo consume symbol
		position = end_position;
		std::cout << "symbol - " << token << "\n";
	}

	void graph_node::top_level_scope(size_t& position, const std::string_view& source)
	{
		uint16_t current_scope_depth = 1;
		do
		{
			position = source.find_first_of("{}", position + 1);
			switch (source[position])
			{
			case '{':
				++current_scope_depth;
				break;
			case '}':
				--current_scope_depth;
				break;
			}
		} while (!is_end(position, source) && current_scope_depth > 0);
		++position;
	}

	void graph_node::parse_scope_or_expression(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		size_t last_position = position;
		position = source.find_first_of("{;", position);
		size_t scope_begin = position;
		if (!is_end(position, source))
		{
			switch (source[position])
			{
			case '{':
				top_level_scope(position, source);
				//last position to scope begin is the scope preface, scope_begin to position is a scope body
				get_owner()->add(std::make_unique<scope>(this, std::string_view(source.data() + last_position, source.data() + scope_begin), std::string_view(source.data() + scope_begin, source.data() + position)));
				break;
			case ';':
				//last position to position is an expression
				get_owner()->add(std::make_unique<expression>(this, std::string_view(source.data() + last_position, source.data() + ++position)));
				break;
			}
		}
	}

	void graph_node::skip_whitespace(size_t& position, const std::string_view& source)
	{
		while (!is_end(position, source) && std::isspace(source[position]))++position;
	}

	bool graph_node::is_end(size_t position, const std::string_view& source)
	{
		return position == std::string::npos || position >= source.size();
	}

}