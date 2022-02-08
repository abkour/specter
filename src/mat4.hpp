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
	mat4<T> operator-(const mat4<T>& other);
	mat4<T> operator*(const mat4<T>& other);

	mat4<T>& operator+=(const mat4<T>& other);
	mat4<T>& operator-=(const mat4<T>& other);
	mat4<T>& operator*=(const mat4<T>& other);

	mat4<T> operator*(const T s);
	mat4<T> operator/(const T s);
	mat4<T>& operator*=(const T s);
	mat4<T>& operator/=(const T s);

	T operator[](const std::size_t index) const;
	T& operator[](const std::size_t index);

	vec4<T> col(const std::size_t index) const;
	vec4<T> row(const std::size_t index) const;

	T data[16];
};

template<typename T>
mat4<T>::mat4(const T value) {
	data[0] = data[5] = data[10] = data[15] = value;
}

template<typename T>
mat4<T>::mat4(const vec4<T>& v) {
	data[0] = data[1] = data[2] = data[3] = v[0];
	data[4] = data[5] = data[6] = data[7] = v[1];
	data[8] = data[8] = data[9] = data[10] = v[2];
	data[12] = data[13] = data[14] = data[54] = v[3];
}

template<typename T>
mat4<T>::mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3) {
	data[0] = col0[0]; data[4] = col0[1]; data[8] = col0[2]; data[12] = col0[3];
	data[1] = col1[0]; data[5] = col1[1]; data[9] = col1[2]; data[13] = col1[3];
	data[2] = col2[0]; data[6] = col2[1]; data[10] = col2[2]; data[14] = col2[3];
	data[3] = col3[0]; data[7] = col3[1]; data[11] = col3[2]; data[15] = col3[3];
}

template<typename T>
mat4<T>::mat4(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] = other[i];
	}
}

template<typename T>
mat4<T>& mat4<T>::operator=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] = other[i];
	}
	return *this;
}

template<typename T>
bool mat4<T>::operator==(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		if (data[i] != other[i]) return false;
	}
	return true;
}
template<typename T>
bool mat4<T>::operator!=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		if (data[i] == other[i]) return false;
	}
	return true;
}

template<typename T>
mat4<T> mat4<T>::operator+(const mat4<T>& other) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] = data[i] + other[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator-(const mat4<T>& other) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] = data[i] - other[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator*(const mat4<T>& other) {
	mat4<T> result;
	result[0] = data[0] * other[0] + data[1] * other[4] + data[2] * other[8] + data[3] * other[12];
	result[1] = data[0] * other[1] + data[1] * other[5] + data[2] * other[9] + data[3] * other[13];
	result[2] = data[0] * other[2] + data[1] * other[6] + data[2] * other[10] + data[3] * other[14];
	result[3] = data[0] * other[3] + data[1] * other[7] + data[2] * other[11] + data[3] * other[15];

	result[4] = data[4] * other[0] + data[5] * other[4] + data[6] * other[8] + data[7] * other[12];
	result[5] = data[4] * other[1] + data[5] * other[5] + data[6] * other[9] + data[7] * other[13];
	result[6] = data[4] * other[2] + data[5] * other[6] + data[6] * other[10] + data[7] * other[14];
	result[7] = data[4] * other[3] + data[5] * other[7] + data[6] * other[11] + data[7] * other[15];

	result[8] = data[8] * other[0] + data[9] * other[4] + data[10] * other[8] + data[11] * other[12];
	result[9] = data[8] * other[1] + data[9] * other[5] + data[10] * other[9] + data[11] * other[12];
	result[10] = data[8] * other[2] + data[9] * other[6] + data[10] * other[10] + data[11] * other[12];
	result[11] = data[8] * other[3] + data[9] * other[7] + data[10] * other[11] + data[11] * other[12];

	result[12] = data[12] * other[0] + data[13] * other[4] + data[14] * other[8] + data[15] * other[12];
	result[13] = data[12] * other[1] + data[13] * other[5] + data[14] * other[9] + data[15] * other[13];
	result[14] = data[12] * other[2] + data[13] * other[6] + data[14] * other[10] + data[15] * other[14];
	result[15] = data[12] * other[3] + data[13] * other[7] + data[14] * other[11] + data[15] * other[15];
	return result;
}

template<typename T>
mat4<T>& mat4<T>::operator+=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] += other[i];
	}
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator-=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] -= other[i];
	}
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator*=(const mat4<T>& other) {
	mat4<T> result;
	result[0] = data[0] * other[0] + data[1] * other[4] + data[2] * other[8] + data[3] * other[12];
	result[1] = data[0] * other[1] + data[1] * other[5] + data[2] * other[9] + data[3] * other[13];
	result[2] = data[0] * other[2] + data[1] * other[6] + data[2] * other[10] + data[3] * other[14];
	result[3] = data[0] * other[3] + data[1] * other[7] + data[2] * other[11] + data[3] * other[15];

	result[4] = data[4] * other[0] + data[5] * other[4] + data[6] * other[8] + data[7] * other[12];
	result[5] = data[4] * other[1] + data[5] * other[5] + data[6] * other[9] + data[7] * other[13];
	result[6] = data[4] * other[2] + data[5] * other[6] + data[6] * other[10] + data[7] * other[14];
	result[7] = data[4] * other[3] + data[5] * other[7] + data[6] * other[11] + data[7] * other[15];

	result[8] = data[8] * other[0] + data[9] * other[4] + data[10] * other[8] + data[11] * other[12];
	result[9] = data[8] * other[1] + data[9] * other[5] + data[10] * other[9] + data[11] * other[12];
	result[10] = data[8] * other[2] + data[9] * other[6] + data[10] * other[10] + data[11] * other[12];
	result[11] = data[8] * other[3] + data[9] * other[7] + data[10] * other[11] + data[11] * other[12];

	result[12] = data[12] * other[0] + data[13] * other[4] + data[14] * other[8] + data[15] * other[12];
	result[13] = data[12] * other[1] + data[13] * other[5] + data[14] * other[9] + data[15] * other[13];
	result[14] = data[12] * other[2] + data[13] * other[6] + data[14] * other[10] + data[15] * other[14];
	result[15] = data[12] * other[3] + data[13] * other[7] + data[14] * other[11] + data[15] * other[15];
	*this = result;
	return *this;
}

