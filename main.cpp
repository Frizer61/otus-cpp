#include "myAlloc.h"
#include "myCont.h"

#include <map>
#include <iostream>

int factor(int a) {
	int res = 1;
	for (int mult = 2; mult <= a; ++mult) {
		res *= mult;
	}
	return res;
}

int main(int, char **) { 
	//----------- Common map -------------
	std::map<int, int> commonMap;
	for (int i = 0; i < 10; ++i) {
		commonMap.emplace(i, factor(i));
	}

	//----------- Custom map with my alloc -------------
	using mapVal = std::pair<const int, int>;
	using mapAlloc = MyAllocator<mapVal, 10>;

	std::map<int, int, std::less<int>, mapAlloc> customMap;
	for (int i = 0; i < 10; ++i) {
		customMap.emplace(i, factor(i));
	}

	for (const auto& [key, value] : customMap) {
		std::cout << key << ' ' << value << std::endl;
	}

	//----------- Custom container -------------
	MyContainer<int> cont1;
	for (int i = 0; i < 10; ++i) {
		cont1.pushBack(i);
	}
	cont1.forEach([](int value){std::cout << value << std::endl;});

	MyContainer<int, MyAllocator<int, 10>> cont2;
	for (int i = 0; i < 10; ++i) {
		cont2.pushBack(i);
	}
	cont2.forEach([](int value){std::cout << value << std::endl;});
	
	return 0;
}
