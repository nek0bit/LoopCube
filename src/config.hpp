#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__
#include <iostream>
#include <unordered_map>

// Mostly a wrapper for multiple std::maps
class Config {
public:
	Config();
	~Config();

	void set(int key, std::string value);
	void set(int key, int value);
	std::string get_string(int key);
	int get_int(int key);
private:
	// enum/number, string
	std::unordered_map<int, std::string> string_values;
	// enum/number, int
	std::unordered_map<int, int> int_values;
	
};

#endif
