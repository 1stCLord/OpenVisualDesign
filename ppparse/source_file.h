#pragma once
#include <string_view>
#include <filesystem>
#include <memory>
#include <string>
#include "graph_node.h"

namespace ppparse
{
	class source_file : public graph_node
	{
	public:
		source_file(const std::filesystem::path &path);
		void parse();
	private:
		std::string entire_file;
		std::vector<std::unique_ptr<graph_node>> node_list;
		graph_node* root_node;

		//parsing functions
		void comments_and_preprocessor_directives(size_t &position);
		void top_level_scope(size_t &position);
		void parse_scope_or_expression(size_t &position);
		void skip_whitespace(size_t &position);
		bool is_end(size_t position);
	};
}