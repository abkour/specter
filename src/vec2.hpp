#pragma once
#include <algorithm>
#include <iostream>

namespace specter {

template<typename T>
struct vec2 {

	vec2() = default;
	vec2(T x, T y);
	vec2(const vec2<T>& other);
	vec2& operator=(const vec2<T>& other);

	bool operator==(const vec2<T>& other) const;
	bool operator!=(const vec2<T>& other) const;
	bool operator<=(const vec2<T>& other) const;
	bool operator>=(const vec2<T>& other) const;
	bool operator<(const vec2<T>& other) const;
	bool operator>(const vec2<T>& other) const;

	template<typename U>
	vec2<T>& operator+=(const U s);
	template<typename U>
	vec2<T>& operator-=(const U s);
	template<typename U>
	vec2<T>& operator*=(const U s);
	template<typename U>
	vec2<T>& operator/=(const U s);

	T operator[](int index) const;

	union {
		struct {
			T x, y;
		};
		T v[2];
	};
};

template<typename T>
vec2<T>::vec2(T x, T y) 
	: x(x)
	, y(y)
{}

template<typename T>
vec2<T>::vec2(const vec2<T>&other)
	: x(other.x)
	, y(other.y)
{}

template<typename T>
vec2<T>& vec2<T>::operator=(const vec2<T>&other) {
	x = other.x;
	y = other.y;
	return *this;
}

template<typename T>
bool vec2<T>::operator==(const vec2<T>&other) const {
	return x == other.x && y == other.y;
}

template<typename T>
bool vec2<T>::operator!=(const vec2<T>&other) const {
	return x != other.x || y != other.y;
}

template<typename T>
bool vec2<T>::operator<=(const vec2<T>&other) const {
	return x <= other.x && y <= other.y;
}

template<typename T>
bool vec2<T>::operator>=(const vec2<T>&other) const {
	return x >= other.x && y >= other.y;
}

template<typename T>
bool vec2<T>::operator<(const vec2<T>&other) const {
	return x < other.x && y < other.y;
}

template<typename T>
bool vec2<T>::operator>(const vec2<T>&other) const {
	return x > other.x && y > other.y;
}

template<typename T, typename U>
vec2<T> operator+(const vec2<T>& v, const U s) {
	return vec2<T>(v.x + static_cast<T>(s), v.y + static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator+(const U s, const vec2<T>& v) {
	return vec2<T>(v.x + static_cast<T>(s), v.y + static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator-(const vec2<T>& v, const U s) {
	return vec2<T>(v.x - static_cast<T>(s), v.y - static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator-(const U s, const vec2<T>& v) {
	return vec2<T>(v.x - static_cast<T>(s), v.y - static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator*(const vec2<T>& v, const U s) {
	return vec2<T>(v.x * static_cast<T>(s), v.y * static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator*(const U s, const vec2<T>& v) {
	return vec2<T>(v.x * static_cast<T>(s), v.y * static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator/(const vec2<T>& v, const U s) {
	return vec2<T>(v.x / static_cast<T>(s), v.y / static_cast<T>(s));
}

template<typename T, typename U>
vec2<T> operator/(const U s, const vec2<T>& v) {
	return vec2<T>(v.x / static_cast<T>(s), v.y / static_cast<T>(s));
}

template<typename T>
template<typename U>
vec2<T>& vec2<T>::operator+=(const U s) {
	x += static_cast<T>(s);
	y += static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec2<T>& vec2<T>::operator-=(const U s) {
	x -= static_cast<T>(s);
	y -= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec2<T>& vec2<T>::operator*=(const U s) {
	x *= static_cast<T>(s);
	y *= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec2<T>& vec2<T>::operator/=(const U s) {
	x /= static_cast<T>(s);
	y /= static_cast<T>(s);
	return *this;
}

template<typename T>
T vec2<T>::operator[](int index) const {
	return v[index];
}


template<typename T>
vec2<T> minComponent(const vec2<T>& v0, const vec2<T>& v1) {
	vec2<T> result;
	result.x = std::min(v0.x, v1.x);
	result.y = std::min(v0.y, v1.y);
	return result;
}

template<typename T>
vec2<T> maxComponent(const vec2<T>& v0, const vec2<T>& v1) {
	vec2<T> result;
	result.x = std::max(v0.x, v1.x);
	result.y = std::max(v0.y, v1.y);
	return result;
}

template<typename T>
T minComponent(const vec2<T>& v) {
	return std::min(v.x, v.y);
}

template<typename T>
T maxComponent(const vec2<T>& v) {
	return std::max(v.x, v.y);
}

template<typename T>
T product(const vec2<T>& v) {
	return v.x * v.y;
}

template<typename T>
T sum(const vec2<T>& v) {
	return v.x + v.y;
}

template<typename T>
T length(const vec2<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

template<typename T>
vec2<T> normalize(const vec2<T>& v) {
	return v / length(v);
}

template<typename T>
vec2<T> inverse(const vec2<T>& v) {
	return vec2<T>(-v.x, -v.y);
}

template<typename T>
vec2<T>& reflect(const vec2<T>& v, const vec2<T>& n) {
	return v - 2 * (dot(v, n)) * n;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& v) {
	return os << v.x << ", " << v.y;
}

using vec2i = vec2<int>;
using vec2u = vec2<unsigned>;
using vec2f = vec2<float>;
using vec2d = vec2<double>;

}
