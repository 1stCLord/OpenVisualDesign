#include "graph_node.h"
#include "source_file.h"
#include "expression.h"
#include "scope.h"
#include "comment.h"
#include "keyword.h"
#include "operator_token.h"
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

	graph_node::graph_node(graph_node * const parent, const std::string_view& node_body, node_type type) :
		parent(parent),
		node_body(node_body),
		type(type)
	{
		if(parent != nullptr)
			parent->add(this);
	}

	void graph_node::add(graph_node* const child)
	{
		children.push_back(child);
	}

	const std::string_view& graph_node::get_node_body()
	{
		return node_body;
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

	void graph_node::parse_expression(const std::string_view &to_parse)
	{
		size_t position = 0;
		do
		{
			size_t initial_position = position;
			comments_and_preprocessor_directives(position, to_parse);
			attributes(position, to_parse);
			template_parameters(position, to_parse);
			keywords(position, to_parse);
			function_parameters(position, to_parse);
			colons(position, to_parse);
			literals(position, to_parse);
			if (position == initial_position)
			{
				operator_token(position, to_parse);
				consume_symbol(position, to_parse);
			}
		} while (!is_end(position, to_parse));
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
				get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + begin, source.data() + position), node_type::preprocessor_directive));
			else
				get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + begin, source.data() + position), node_type::comment));
			++position;
		}
		else if (source[position] == '/' && source[position + 1] == '*')
		{
			size_t begin = position;
			position = source.find("*/", position);
			if (is_end(position, source))return;
			get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + begin, source.data() + position), node_type::comment));
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
			get_owner()->add(std::make_unique<expression>(this, std::string_view(source.data() + position + 2, source.data() + result - 2), node_type::attribute));
			position = result;
		}
	}

	void graph_node::template_parameters(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t result = parse_section(position, source, std::string_view("<"), std::string_view(">"));
		if (result != std::string::npos)
		{
			get_owner()->add(std::make_unique<expression>(this, std::string_view(source.data() + position + 1, source.data() + result - 1), node_type::template_parameters));
			position = result;
		}
	}

	void graph_node::keywords(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t end_position = source.find_first_of(" \n\r\t\v\f:[+-*/=&^|!?~", position);
		if (is_end(end_position, source))return;
		std::string_view token = std::string_view(source.data() + position, source.data() + end_position);
		if (std::find(all_keywords.cbegin(), all_keywords.cend(), token) != all_keywords.cend())
		{
			get_owner()->add(std::make_unique<graph_node>(this, token, node_type::keyword));
			position = end_position;
		}
	}

	void graph_node::function_parameters(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t result = parse_section(position, source, std::string_view("("), std::string_view(")"));
		if (result != std::string::npos)
		{
			get_owner()->add(std::make_unique<expression>(this, std::string_view(source.data() + position + 1, source.data() + result - 1), node_type::function_parameters));
			position = result;
		}
	}

	void graph_node::literals(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t result = parse_section(position, source, std::string_view("\""), std::string_view("\""));
		if (result != std::string::npos)
		{
			get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + position + 1, source.data() + result - 1), node_type::literal));
			position = result;
		}
	}


	void graph_node::colons(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;

		if (source[position] == ':')
		{
			if (source.length() > 1 && source[position + 1] == ':')
			{
				get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + position, source.data() + position + 2), node_type::static_member));
				position += 2;
			}
			else
			{
				get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + position, source.data() + position + 1), node_type::colon));
				++position;
			}

		}
	}

	void graph_node::operator_token(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		
		for (const std::string &operator_token : operator_tokens)
		{
			if (source.substr(position).starts_with(operator_token))
			{
				get_owner()->add(std::make_unique<graph_node>(this, std::string_view(source.data() + position, source.data() + position + operator_token.size()), node_type::operator_token));
				position += operator_token.size();
			}

		}
	}

	void graph_node::consume_symbol(size_t& position, const std::string_view& source)
	{
		skip_whitespace(position, source);
		if (is_end(position, source))return;
		size_t end_position = source.find_first_of(" \n\r\t\v\f()<>:[]+-*/=&^|!?~,", position);
		if (end_position == std::string::npos)end_position = source.length();
		std::string_view token = std::string_view(source.data() + position, source.data() + end_position);

		if(token.size() && token[0] == token[token.size()-1] && token[0] == '\"')
			get_owner()->add(std::make_unique<graph_node>(this, token, node_type::literal));
		else
			get_owner()->add(std::make_unique<graph_node>(this, token, node_type::symbol));
		position = end_position;
	}

	void graph_node::top_level_scope(size_t& position, const std::string_view& source)
	{
		uint16_t current_scope_depth = 1;
		do
		{
			position = source.find_first_of("{}", position + 1);
			if (is_end(position, source))return;
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
				get_owner()->add(std::make_unique<expression>(this, std::string_view(source.data() + last_position, source.data() + position++)));
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