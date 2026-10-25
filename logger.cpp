#include "logger.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void writeBulk(std::ostream& output, const Bulk& bulk) {
	output << "bulk: ";
	for (std::size_t i = 0; i < bulk.commands.size(); ++i) {
		if (i > 0) {
			output << ", ";
		}
		output << bulk.commands[i];
	}
	output << '\n';
}

}

void ConsoleLogger::write(const Bulk& bulk) const {
	writeBulk(std::cout, bulk);
	std::cout.flush();
}

void FileLogger::write(const Bulk& bulk) const {
	std::string base = "bulk" + std::to_string(bulk.time);
	std::string filename = base + ".log";
	int number = 1;
	// Несколько пакетов могут начаться в одну секунду.
	while (std::ifstream(filename).good()) {
		filename = base + "_" + std::to_string(number++) + ".log";
	}

	std::ofstream file(filename);
	if (!file) {
		throw std::runtime_error("Cannot open log file: " + filename);
	}
	writeBulk(file, bulk);
	file.close();
	if (!file) {
		throw std::runtime_error("Cannot write log file: " + filename);
	}
}
