#pragma once


namespace OVD
{

	template<class token_type, class callable_type>
	void tokenise(const std::string_view& source, token_type token, callable_type callable, bool include_last = true)
	{
		size_t current_position = source.find(token);
		size_t last_position = 0;
		while (current_position != std::string::npos)
		{
			callable(last_position, current_position);
			last_position = current_position + 1;
			current_position = source.find(token, current_position + 1);
		}

		if (include_last)
		{
			size_t size = source.size();
			if (size)
				callable(last_position, size);
		}
	}
}