#include "bayan.h"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <algorithm>
#include <climits>
#include <cstdio>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>

namespace fs = boost::filesystem;

Bayan::Bayan(const Options& options) : options(options) {
	for (const auto& directory : options.excluded) {
		auto path = fs::weakly_canonical(fs::absolute(directory));
		while (path != path.root_path() &&
			(path.filename() == "." || path.filename().empty())) {
			path = path.parent_path();
		}
		excluded.push_back(path);
	}

	for (const auto& mask : options.masks) {
		std::string pattern;
		for (char c : mask) {
			if (c == '*') {
				pattern += ".*";
			} else if (c == '?') {
				pattern += '.';
			} else {
				if (std::string(".\\+^$()[]{}|").find(c) != std::string::npos) {
					pattern += '\\';
				}
				pattern += c;
			}
		}
		masks.emplace_back(pattern, std::regex::icase);
	}
}

bool Bayan::isExcluded(const fs::path& path) const {
	for (const auto& directory : excluded) {
		auto current = path.begin();
		auto part = directory.begin();
		while (current != path.end() && part != directory.end() && *current == *part) {
			++current;
			++part;
		}
		if (part == directory.end()) {
			return true;
		}
	}
	return false;
}

bool Bayan::matches(const fs::path& path) const {
	if (masks.empty()) {
		return true;
	}
	for (const auto& mask : masks) {
		if (std::regex_match(path.filename().string(), mask)) {
			return true;
		}
	}
	return false;
}

void Bayan::scan(const fs::path& directory, int depth, std::vector<fs::path>& files) const {
	if (isExcluded(directory)) {
		return;
	}
	for (const auto& entry : fs::directory_iterator(directory)) {
		auto status = entry.symlink_status();
		if (fs::is_directory(status)) {
			if (options.depth == -1 || depth < options.depth) {
				scan(entry.path(), depth + 1, files);
			}
		} else if (fs::is_regular_file(status) && matches(entry.path())) {
			files.push_back(entry.path());
		}
	}
}

std::string Bayan::readHash(const fs::path& path, std::uintmax_t offset,
	std::size_t count, std::vector<char>& buffer) const {
	auto closeFile = [](std::FILE* file) { std::fclose(file); };
	std::unique_ptr<std::FILE, decltype(closeFile)> file(
		std::fopen(path.string().c_str(), "rb"), closeFile);
	if (!file) {
		throw std::runtime_error("Cannot open file: " + path.string());
	}
	// Отключаем буферизацию, чтобы не читать данные следующих блоков.
	if (std::setvbuf(file.get(), nullptr, _IONBF, 0) != 0 ||
		std::fseek(file.get(), static_cast<long>(offset), SEEK_SET) != 0) {
		throw std::runtime_error("Cannot seek file: " + path.string());
	}
	std::fill(buffer.begin(), buffer.end(), 0);
	if (std::fread(buffer.data(), 1, count, file.get()) != count) {
		throw std::runtime_error("Cannot read file: " + path.string());
	}

	if (options.hash == "crc32") {
		boost::crc_32_type hash;
		hash.process_bytes(buffer.data(), buffer.size());
		return std::to_string(hash.checksum());
	}
	boost::uuids::detail::md5 hash;
	hash.process_bytes(buffer.data(), buffer.size());
	boost::uuids::detail::md5::digest_type digest;
	hash.get_digest(digest);
	return std::string(reinterpret_cast<const char*>(&digest), sizeof(digest));
}

std::vector<std::vector<fs::path>> Bayan::findDuplicates() const {
	std::vector<fs::path> files;
	for (const auto& directory : options.directories) {
		auto path = fs::canonical(directory);
		if (!fs::is_directory(path)) {
			throw std::runtime_error("Not a directory: " + directory);
		}
		scan(path, 0, files);
	}

	std::map<std::uintmax_t, std::vector<fs::path>> sizes;
	std::set<fs::path> visited;
	for (const auto& file : files) {
		if (!visited.insert(file).second) {
			continue;
		}
		auto size = fs::file_size(file);
		if (size >= options.minSize) {
			if (size > LONG_MAX) {
				throw std::runtime_error("File is too large: " + file.string());
			}
			sizes[size].push_back(file);
		}
	}

	std::vector<std::vector<fs::path>> result;
	std::vector<char> buffer;
	for (const auto& item : sizes) {
		if (item.second.size() < 2) {
			continue;
		}
		std::vector<std::vector<fs::path>> groups{item.second};
		std::uintmax_t offset = 0;
		while (offset < item.first && !groups.empty()) {
			if (buffer.empty()) {
				buffer.resize(options.blockSize);
			}
			auto count = static_cast<std::size_t>(
				std::min<std::uintmax_t>(options.blockSize, item.first - offset));
			std::vector<std::vector<fs::path>> next;
			for (const auto& group : groups) {
				std::map<std::string, std::vector<fs::path>> hashes;
				for (const auto& file : group) {
					hashes[readHash(file, offset, count, buffer)].push_back(file);
				}
				for (auto& hash : hashes) {
					if (hash.second.size() > 1) {
						next.push_back(std::move(hash.second));
					}
				}
			}
			groups = std::move(next);
			offset += count;
		}
		for (auto& group : groups) {
			std::sort(group.begin(), group.end());
			result.push_back(std::move(group));
		}
	}
	return result;
}
