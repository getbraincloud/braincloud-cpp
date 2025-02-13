#include <iostream>
#include <zlib.h>
#include "braincloud/internal/DataUtilities.h"


std::vector<uint8_t> DataUtilities::CompressBytes(const std::vector<uint8_t>& uncompressedData)
{
    std::vector<uint8_t> compressedData;

    if (uncompressedData.empty()) {
        return compressedData;
    }

    // Compute the upper bound of the compressed size
    uLongf compressedSize = compressBound(uncompressedData.size());
    compressedData.resize(compressedSize);

    // Perform compression
    int result = compress2(compressedData.data(), &compressedSize, uncompressedData.data(), uncompressedData.size(), Z_BEST_COMPRESSION);

    if (result == Z_OK) {
        compressedData.resize(compressedSize); // Resize to actual compressed size
    }
    else {
        std::cerr << "Failed to compress data. Error code: " << result << std::endl;
        compressedData.clear();
    }

    return compressedData;
}

std::string DataUtilities::CompressString(const std::string& uncompressedString)
{
    uLongf compressedSize = compressBound(uncompressedString.size());  // Get max possible size
    std::string compressedData(compressedSize, '\0');     // Allocate string with space

    if (compress(reinterpret_cast<Bytef*>(&compressedData[0]), &compressedSize,
        reinterpret_cast<const Bytef*>(uncompressedString.data()), uncompressedString.size()) != Z_OK)
    {
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(compressedSize);  // Trim unused space
    return compressedData;
}
