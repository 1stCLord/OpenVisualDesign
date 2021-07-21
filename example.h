#pragma once
#include <files>
//this is an example c++ header file for parsing

class foo
{
	/*
	multiline
	comment
	*/
	
	//other   \
	multiline \
	comment \
	
		
	private:
	//private function not exposed to ovd
	void normal_func();
	
	//function with definition
	void defined_normal_func()
	{
		//function body
		printf("");
	}
	
	class child_class
	{
		void with_defined_member(){}
	};
	
	public:
	//templated callable
	[[ovd::callable]]
	template <class T>
	void callable_func_two_param(int first, int second);
	
	//callable with templated return type
	[[ovd::callable]]
	std::vector<std::string> callable_func_return(){}
	
	//callable with reference parameter
	[[ovd::defined, ovd::callable]]
	void callable_func_param(std::string &value);

	//definable function
	[[ovd::defined, ovd::callable]]
	void callable_func_no_param();

	//definable function with two parameters
	[[ovd::defined, ovd::callable]]
	void callable_func_two_param(int first, int second);
	
	//variable accessable in definable functions
	[[ovd::variable]]
	int bar;
}