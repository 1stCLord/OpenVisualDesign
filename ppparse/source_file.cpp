#include "source_file.h"
#include "scope.h"
#include "graph_node.h"
#include <fstream>

namespace ppparse
{

	source_file::source_file(const std::filesystem::path &path) : graph_node(nullptr, std::string_view(), node_type::file)
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
		print();
	}
	
	void source_file::add(std::unique_ptr<graph_node> &&node)
	{
		node_list.push_back(std::move(node));
	}

	void source_file::print(graph_node const *node, std::ofstream &file, int depth)
	{
		char tab = '\t', nl = '\n';
		file.write("\nline ---", sizeof("\nline ---"));
		for(int i = 0; i < depth; ++i)
			file.write(&tab, 1);

		if(node->children.empty())
			file.write(node->node_body.data(), node->node_body.length());
		else
		{
			if (node->type == node_type::scope)
			{
				for (graph_node const* child : ((scope*)node)->preface_children)
					print(child, file, depth);
			}

			for (graph_node const* child : node->children)
			{
				print(child, file, node->type == node_type::expression ?  depth : depth + 1);
			}
		}
	}

	void source_file::print() const
	{
		std::ofstream file;
		file.open("tmp.txt");
		if (file.is_open())
		{
			print(this, file, 0);
			//file.write()
		}
	}

	std::vector<graph_node const *> source_file::get_elements_of_type(node_type type) const
	{
		std::vector<graph_node const*> results;
		for (const std::unique_ptr<graph_node> &node : node_list)
		{
			if (node->type == type)
				results.push_back(node.get());
		}
		return results;
	}
}