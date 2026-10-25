#include "bulk.h"

#include <stdexcept>
#include <utility>

BulkParser::BulkParser(std::size_t size, std::function<void(const Bulk&)> handler)
	: size_(size), handler_(std::move(handler)) {
	if (size_ == 0) {
		throw std::invalid_argument("Bulk size must be positive");
	}
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
		bulk_.time = std::time(nullptr);
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
	handler_(bulk_);
	bulk_.commands.clear();
}
