#include "bulk.h"
#include "logger.h"

#include <exception>
#include <iostream>
#include <limits>
#include <string>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Usage: bulk N\n";
		return 1;
	}

	try {
		std::string argument = argv[1];
		if (argument.empty() || argument.find_first_not_of("0123456789") != std::string::npos) {
			std::cerr << "N must be a positive integer\n";
			return 1;
		}

		auto size = std::stoull(argument);
		if (size == 0 || size > std::numeric_limits<std::size_t>::max()) {
			std::cerr << "N must be a positive integer\n";
			return 1;
		}

		ConsoleLogger console;
		FileLogger file;
		BulkParser parser(static_cast<std::size_t>(size));
		parser.addLogger(console);
		parser.addLogger(file);

		std::string command;
		while (std::getline(std::cin, command)) {
			parser.process(command);
		}
		parser.finish();
	} catch (const std::exception& error) {
		std::cerr << error.what() << '\n';
		return 1;
	}

	return 0;
}
