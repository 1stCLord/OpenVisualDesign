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
	private:
		std::string entire_file;
		std::vector<std::unique_ptr<graph_node>> node_list;
	};
}