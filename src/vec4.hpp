#pragma once 
#include <algorithm>
#include <iostream>

namespace specter {

template<typename T>
struct vec4 {

	vec4() = default;
	vec4(T x, T y, T z, T w);
	vec4(const vec4<T>&other);
	vec4(vec4<T> && other);
	vec4& operator=(const vec4<T>&other);

	bool operator==(const vec4<T>&other) const;
	bool operator!=(const vec4<T>&other) const;
	bool operator<=(const vec4<T>&other) const;
	bool operator>=(const vec4<T>&other) const;
	bool operator<(const vec4<T>&other) const;
	bool operator>(const vec4<T>&other) const;

	vec4<T> operator+(const vec4<T>& other);
	vec4<T> operator-(const vec4<T>& other);
	vec4<T> operator*(const vec4<T>& other);
	vec4<T> operator/(const vec4<T>& other);

	vec4<T>& operator+=(const vec4<T>& other);
	vec4<T>& operator-=(const vec4<T>& other);
	vec4<T>& operator*=(const vec4<T>& other);
	vec4<T>& operator/=(const vec4<T>& other);

	template<typename U>
	vec4<T>& operator+=(const U s);
	template<typename U>
	vec4<T>& operator-=(const U s);
	template<typename U>
	vec4<T>& operator*=(const U s);
	template<typename U>
	vec4<T>& operator/=(const U s);

	T operator[](int index) const;

	union {
		struct {
			T x, y, z, w;
		};
		T v[4];
	};
};

template<typename T>
vec4<T>::vec4(T x, T y, T z, T w) 
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{}

template<typename T>
vec4<T>::vec4(const vec4<T>& other) 
	: x(other.x)
	, y(other.y)
	, z(other.z)
	, w(other.w)
{}

template<typename T>
vec4<T>::vec4(vec4<T>&& other)
	: x(std::move(other.x))
	, y(std::move(other.y))
	, z(std::move(other.z))
	, w(std::move(other.w))
{}

template<typename T>
vec4<T>& vec4<T>::operator=(const vec4<T>& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}


template<typename T>
bool vec4<T>::operator==(const vec4<T>& other) const {
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

template<typename T>
bool vec4<T>::operator!=(const vec4<T>& other) const {
	return x != other.x || y != other.y || z != other.z || w != other.w;
}

template<typename T>
bool vec4<T>::operator<=(const vec4<T>& other) const {
	return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
}

template<typename T>
bool vec4<T>::operator>=(const vec4<T>& other) const {
	return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
}

template<typename T>
bool vec4<T>::operator<(const vec4<T>& other) const {
	return x < other.x && y < other.y && z < other.z && w < other.w;
}

template<typename T>
bool vec4<T>::operator>(const vec4<T>& other) const {
	return x > other.x && y > other.y && z > other.z && w > other.w;
}

template<typename T>
vec4<T> vec4<T>::operator+(const vec4<T>& other) {
	return vec4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<typename T>
vec4<T> vec4<T>::operator-(const vec4<T>& other) {
	return vec4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<typename T>
vec4<T> vec4<T>::operator*(const vec4<T>& other) {
	return vec4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<typename T>
vec4<T> vec4<T>::operator/(const vec4<T>& other) {
	return vec4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
}

template<typename T>
vec4<T>& vec4<T>::operator+=(const vec4<T>& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

template<typename T>
vec4<T>& vec4<T>::operator-=(const vec4<T>& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

template<typename T>
vec4<T>& vec4<T>::operator*=(const vec4<T>& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

template<typename T>
vec4<T>& vec4<T>::operator/=(const vec4<T>& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}

template<typename T, typename U>
vec4<T> operator+(const vec4<T>& v, const U s) {
	return vec4<T>(v.x + s, v.y + s, v.z + s, v.w + s);
}

template<typename T, typename U>
vec4<T> operator+(const U s, const vec4<T>& v) {
	return vec4<T>(v.x + s, v.y + s, v.z + s, v.w + s);
}

template<typename T, typename U>
vec4<T> operator-(const vec4<T>& v, const U s) {
	return vec4<T>(v.x - s, v.y - s, v.z - s, v.w - s);
}

template<typename T, typename U>
vec4<T> operator-(const U s, const vec4<T>& v) {
	return vec4<T>(v.x - s, v.y - s, v.z - s, v.w - s);
}

template<typename T, typename U>
vec4<T> operator*(const vec4<T>& v, const U s) {
	return vec4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template<typename T, typename U>
vec4<T> operator*(const U s, const vec4<T>& v) {
	return vec4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template<typename T, typename U>
vec4<T> operator/(const vec4<T>& v, const U s) {
	return vec4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}

template<typename T, typename U>
vec4<T> operator/(const U s, const vec4<T>& v) {
	return vec4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}

template<typename T>
template<typename U>
vec4<T>& vec4<T>::operator+=(const U s) {
	x += s;
	y += s;
	z += s;
	w += s;
	return *this;
}

template<typename T>
template<typename U>
vec4<T>& vec4<T>::operator-=(const U s) {
	x -= s;
	y -= s;
	z -= s;
	w -= s;
	return *this;
}

template<typename T>
template<typename U>
vec4<T>& vec4<T>::operator*=(const U s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

template<typename T>
template<typename U>
vec4<T>& vec4<T>::operator/=(const U s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

template<typename T>
T vec4<T>::operator[](int index) const {
	return v[index];
}


template<typename T>
vec4<T> minComponent(const vec4<T>& v0, const vec4<T>& v1) {
	vec4<T> result;
	result.x = std::min(v0.x, v1.x);
	result.y = std::min(v0.y, v1.y);
	result.z = std::min(v0.z, v1.z);
	result.w = std::min(v0.w, v1.w);
	return result;
}

template<typename T>
vec4<T> maxComponent(const vec4<T>& v0, const vec4<T>& v1) {
	vec4<T> result;
	result.x = std::max(v0.x, v1.x);
	result.y = std::max(v0.y, v1.y);
	result.z = std::max(v0.z, v1.z);
	result.w = std::max(v0.w, v1.w);
	return result;
}

template<typename T>
T minComponent(const vec4<T>& v)  {
	return std::min(std::min(v.x, v.y), std::min(v.z, v.w));
}

template<typename T>
T maxComponent(const vec4<T>& v)  {
	return std::max(std::max(v.x, v.y), std::max(v.z, v.w));
}

template<typename T>
T product(const vec4<T>& v) {
	return v.x * v.y * v.z * v.w;
}

template<typename T>
T sum(const vec4<T>& v) {
	return v.x + v.y + v.z + v.w;
}

template<typename T>
T length(const vec4<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

template<typename T>
vec4<T> normalize(const vec4<T>& v) {
	return v / length(v);
}

template<typename T>
T dot(const vec4<T>& v0, const vec4<T>& v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

template<typename T>
vec4<T> inverse(const vec4<T>& v) {
	return vec4<T>(-v.x, -v.y, -v.z, -v.w);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec4<T>& vec) {
	return os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
}

// Name alias the common vector types
using vec4i = vec4<int>;
using vec4u = vec4<unsigned>;
using vec4f = vec4<float>;
using vec4d = vec4<double>;

}