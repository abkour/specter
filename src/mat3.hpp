#pragma once
#include "vec3.hpp"

namespace specter {


template<typename T>
struct mat3 {

	mat3() = default;
	mat3(const T value);
	mat3(const vec3<T>& v);
	mat3(const vec3<T>& col0, const vec3<T>& col1, const vec3<T>& col2);
	mat3(const mat3<T>& other);
	mat3<T>& operator=(const mat3<T>& other);

	bool operator==(const mat3<T>& other);
	bool operator!=(const mat3<T>& other);

	mat3<T> operator+(const mat3<T>& other);
	mat3<T> operator-(const mat3<T>& other);
	mat3<T> operator*(const mat3<T>& other);

	mat3<T>& operator+=(const mat3<T>& other);
	mat3<T>& operator-=(const mat3<T>& other);
	mat3<T>& operator*=(const mat3<T>& other);

	mat3<T> operator*(const T s);
	mat3<T> operator/(const T s);
	mat3<T>& operator*=(const T s);
	mat3<T>& operator/=(const T s);

	T operator[](const std::size_t index) const;
	T& operator[](const std::size_t index);

	vec3<T> col(const std::size_t index) const;
	vec3<T> row(const std::size_t index) const;

	T data[9];
};

template<typename T>
mat3<T>::mat3(const T value) {
	data[0] = data[4] = data[8] = value;
}

template<typename T>
mat3<T>::mat3(const vec3<T>& v)  {
	data[0] = v[0]; data[1] = v[1]; data[2] = v[2];
	data[3] = v[0]; data[4] = v[1]; data[5] = v[2];
	data[6] = v[0]; data[7] = v[1]; data[8] = v[2];
}

template<typename T>
mat3<T>::mat3(const vec3<T>& col0, const vec3<T>& col1, const vec3<T>& col2) {
	data[0] = col0[0]; data[3] = col0[1]; data[6] = col0[2];
	data[1] = col1[0]; data[4] = col1[1]; data[5] = col1[2];
	data[2] = col2[0]; data[7] = col2[1]; data[8] = col2[2];
}

template<typename T>
mat3<T>::mat3(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] = other[i];
	}
}

template<typename T>
mat3<T>& mat3<T>::operator=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] = other[i];
	}
	return *this;
}

template<typename T>
bool mat3<T>::operator==(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		if (data[i] != other[i]) return false;
	}
	return true;
}

template<typename T>
bool mat3<T>::operator!=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		if (data[i] == other[i]) return false;
	}
	return true;
}

template<typename T>
mat3<T> mat3<T>::operator+(const mat3<T>& other) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result[i] = data[i] + other[i];
	}
	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator+=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] += other[i];
	}
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator-(const mat3<T>& other) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result[i] = data[i] - other[i];
	}
	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator-=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] -= other[i];
	}
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator*(const mat3<T>& other) {
	mat3<T> result;

	result[0] = data[0] * other[0] + data[1] * other[3] + data[2] * other[6];
	result[1] = data[0] * other[1] + data[1] * other[4] + data[2] * other[7];
	result[2] = data[0] * other[2] + data[1] * other[5] + data[2] * other[8];
	result[3] = data[3] * other[0] + data[4] * other[3] + data[5] * other[6];
	result[4] = data[3] * other[1] + data[4] * other[4] + data[5] * other[7];
	result[5] = data[3] * other[2] + data[4] * other[5] + data[5] * other[8];
	result[6] = data[6] * other[0] + data[7] * other[3] + data[8] * other[6];
	result[7] = data[6] * other[1] + data[7] * other[4] + data[8] * other[7];
	result[8] = data[6] * other[2] + data[7] * other[5] + data[8] * other[8];

	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator*=(const mat3<T>& other) {
	mat3<T> result;

	result[0] = data[0] * other[0] + data[1] * other[3] + data[2] * other[6];
	result[1] = data[0] * other[1] + data[1] * other[4] + data[2] * other[7];
	result[2] = data[0] * other[2] + data[1] * other[5] + data[2] * other[8];
	result[3] = data[3] * other[0] + data[4] * other[3] + data[5] * other[6];
	result[4] = data[3] * other[1] + data[4] * other[4] + data[5] * other[7];
	result[5] = data[3] * other[2] + data[4] * other[5] + data[5] * other[8];
	result[6] = data[6] * other[0] + data[7] * other[3] + data[8] * other[6];
	result[7] = data[6] * other[1] + data[7] * other[4] + data[8] * other[7];
	result[8] = data[6] * other[2] + data[7] * other[5] + data[8] * other[8];

	*this = result;
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator*(const T s) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result[i] = data[i] * s;
	}
	return result;
}

