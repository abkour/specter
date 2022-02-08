#pragma once
#include <algorithm>
#include <iostream>

namespace specter {

template<typename T>
struct vec3 {

	vec3() = default;
	vec3(T value);
	vec3(T x, T y, T z);
	vec3(const vec3<T>&other);
	vec3& operator=(const vec3<T>&other);

	bool operator==(const vec3<T>&other) const;
	bool operator!=(const vec3<T>&other) const;
	bool operator<=(const vec3<T>&other) const;
	bool operator>=(const vec3<T>&other) const;
	bool operator<(const vec3<T>&other) const;
	bool operator>(const vec3<T>&other) const;

	template<typename U>
	vec3<T>& operator+=(const U s);
	template<typename U>
	vec3<T>& operator-=(const U s);
	template<typename U>
	vec3<T>& operator*=(const U s);
	template<typename U>
	vec3<T>& operator/=(const U s);

	T operator[](int index) const;

	union {
		struct {
			T x, y, z;
		};
		T v[3];
	};

};

template<typename T>
vec3<T>::vec3(T value) 
	: x(value)
	, y(value)
	, z(value)
{}

template<typename T>
vec3<T>::vec3(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{}

template<typename T>
vec3<T>::vec3(const vec3<T>& other) 
	: x(other.x)
	, y(other.y)
	, z(other.z)
{}

template<typename T>
vec3<T>& vec3<T>::operator=(const vec3<T>& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}


template<typename T>
bool vec3<T>::operator==(const vec3<T>& other) const {
	return x == other.x && y == other.y && z == other.z;
}

template<typename T>
bool vec3<T>::operator!=(const vec3<T>& other) const {
	return x != other.x || y != other.y || z != other.z;
}

template<typename T>
bool vec3<T>::operator<=(const vec3<T>& other) const {
	return x <= other.x && y <= other.y && z <= other.z;
}

template<typename T>
bool vec3<T>::operator>=(const vec3<T>& other) const {
	return x >= other.x && y >= other.y && z >= other.z;
}

template<typename T>
bool vec3<T>::operator<(const vec3<T>& other) const {
	return x < other.x && y < other.y && z < other.z;
}

template<typename T>
bool vec3<T>::operator>(const vec3<T>& other) const {
	return x > other.x && y > other.y && z > other.z;
}


template<typename T, typename U>
vec3<T> operator+(const vec3<T>& v, const U s) {
	return vec3<T>(v.x + static_cast<T>(s), v.y + static_cast<T>(s), v.z + static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator+(const U s, const vec3<T>& v) {
	return vec3<T>(v.x + static_cast<T>(s), v.y + static_cast<T>(s), v.z + static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator-(const vec3<T>& v, const U s) {
	return vec3<T>(v.x - static_cast<T>(s), v.y - static_cast<T>(s), v.z - static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator-(const U s, const vec3<T>& v) {
	return vec3<T>(v.x - static_cast<T>(s), v.y - static_cast<T>(s), v.z - static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator*(const vec3<T>& v, const U s) {
	return vec3<T>(v.x * static_cast<T>(s), v.y * static_cast<T>(s), v.z * static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator*(const U s, const vec3<T>& v) {
	return vec3<T>(v.x * static_cast<T>(s), v.y * static_cast<T>(s), v.z * static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator/(const vec3<T>& v, const U s) {
	return vec3<T>(v.x / static_cast<T>(s), v.y / static_cast<T>(s), v.z / static_cast<T>(s));
}

template<typename T, typename U>
vec3<T> operator/(const U s, const vec3<T>& v) {
	return vec3<T>(v.x / static_cast<T>(s), v.y / static_cast<T>(s), v.z / static_cast<T>(s));
}

template<typename T>
template<typename U>
vec3<T>& vec3<T>::operator+=(const U s) {
	x += static_cast<T>(s);
	y += static_cast<T>(s);
	z += static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec3<T>& vec3<T>::operator-=(const U s) {
	x -= static_cast<T>(s);
	y -= static_cast<T>(s);
	z -= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec3<T>& vec3<T>::operator*=(const U s) {
	x *= static_cast<T>(s);
	y *= static_cast<T>(s);
	z *= static_cast<T>(s);
	return *this;
}

template<typename T>
template<typename U>
vec3<T>& vec3<T>::operator/=(const U s) {
	x /= static_cast<T>(s);
	y /= static_cast<T>(s);
	z /= static_cast<T>(s);
	return *this;
}

template<typename T>
T vec3<T>::operator[](int index) const {
	return v[index];
}


template<typename T>
vec3<T> minComponent(const vec3<T>& v0, const vec3<T>& v1) {
	vec3<T> result;
	result.x = std::min(v0.x, v1.x);
	result.y = std::min(v0.y, v1.y);
	result.z = std::min(v0.z, v1.z);
	return result;
}

template<typename T>
vec3<T> maxComponent(const vec3<T>& v0, const vec3<T>& v1) {
	vec3<T> result;
	result.x = std::max(v0.x, v1.x);
	result.y = std::max(v0.y, v1.y);
	result.z = std::max(v0.z, v1.z);
	return result;
}

template<typename T>
T minComponent(const vec3<T>& v) {
	return std::min(std::min(v.x, v.y), v.z);
}

template<typename T>
T maxComponent(const vec3<T>& v) {
	return std::max(std::max(v.x, v.y), v.z);
}

template<typename T>
T product(const vec3<T>& v) {
	return v.x * v.y * v.z;
}

template<typename T>
T sum(const vec3<T>& v) {
	return v.x + v.y + v.z;
}

template<typename T>
T length(const vec3<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

template<typename T>
vec3<T> normalize(const vec3<T>& v) {
	return v / length(v);
}

template<typename T>
T dot(const vec3<T>& v0, const vec3<T>& v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

template<typename T>
vec3<T> cross(const vec3<T>& v0, const vec3<T>& v1) {
	vec3<T> result;
	result.x = v0.y * v1.z - v0.z * v1.y;
	result.y = v0.z * v1.x - v0.x * v1.z;
	result.z = v0.x * v1.y - v0.y * v1.x;
	return result;
}

template<typename T>
vec3<T> inverse(const vec3<T>& v) {
	return vec3<T>(-v.x, -v.y, -v.z);
}

// Reflects vector v around normal n.
// Caller needs to make sure that n is normalized.
template<typename T>
vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
	return v - 2 * (dot(v, n)) * n;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& v) {
	return os << v.x << ", " << v.y << ", " << v.z;
}

using vec3u = vec3<unsigned>;
using vec3i = vec3<int>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;

}