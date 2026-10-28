#pragma once

struct Bulk;

class ILogger {
public:
	virtual ~ILogger() = default;
	virtual void onBulkStart() {}
	virtual void write(const Bulk& bulk) = 0;
};
