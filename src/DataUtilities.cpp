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
    z_stream zs{};
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    // Initialize with raw deflate (-15) or GZip (15 | 16)
    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        throw std::runtime_error("Failed to initialize zlib for GZip compression");
    }

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(uncompressedString.data()));
    zs.avail_in = static_cast<uInt>(uncompressedString.size());

    std::vector<char> compressedData;
    compressedData.resize(uncompressedString.size() + uncompressedString.size() / 10 + 12); // Better estimation

    zs.next_out = reinterpret_cast<Bytef*>(compressedData.data());
    zs.avail_out = static_cast<uInt>(compressedData.size());

    int ret = deflate(&zs, Z_FINISH);
    if (ret != Z_STREAM_END)
    {
        deflateEnd(&zs);
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(zs.total_out); // Trim to actual size
    deflateEnd(&zs);

    return std::string(compressedData.begin(), compressedData.end()); // Convert to string
}
