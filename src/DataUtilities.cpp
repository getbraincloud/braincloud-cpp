#include "braincloud/internal/DataUtilities.h"
#include <iostream>
#include <zlib.h>
#include <vector>
#include "braincloud/internal/DataUtilities.h"

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

std::string DataUtilities::DecompressString(const std::string& compressedString)
{
    if (!IsGZipCompressed(compressedString)) {
        // Data is already decompressed, just return it
        return compressedString;
    }

    z_stream zs{};
    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(compressedString.data()));
    zs.avail_in = static_cast<uInt>(compressedString.size());

    // Initialize with GZip decoding (15 | 16 enables GZip headers)
    if (inflateInit2(&zs, 15 | 16) != Z_OK)
    {
        throw std::runtime_error("Failed to initialize zlib for GZip decompression");
    }

    std::vector<char> decompressedData;
    decompressedData.resize(compressedString.size() * 4); // Initial buffer size

    int result;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(decompressedData.data() + zs.total_out);
        zs.avail_out = static_cast<uInt>(decompressedData.size() - zs.total_out);

        result = inflate(&zs, Z_NO_FLUSH);

        if (result == Z_BUF_ERROR) {
            // Output buffer full, increase its size
            decompressedData.resize(decompressedData.size() * 2);
            continue;
        }
        if (result != Z_OK && result != Z_STREAM_END) {
            inflateEnd(&zs);
            throw std::runtime_error("Decompression failed with error code: " + std::to_string(result));
        }
    } while (result != Z_STREAM_END);

    decompressedData.resize(zs.total_out); // Resize buffer to actual size
    inflateEnd(&zs);

    return std::string(decompressedData.begin(), decompressedData.end()); // Convert to string
}

bool DataUtilities::IsGZipCompressed(const std::string& data)
{
    return data.size() >= 2 &&
        static_cast<unsigned char>(data[0]) == 0x1F &&
        static_cast<unsigned char>(data[1]) == 0x8B;
}
