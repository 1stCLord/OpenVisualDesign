#include "source_file.h"
#include <fstream>

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
		node_body = std::string_view(entire_file.data(), entire_file.size());
		parse_block();
		printf("bloop");

	}
	
	void source_file::add(std::unique_ptr<graph_node> &&node)
	{
		node_list.push_back(std::move(node));
	}
}