#pragma once
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
		void add(std::unique_ptr<graph_node> &&node);
		void print() const;

		std::vector<graph_node const *> get_elements_of_type(node_type type);
	private:
		std::string entire_file;
		std::vector<std::unique_ptr<graph_node>> node_list;

		static void print(graph_node const* node, std::ofstream& file, int depth);
	};
}