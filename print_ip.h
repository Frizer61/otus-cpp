#pragma once

#include <iostream>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

namespace detail {

/**
 * @brief Определяет контейнеры, которые можно вывести как IP-адрес.
 *
 * По умолчанию тип не считается поддерживаемым контейнером.
 */
template<typename T>
struct IsIpContainer : std::false_type {
};

/**
 * @brief Разрешает использование std::vector.
 */
template<typename T, typename Allocator>
struct IsIpContainer<std::vector<T, Allocator>> : std::true_type {
};

/**
 * @brief Разрешает использование std::list.
 */
template<typename T, typename Allocator>
struct IsIpContainer<std::list<T, Allocator>> : std::true_type {
};

/**
 * @brief Возвращает беззнаковый тип для целочисленного значения.
 */
template<typename T>
struct UnsignedType {
	using Type = typename std::make_unsigned<T>::type;
};

/**
 * @brief Позволяет обрабатывать bool как однобайтовое целое значение.
 */
template<>
struct UnsignedType<bool> {
	using Type = unsigned char;
};

} // namespace detail

/**
 * @brief Печатает целое число побайтово, начиная со старшего байта.
 * @tparam T Целочисленный тип адреса.
 * @param address Значение условного IP-адреса.
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_ip(const T& address) {
	using UnsignedType = typename detail::UnsignedType<T>::Type;
	const auto value = static_cast<UnsignedType>(address);

	for (std::size_t byte = sizeof(T); byte > 0; --byte) {
		if (byte != sizeof(T)) {
			std::cout << '.';
		}

		const auto shift = (byte - 1) * 8;
		const auto octet = static_cast<unsigned int>((value >> shift) & 0xFF);
		std::cout << octet;
	}

	std::cout << '\n';
}

/**
 * @brief Печатает строку без преобразований.
 * @tparam T Тип строки.
 * @param address Строковое представление условного IP-адреса.
 */
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(const T& address) {
	std::cout << address << '\n';
}

/**
 * @brief Печатает элементы std::vector или std::list через точку.
 * @tparam T Тип поддерживаемого контейнера.
 * @param address Контейнер с элементами условного IP-адреса.
 */
template<typename T>
typename std::enable_if<detail::IsIpContainer<T>::value, void>::type
print_ip(const T& address) {
	bool first = true;
	for (const auto& part : address) {
		if (!first) {
			std::cout << '.';
		}

		std::cout << part;
		first = false;
	}

	std::cout << '\n';
}
