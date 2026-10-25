#pragma once

#include "bulk.h"

class ConsoleLogger {
public:
	void write(const Bulk& bulk) const;
};

class FileLogger {
public:
	void write(const Bulk& bulk) const;
};
