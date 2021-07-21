#pragma once
#include <string_view>
#include <vector>

namespace ppparse
{
	class source_file;

	class graph_node
	{
		friend source_file;

	public:
		graph_node(graph_node * const parent, const std::string_view& node_body);
	private:
		source_file* get_owner();

		std::string_view node_body;

		graph_node * const parent;
		std::vector<graph_node *> children;

	protected:
		//parsing functions
		void parse_block();
		void comments_and_preprocessor_directives(size_t& position);
		void top_level_scope(size_t& position);
		void parse_scope_or_expression(size_t& position);
		void skip_whitespace(size_t& position);
		bool is_end(size_t position);
	};
}