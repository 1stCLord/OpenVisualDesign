#pragma once
//#include <vector>
//#include <string>

template<class T>
class bar
{
};

class string
{
};

class foo
{
	void normal_func();
	
	[[ovd::callable]]
	bar<string> callable_func_return(){}
	
	[[ovd::defined, ovd::callable]]
	void callable_func_param(string &value);

	[[ovd::defined, ovd::callable]]
	void callable_func_no_param();

	[[ovd::defined, ovd::callable]]
	void callable_func_two_param(int first, int second);
	
	[[ovd::variable]]
	int bar;
};