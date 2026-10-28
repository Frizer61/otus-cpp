#pragma once

#include "ILogger.h"

#include <ctime>

class ConsoleLogger : public ILogger {
public:
	void write(const Bulk& bulk) override;
};

class FileLogger : public ILogger {
public:
	void onBulkStart() override;
	void write(const Bulk& bulk) override;

private:
	std::time_t time_ = 0;
};
