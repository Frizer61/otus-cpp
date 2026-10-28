#pragma once

#include "ILogger.h"

#include <cstddef>
#include <string>
#include <vector>

struct Bulk {
	std::vector<std::string> commands;
};

class BulkParser {
public:
	explicit BulkParser(std::size_t size);
	void addLogger(ILogger& logger);
	void process(const std::string& command);
	void finish();

private:
	void flush();

	std::size_t size_;
	std::size_t depth_ = 0;
	Bulk bulk_;
	// Логгеры должны существовать всё время работы парсера.
	std::vector<ILogger*> loggers_;
};
