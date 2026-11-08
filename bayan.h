#pragma once

#include <boost/filesystem.hpp>

#include <cstddef>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

struct Options {
	std::vector<std::string> directories;
	std::vector<std::string> excluded;
	std::vector<std::string> masks;
	int depth = 0;
	std::uintmax_t minSize = 2;
	std::size_t blockSize = 4096;
	std::string hash = "crc32";
};

class Bayan {
	Options options;
	std::vector<boost::filesystem::path> excluded;
	std::vector<std::regex> masks;

	bool isExcluded(const boost::filesystem::path& path) const;
	bool matches(const boost::filesystem::path& path) const;
	void scan(const boost::filesystem::path& directory, int depth,
		std::vector<boost::filesystem::path>& files) const;
	std::string readHash(const boost::filesystem::path& path, std::uintmax_t offset,
		std::size_t count, std::vector<char>& buffer) const;

public:
	explicit Bayan(const Options& options);
	std::vector<std::vector<boost::filesystem::path>> findDuplicates() const;
};
