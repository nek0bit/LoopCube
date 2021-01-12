#pragma once

#include <iostream>
#include <unordered_map>

// A wrapper for multiple std::unordered_maps for config
struct Config
{
	Config();
	~Config() = default;

	void set(int key, std::string value);
	void set(int key, int value);
	std::string getString(int key);
	int getInt(int key);
private:
	// enum/number, string
	std::unordered_map<int, std::string> stringValues;
	// enum/number, int
	std::unordered_map<int, int> intValues;	
};
