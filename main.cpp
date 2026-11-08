#include "bayan.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <limits>
#include <stdexcept>

int main(int argc, char* argv[]) {
	try {
		Options options;
		long long minSize = 2;
		long long blockSize = 4096;
		namespace po = boost::program_options;
		po::options_description description("bayan options");
		description.add_options()
			("help,h", "Show help")
			("scan,d", po::value(&options.directories)->multitoken()->composing(),
				"Directories to scan (default: current directory)")
			("exclude,e", po::value(&options.excluded)->multitoken()->composing(),
				"Directories to exclude")
			("depth,l", po::value(&options.depth)->default_value(0),
				"Scan depth: 0 = root only, -1 = unlimited")
			("min-size,m", po::value(&minSize)->default_value(2),
				"Minimum file size in bytes")
			("mask,f", po::value(&options.masks)->multitoken()->composing(),
				"Filename masks: * and ?, case insensitive")
			("block-size,s", po::value(&blockSize)->default_value(4096),
				"Block size in bytes")
			("hash,a", po::value(&options.hash)->default_value("crc32"),
				"Hash algorithm: crc32 or md5");

		po::variables_map arguments;
		po::store(po::parse_command_line(argc, argv, description), arguments);
		if (arguments.count("help")) {
			std::cout << description << '\n';
			return 0;
		}
		po::notify(arguments);

		if (options.depth < -1) {
			throw std::runtime_error("Depth must be -1 or greater");
		}
		if (minSize < 0) {
			throw std::runtime_error("Minimum file size must not be negative");
		}
		if (blockSize <= 0 || static_cast<std::uintmax_t>(blockSize) >
			std::numeric_limits<std::size_t>::max()) {
			throw std::runtime_error("Invalid block size");
		}
		options.minSize = static_cast<std::uintmax_t>(minSize);
		options.blockSize = static_cast<std::size_t>(blockSize);
		if (options.hash != "crc32" && options.hash != "md5") {
			throw std::runtime_error("Hash must be crc32 or md5");
		}
		if (options.directories.empty()) {
			options.directories.push_back(".");
		}

		Bayan bayan(options);
		auto groups = bayan.findDuplicates();
		for (std::size_t i = 0; i < groups.size(); ++i) {
			if (i > 0) {
				std::cout << '\n';
			}
			for (const auto& file : groups[i]) {
				std::cout << file.string() << '\n';
			}
		}
	} catch (const std::exception& error) {
		std::cerr << "Error: " << error.what() << '\n';
		return 1;
	}
	return 0;
}
