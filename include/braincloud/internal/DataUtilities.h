#pragma once
#include <vector>

class DataUtilities {
public : 
	static std::vector<uint8_t> CompressBytes(const std::vector<uint8_t>& uncompressedData);
	static std::string CompressString(const std::string& uncompressedString);
};