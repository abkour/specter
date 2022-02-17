#pragma once
#include <algorithm>
#include <iostream>

#include <immintrin.h>

namespace specter {

// Represents a 3D vector of type T
template<typename T>
struct vec3 {

	vec3() = default;
	vec3(T value);
	vec3(T x, T y, T z);
	vec3(const vec3<T>& other);
	vec3(vec3<T>&& other);
	vec3& operator=(const vec3<T>& other);
	vec3& operator=(vec3<T>&& other);

	bool operator==(const vec3<T>& other) const;
	bool operator!=(const vec3<T>& other) const;
	bool operator<=(const vec3<T>& other) const;
	bool operator>=(const vec3<T>& other) const;
	bool operator<(const vec3<T>& other) const;
	bool operator>(const vec3<T>& other) const;

	vec3<T> operator+(const vec3<T>& other) const;
	vec3<T> operator-(const vec3<T>& other) const;
	vec3<T> operator*(const vec3<T>& other) const;
	vec3<T> operator/(const vec3<T>& other) const;

	vec3<T>& operator+=(const vec3<T>& other);
	vec3<T>& operator-=(const vec3<T>& other);
	vec3<T>& operator*=(const vec3<T>& other);
	vec3<T>& operator/=(const vec3<T>& other);

	vec3<T>& operator+=(const T s);
	vec3<T>& operator-=(const T s);
	vec3<T>& operator*=(const T s);
	vec3<T>& operator/=(const T s);

	T operator[](const std::size_t index) const;
	T& operator[](const std::size_t index);

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
vec3<T>::vec3(vec3<T>&& other) {
	std::swap(v, other.v);
}

template<typename T>
vec3<T>& vec3<T>::operator=(const vec3<T>& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator=(vec3<T>&& other) {
	std::swap(v, other.v);
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

template<typename T>
vec3<T> vec3<T>::operator+(const vec3<T>& other) const {
	return vec3<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
vec3<T> vec3<T>::operator-(const vec3<T>& other) const {
	return vec3<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
vec3<T> vec3<T>::operator*(const vec3<T>& other) const {
	return vec3<T>(x * other.x, y * other.y, z * other.z);
}

template<typename T>
vec3<T> vec3<T>::operator/(const vec3<T>& other) const {
	return vec3<T>(x / other.x, y / other.y, z / other.z);
}

template<typename T>
vec3<T>& vec3<T>::operator+=(const vec3<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator-=(const vec3<T>& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator*=(const vec3<T>& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator/=(const vec3<T>& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

template<typename T>
vec3<T> operator+(const vec3<T>& v, const T s) {
	return vec3<T>(v.x + s, v.y + s, v.z + s);
}

template<typename T>
vec3<T> operator+(const T s, const vec3<T>& v) {
	return vec3<T>(v.x + s, v.y + s, v.z + s);
}

template<typename T>
vec3<T> operator-(const vec3<T>& v, const T s) {
	return vec3<T>(v.x - s, v.y - s, v.z - s);
}

template<typename T>
vec3<T> operator-(const T s, const vec3<T>& v) {
	return vec3<T>(v.x - s, v.y - s, v.z - s);
}

template<typename T>
vec3<T> operator*(const vec3<T>& v, const T s) {
	return vec3<T>(v.x * s, v.y * s, v.z * s);
}

template<typename T>
vec3<T> operator*(const T s, const vec3<T>& v) {
	return vec3<T>(v.x * s, v.y * s, v.z * s);
}

template<typename T>
vec3<T> operator/(const vec3<T>& v, const T s) {
	return vec3<T>(v.x / s, v.y / s, v.z / s);
}

template<typename T>
vec3<T> operator/(const T s, const vec3<T>& v) {
	return vec3<T>(s / v.x, s / v.y, s / v.z);
}

template<typename T>
vec3<T>& vec3<T>::operator+=(const T s) {
	x += s;
	y += s;
	z += s;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator-=(const T s) {
	x -= s;
	y -= s;
	z -= s;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator*=(const T s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

template<typename T>
vec3<T>& vec3<T>::operator/=(const T s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

template<typename T>
T vec3<T>::operator[](const std::size_t index) const {
	return v[index];
}

template<typename T>
T& vec3<T>::operator[](const std::size_t index) {
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
vec3<T> abs(const vec3<T>& v) {
	return vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
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
T absDot(const vec3<T>& v0, const vec3<T>& v1) {
	return std::abs(dot(v0, v1));
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
vec3<T> invert(const vec3<T>& v) {
	return vec3<T>(-v.x, -v.y, -v.z);
}

// Reflects vector v around normal n.
// Caller needs to make sure that n is normalized.
template<typename T>
vec3<T> reflect(const vec3<T>& v, const vec3<T>& n) {
	return v - 2 * (dot(v, n)) * n;
}

template<typename T>
T angle(const vec3<T>& v0, const vec3<T>& v1) {
	return std::acos(std::clamp(dot(v0, v1), T(-1), T(1)));
}

template<typename T>
vec3<T> orientNormal(const vec3<T>& normal, const vec3<T>& vector) {
	return dot(normal, vector) < 0.f ? -normal : normal;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& v) {
	return os << v.x << ", " << v.y << ", " << v.z;
}

// Name alias the common vector types
using vec3u = vec3<unsigned>;
using vec3i = vec3<int>;
using vec3f = vec3<float>;
using vec3d = vec3<double>;

}