#include "bulk.h"

#include <stdexcept>

BulkParser::BulkParser(std::size_t size)
	: size_(size) {
	if (size_ == 0) {
		throw std::invalid_argument("Bulk size must be positive");
	}
}

void BulkParser::addLogger(ILogger& logger) {
	loggers_.push_back(&logger);
}

void BulkParser::process(const std::string& command) {
	if (command == "{") {
		if (depth_ == 0) {
			flush();
		}
		++depth_;
		return;
	}

	if (command == "}") {
		if (depth_ > 0) {
			--depth_;
			if (depth_ == 0) {
				flush();
			}
		}
		return;
	}

	if (bulk_.commands.empty()) {
		for (auto logger : loggers_) {
			logger->onBulkStart();
		}
	}
	bulk_.commands.push_back(command);
	if (depth_ == 0 && bulk_.commands.size() == size_) {
		flush();
	}
}

void BulkParser::finish() {
	if (depth_ == 0) {
		flush();
	} else {
		bulk_.commands.clear();
		depth_ = 0;
	}
}

void BulkParser::flush() {
	if (bulk_.commands.empty()) {
		return;
	}
	for (auto logger : loggers_) {
		logger->write(bulk_);
	}
	bulk_.commands.clear();
}
