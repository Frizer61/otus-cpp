#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using IpAddress = std::array<std::uint8_t, 4>;
using IpPool = std::vector<IpAddress>;

IpAddress parseIpAddress(const std::string& line) {
	IpAddress address{};
	std::size_t partBegin = 0;

	for (auto& part : address) {
		const auto partEnd = line.find_first_of(".\t", partBegin);
		part = static_cast<std::uint8_t>(std::stoi(line.substr(partBegin, partEnd - partBegin)));
		partBegin = partEnd + 1;
	}

	return address;
}

void printIpAddress(const IpAddress& address) {
	std::cout
		<< static_cast<unsigned int>(address[0]) << '.'
		<< static_cast<unsigned int>(address[1]) << '.'
		<< static_cast<unsigned int>(address[2]) << '.'
		<< static_cast<unsigned int>(address[3]) << '\n';
}

template<typename... Octets>
void printFiltered(const IpPool& ipPool, Octets... octets) {
	const std::array<int, sizeof...(Octets)> filter{octets...};
	
	int matchGroup = 0;
	for (const auto& addres : ipPool) {
		int match = 1;
		for (size_t i = 0; i < filter.size(); ++i) {
			if (addres[i] != filter[i]) {
				match = 0;
				break;
			}
		}

		if (match) {
			matchGroup = 1;
			printIpAddress(addres);
		}
		else if (matchGroup) break;
	}
}

int main() {
	try {
		IpPool ipPool;

		for (std::string line; std::getline(std::cin, line);) {
			ipPool.push_back(parseIpAddress(line));
		}

		//std::sort(ipPool.begin(), ipPool.end(), std::greater<>{});

		printFiltered(ipPool, 1);
		printFiltered(ipPool, 46, 70);
		// printFiltered(ipPool, [](const IpAddress&) {return true;});
		// printFiltered(ipPool, [](const IpAddress& address) {return address[0] == 1;});
		// printFiltered(ipPool, [](const IpAddress& address) {return address[0] == 46 && address[1] == 70;});
		// printFiltered(ipPool, [](const IpAddress& address) {return std::find(address.begin(), address.end(), 46) != address.end();});
	} 
	catch (const std::exception& error) {
		std::cerr << error.what() << std::endl;
		return 1;
	}

	return 0;
}
