#include "config.hpp"

Config::Config() : string_values{}, int_values{} {}

Config::~Config() {}

void Config::set(int key, std::string value) {
	string_values[key] = value;
}

void Config::set(int key, int value) {
	int_values[key] = value;
}

std::string Config::get_string(int key) {
	return string_values[key];
}

int Config::get_int(int key) {
	return int_values[key];
}
