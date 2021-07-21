#include "graph_node.h"
#include "source_file.h"
#include "expression.h"
#include "scope.h"
#include "comment.h"
#include "preprocessor_directive.h"

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
				skip_whitespace(position);
				previous_position = position;
				comments_and_preprocessor_directives(position);
			} while (!is_end(position) && position != previous_position);

			parse_scope_or_expression(position);
		} while (!is_end(position));
	}

	void graph_node::comments_and_preprocessor_directives(size_t& position)
	{
		if (is_end(position))return;

		char end_token = '\n';
		if (node_body[position] == '#' || (node_body[position] == '/' && node_body[position + 1] == '/'))
		{
			size_t begin = position;
			do
			{
				position = node_body.find('\n', position+1);
			} while (!is_end(position) && node_body[position - 1] == '\\');

			if (node_body[begin] == '#')
				get_owner()->add(std::make_unique<preprocessor_directive>(this, std::string_view(node_body.data() + begin, node_body.data() + position)));
			else
				get_owner()->add(std::make_unique<comment>(this, std::string_view(node_body.data() + begin, node_body.data() + position)));
			++position;
		}
		else if (node_body[position] == '/' && node_body[position + 1] == '*')
		{
			size_t begin = position;
			position = node_body.find("*/", position);
			get_owner()->add(std::make_unique<comment>(this, std::string_view(node_body.data() + begin, node_body.data() + position)));
			position+=2;
		}
	}

	void graph_node::top_level_scope(size_t& position)
	{
		uint16_t current_scope_depth = 1;
		do
		{
			position = node_body.find_first_of("{}", position + 1);
			switch (node_body[position])
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

	void graph_node::parse_scope_or_expression(size_t& position)
	{
		skip_whitespace(position);
		size_t last_position = position;
		position = node_body.find_first_of("{;", position);
		size_t scope_begin = position;
		if (!is_end(position))
		{
			switch (node_body[position])
			{
			case '{':
				top_level_scope(position);
				//last position to scope begin is the scope preface, scope_begin to position is a scope body
				get_owner()->add(std::make_unique<scope>(this, std::string_view(node_body.data() + last_position, node_body.data() + scope_begin), std::string_view(node_body.data() + scope_begin, node_body.data() + position)));
				break;
			case ';':
				//last position to position is an expression
				get_owner()->add(std::make_unique<expression>(this, std::string_view(node_body.data() + last_position, node_body.data() + ++position)));
				break;
			}
		}
	}

	void graph_node::skip_whitespace(size_t& position)
	{
		while (!is_end(position) && std::isspace(node_body[position]))++position;
	}

	bool graph_node::is_end(size_t position)
	{
		return position == std::string::npos || position >= node_body.size();
	}

}