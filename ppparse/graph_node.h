#pragma once
#include <string_view>
#include <vector>
#include <span>

namespace ppparse
{
	class source_file;
	class scope;

	enum class node_type
	{
		symbol,
		static_member,
		colon,
		semi_colon,
		comment,
		literal,
		keyword,
		operator_token,
		preprocessor_directive,
		attribute,
		function_parameters,
		template_parameters,
		scope,
		expression,
		file,
		unknown
	};

	class graph_node
	{
		friend source_file;

	public:
		graph_node(graph_node * const parent, const std::string_view& node_body, node_type type);

		virtual void add(graph_node* const child);
		const std::string_view &get_node_body() const;

		virtual const std::string_view get_name() const;

		const std::vector<graph_node*> &get_children() const;

		std::string get_qualified_scope() const;

		const node_type type;
		graph_node* const parent;

		template<class Predicate>
		graph_node* ascend_until(const Predicate &predicate)
		{
			if (predicate(this))return this;
			else if (parent != nullptr) return parent->ascend_until(predicate);
			else return nullptr;
		}

		std::vector<std::span<graph_node * const>> children_as_parameter_list_members() const;
	private:
		source_file* get_owner();		

		std::string_view node_body;
	protected:
		std::vector<graph_node*> children;

	protected:
		//parsing functions
		void parse_block();
		void parse_expression(const std::string_view &to_parse);
		size_t parse_section(size_t& position, const std::string_view& source, const std::string_view &open_token, const std::string_view &close_token);
		void comments_and_preprocessor_directives(size_t& position, const std::string_view &source);
		void attributes(size_t& position, const std::string_view& source);
		void template_parameters(size_t& position, const std::string_view& source);
		void keywords(size_t& position, const std::string_view& source);
		void function_parameters(size_t& position, const std::string_view& source);
		void literals(size_t& position, const std::string_view& source);
		void colons(size_t& position, const std::string_view& source);
		void operator_token(size_t& position, const std::string_view& source);
		void consume_symbol(size_t& position, const std::string_view& source);
		void top_level_scope(size_t& position, const std::string_view& source);
		void parse_scope_or_expression(size_t& position, const std::string_view& source);
		void skip_whitespace(size_t& position, const std::string_view& source);
		bool is_end(size_t position, const std::string_view& source);
	};

	//used to turn a bunch of symbols and static member qualifiers into a fully qualified name string view
	template<class iterator>
	std::string_view parse_name(iterator it, iterator end)
	{
		std::string_view name;
		while (it != end && ((*it)->type == node_type::symbol || (*it)->type == node_type::static_member))
		{
			if (name.empty())
				name = (*it)->get_node_body();
			else
				name = std::string_view(&name.front(), &((*it)->get_node_body().back())+1);
			++it;
		}
		return name;
	}
}