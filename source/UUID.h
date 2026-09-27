#pragma once
#include <cstdint>
#include <string>

class UUID
{
public:
	UUID() : Value(Generate()) {}
	UUID(uint64_t value) : Value(value) {}

	uint64_t GetValue() const { return Value; }
	std::string ToString() const;

	explicit operator uint64_t() const { return Value; }
	bool operator==(const UUID& other) const { return Value == other.Value; }

private:
	uint64_t Value;

	static uint64_t Generate();
};

