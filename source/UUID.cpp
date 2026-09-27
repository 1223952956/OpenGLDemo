#include "UUID.h"
#include <random>

std::string UUID::ToString() const
{
	return std::to_string(Value);
}

uint64_t UUID::Generate()
{
	static std::random_device device;
	static std::mt19937_64 generator(device());
	static std::uniform_int_distribution<uint64_t> distribution;

	uint64_t result = 0;

	while (result == 0)
	{
		result = distribution(generator);
	}


	return result;
}
