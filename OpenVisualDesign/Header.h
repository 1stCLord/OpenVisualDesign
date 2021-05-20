#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include "Scope.h"

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

		std::vector<Scope> scopes;

		static void parse(const std::string& header_string, std::vector<Scope>& scopes);
	};
}