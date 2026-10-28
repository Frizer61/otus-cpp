#include "bulk.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class TestLogger : public ILogger {
public:
	void onBulkStart() override {
		++starts;
	}

	void write(const Bulk& bulk) override {
		result.push_back(bulk.commands);
	}

	int starts = 0;
	std::vector<std::vector<std::string>> result;
};

void check(const std::string& name, std::size_t size, const std::string& input,
	const std::vector<std::vector<std::string>>& expected) {
	TestLogger logger;
	TestLogger anotherLogger;
	BulkParser parser(size);
	parser.addLogger(logger);
	parser.addLogger(anotherLogger);

	std::istringstream stream(input);
	std::string command;
	while (std::getline(stream, command)) {
		parser.process(command);
	}
	parser.finish();
	parser.finish();
	if (logger.result != expected || anotherLogger.result != expected) {
		throw std::runtime_error("Failed: " + name);
	}
}

void checkNotifications() {
	TestLogger logger;
	BulkParser parser(2);
	parser.addLogger(logger);
	parser.process("{");
	parser.process("}");
	if (logger.starts != 0 || !logger.result.empty()) {
		throw std::runtime_error("Empty block must not notify loggers");
	}

	parser.process("cmd1");
	parser.process("cmd2");
	parser.process("{");
	parser.process("cmd3");
	parser.finish();
	// Незакрытый блок начался, но не должен попасть в вывод.
	if (logger.starts != 2 || logger.result.size() != 1) {
		throw std::runtime_error("Incorrect logger notifications");
	}

	parser.process("cmd4");
	parser.finish();
	if (logger.starts != 3 || logger.result.back() != std::vector<std::string>{"cmd4"}) {
		throw std::runtime_error("New block must notify loggers again");
	}
}

int main() {
	try {
		checkNotifications();
		check("static blocks", 3, "cmd1\ncmd2\ncmd3\ncmd4\n",
			{{"cmd1", "cmd2", "cmd3"}, {"cmd4"}});
		check("dynamic block", 3, "cmd1\n{\ncmd2\ncmd3\ncmd4\ncmd5\n}\ncmd6\n",
			{{"cmd1"}, {"cmd2", "cmd3", "cmd4", "cmd5"}, {"cmd6"}});
		check("nested blocks", 2, "{\ncmd1\n{\ncmd2\n}\ncmd3\n}\ncmd4\ncmd5\n",
			{{"cmd1", "cmd2", "cmd3"}, {"cmd4", "cmd5"}});
		check("unfinished block", 3, "cmd1\n{\ncmd2\n", {{"cmd1"}});
		check("unfinished outer block", 3, "{\ncmd1\n{\ncmd2\n}\n", {});
		check("empty blocks", 3, "{\n{\n}\n}\n", {});
		check("empty input", 3, "", {});
		check("size one", 1, "cmd1\ncmd2", {{"cmd1"}, {"cmd2"}});
		check("empty command", 2, "\ncmd1\n", {{"", "cmd1"}});
		check("extra closing brace", 3, "}\ncmd1\n", {{"cmd1"}});

		bool rejected = false;
		try {
			BulkParser parser(0);
		} catch (const std::invalid_argument&) {
			rejected = true;
		}
		if (!rejected) {
			throw std::runtime_error("Zero size must be rejected");
		}
	} catch (const std::exception& error) {
		std::cerr << error.what() << '\n';
		return 1;
	}

	return 0;
}