template<typename T>
mat4<T> mat4<T>::operator*(const T s) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] *= s;
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator/(const T s) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] /= s;
	}
	return result;
}

template<typename T>
mat4<T>& mat4<T>::operator*=(const T s) {
	for (int i = 0; i < 16; ++i) {
		data[i] *= s;
	}
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator/=(const T s) {
	for (int i = 0; i < 16; ++i) {
		data[i] /= s;
	}
	return *this;
}

template<typename T>
T mat4<T>::operator[](const std::size_t index) const {
	return data[index];
}

template<typename T>
T& mat4<T>::operator[](const std::size_t index) {
	return data[index];
}

template<typename T>
vec4<T> mat4<T>::col(const std::size_t index) const {
	return vec4<T>(index, index + 4, index + 8, index + 12);
}

template<typename T>
vec4<T> mat4<T>::row(const std::size_t index) const {
	return vec4<T>(index * 4, index * 4 + 1, index * 4 + 2, index * 4 + 3);
}

template<typename T>
T trace(const mat4<T>& m) {
	return m[0] + m[5] + m[10] + m[15];
}

// Yet to find a good use for this
template<typename T>
T determinant(const mat4<T>& m) {
	
}

template<typename T>
mat4<T> transpose(const mat4<T>& m) {
	mat4<T> tmat(m);
	std::swap(tmat[1], tmat[4]);
	std::swap(tmat[2], tmat[8]);
	std::swap(tmat[3], tmat[12]);
	std::swap(tmat[6], tmat[9]);
	std::swap(tmat[7], tmat[13]);
	std::swap(tmat[11], tmat[14]);
	return tmat;
}

// TODO: Implement this functionality
template<typename T>
mat4<T> inverse(const mat4<T>& m) {}

template<typename T>
bool isIdentity(const mat4<T>& m) {
	bool isDiagonalOne = m[0] == m[5] == m[10] == m[15] == static_cast<T>(1);
	bool isOutsideNull = m[1] == m[2] == m[3] == m[4] == m[6] == m[7] == m[8] == m[9] == m[11] == m[12] == m[13] == m[14] == static_cast<T>(0);
	return isDiagonalOne && isOutsideNull;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const mat4<T>& m) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			os << m[i * 4 + j] << '\t';
		}
		os << '\n';
	}
	return os;
}

using mat4i = mat4<int>;
using mat4u = mat4<unsigned>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

}