template<typename T>
mat3<T> mat3<T>::operator/(const T s) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result[i] = data[i] / s;
	}
	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator*=(const T s) {
	for (int i = 0; i < 9; ++i) {
		data[i] *= s;
	}
	return *this;
}

template<typename T>
mat3<T>& mat3<T>::operator/=(const T s) {
	for (int i = 0; i < 9; ++i) {
		data[i] /= s;
	}
	return *this;
}

template<typename T>
T mat3<T>::operator[](const std::size_t index) const {
	return data[index];
}

template<typename T>
T& mat3<T>::operator[](const std::size_t index) {
	return data[index];
}

template<typename T>
vec3<T> mat3<T>::col(const std::size_t index) const {
	return vec3<T>(data[index], data[index + 3], data[index + 6]);
}

template<typename T>
vec3<T> mat3<T>::row(const std::size_t index) const {
	return vec3<T>(data[index * 3], data[index * 3 + 1], data[index * 3 + 2]);
}

template<typename T>
T trace(const mat3<T>& m) {
	return m[0] + m[4] + m[8];
}

template<typename T>
T determinant(const mat3<T>& m) {
	return	m[0] * m[4] * m[8] - m[0] * m[5] * m[7]
		+	m[1] * m[5] * m[6] - m[1] * m[3] * m[8]
		+	m[2] * m[3] * m[7] - m[2] * m[4] * m[6];
}

template<typename T>
mat3<T> transpose(const mat3<T>& other) {
	mat3<T> tmat3(other);
	std::swap(tmat3[1], tmat3[3]);
	std::swap(tmat3[2], tmat3[6]);
	std::swap(tmat3[5], tmat3[7]);
	return tmat3;
}

template<typename T>
mat3<T> inverse(const mat3<T>& m) {
	T det = determinant(m);

	mat3<T> inverseMat;
	inverseMat[0] = + m[4] * m[8] - m[5] * m[7];
	inverseMat[1] = - m[1] * m[8] + m[2] * m[7];
	inverseMat[2] = + m[1] * m[5] - m[2] * m[4];
	inverseMat[3] = - m[3] * m[8] + m[5] * m[6];
	inverseMat[4] = + m[0] * m[8] - m[2] * m[6];
	inverseMat[5] = - m[0] * m[5] + m[2] * m[3];
	inverseMat[6] = + m[3] * m[7] - m[4] * m[6];
	inverseMat[7] = - m[0] * m[7] + m[1] * m[6];
	inverseMat[8] = + m[0] * m[4] - m[1] * m[3];

	inverseMat /= det;
	return inverseMat;
}

template<typename T>
bool isIdentity(const mat3<T>& m) {
	bool isDiagonalOne = m[0] == m[4] == m[8] == static_cast<T>(1);
	bool isOutsideNull = m[1] == m[2] == m[3] == m[5] == m[6] == m[7] == static_cast<T>(0);
	return isDiagonalOne && isOutsideNull;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, mat3<T>& mat) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			os << mat[i * 3 + j] << "\t";
		}
		os << '\n';
	}
	return os;
}

using mat3i = mat3<int>;
using mat3u = mat3<unsigned>;
using mat3f = mat3<float>;
using mat3d = mat3<double>;

}