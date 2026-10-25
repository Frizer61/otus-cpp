#pragma once

#include <cstddef>
#include <ctime>
#include <functional>
#include <string>
#include <vector>

struct Bulk {
	std::vector<std::string> commands;
	std::time_t time = 0;
};

class BulkParser {
public:
	BulkParser(std::size_t size, std::function<void(const Bulk&)> handler);
	void process(const std::string& command);
	void finish();

private:
	void flush();

	std::size_t size_;
	std::size_t depth_ = 0;
	Bulk bulk_;
	std::function<void(const Bulk&)> handler_;
};
