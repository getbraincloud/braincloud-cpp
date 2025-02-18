#pragma once
#include <string>

class DataUtilities {
public : 
	static std::string CompressString(const std::string& uncompressedString);
	static std::string DecompressString(const std::string& compressedString);
};