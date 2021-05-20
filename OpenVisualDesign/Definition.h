#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Callable.h"

namespace OVD
{
	class Definition
	{
	public:
		class Callee
		{
		public:
			Callable const * callable;
			std::vector<Callee const *> parameter_sources;
			Callee(Callable const* callable) : callable(callable), parameter_sources(callable->parameters.size(),nullptr)
			{
			}
		};

		Definition(const Callable& callable) : callable(callable) {}

		const Callable& get_callable() const { return callable; }
		std::vector<std::unique_ptr<Callee>>& get_callees() { return callees; }
	private:
		Callable callable;
		std::vector<std::unique_ptr<Callee>> callees;
	};
}