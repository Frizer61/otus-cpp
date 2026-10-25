#include "bulk.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void check(const std::string& name, std::size_t size, const std::string& input,
	const std::vector<std::vector<std::string>>& expected) {
	std::vector<std::vector<std::string>> result;
	BulkParser parser(size, [&](const Bulk& bulk) {
		result.push_back(bulk.commands);
	});

	std::istringstream stream(input);
	std::string command;
	while (std::getline(stream, command)) {
		parser.process(command);
	}
	parser.finish();
	parser.finish();
	if (result != expected) {
		throw std::runtime_error("Failed: " + name);
	}
}

int main() {
	try {
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
			BulkParser parser(0, [](const Bulk&) {});
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
