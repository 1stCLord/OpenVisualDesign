#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include "Scope.h"
#include "source_file.h"

namespace OVD
{
	class Header
	{
	public:
		Header(const std::string& filename);

		const std::vector<Scope>& get_scopes() const { return scopes; }
		std::vector<Scope>& get_scopes() { return scopes; }

		const std::filesystem::path& get_filename() const { return filename; }

	private:
		std::filesystem::path filename;

		//old half baked headers
		std::vector<Scope> scopes;
		static void parse(const std::string& header_string, std::vector<Scope>& scopes);

		//new headers using ppparse
		ppparse::source_file ppparse_file;
		std::vector<ppparse::graph_node const *> ppparse_scopes;
		static void parse(const ppparse::source_file& source_file, std::vector<ppparse::graph_node const *>& scopes);
	};
}