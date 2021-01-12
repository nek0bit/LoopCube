#include "config.hpp"

Config::Config() : stringValues{}, intValues{}
{}

void Config::set(int key, std::string value)
{
	stringValues[key] = value;
}

void Config::set(int key, int value)
{
	intValues[key] = value;
}

std::string Config::getString(int key)
{
	return stringValues[key];
}

int Config::getInt(int key)
{
	return intValues[key];
}
