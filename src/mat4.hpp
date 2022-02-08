#pragma once
#include "vec4.hpp"

namespace specter {

template<typename T>
struct mat4 {

	mat4() = default;
	mat4(const T value);
	mat4(const vec4<T>& v);
	mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3);
	mat4(const mat4<T>& other);
	mat4<T>& operator=(const mat4<T>& other);

	bool operator==(const mat4<T>& other);
	bool operator!=(const mat4<T>& other);

	mat4<T> operator+(const mat4<T>& other);
	mat4<T>& operator+=(const mat4<T>& other);
	mat4<T> operator-(const mat4<T>& other);
	mat4<T>& operator-=(const mat4<T>& other);
	mat4<T> operator*(const mat4<T>& other);
	mat4<T>& operator*=(const mat4<T>& other);


	vec4<T> col(int index) const;
	vec4<T> row(int index) const;

	T data[16];
};

template<typename T>
T trace(const mat4<T>& m) {
	return m[0] + m[5] + m[10] + m[15];
}

template<typename T>
T determinant(const mat4<T>& m) {

}

template<typename T>
mat4<T> transpose(const mat4<T>& other);

template<typename T>
mat4<T> inverse(const mat4<T>& other);

template<typename T>
mat4<T> identity();

using mat4i = mat4<int>;
using mat4u = mat4<unsigned>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

}