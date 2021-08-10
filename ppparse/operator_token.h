#pragma once
namespace ppparse
{
	const std::array<std::string, 37> operator_tokens =
	{
		//arithmetic
		"+",
		"-",
		"*",
		"/",
		"%",
		//compound assignment
		"+=",
		"-=",
		"*+",
		"/+",
		"%=",
		">>=",
		"<<=",
		"&=",
		"^=",
		"|=",
		//increment/decrement
		"++",
		"--",
		//relational/comparison
		"==",
		"!=",
		">",
		"<",
		">=",
		"<=",
		"<=>",
		//logical
		"!",
		"&&",
		"||",
		//ternary,comma
		"?",
		",",
		//bitwise
		"&",
		"|",
		"^",
		"~",
		"<<",
		">>",
		".",
		"->"
	};

	/*class operator_token : public graph_node
	{
	public:
		keyword(graph_node* const parent, const std::string_view& node_body) : graph_node(parent, node_body)
		{
			node_name = "operator";
		}
	};*/
}