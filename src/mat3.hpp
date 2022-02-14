#pragma once
#include "vec3.hpp"

namespace specter {


template<typename T>
struct mat3 {

	mat3();
	mat3(const T value);
	mat3(const vec3<T>& v);
	mat3(const vec3<T>& col0, const vec3<T>& col1, const vec3<T>& col2);
	mat3(const mat3<T>& other);
	mat3(mat3<T>&& other);
	mat3<T>& operator=(const mat3<T>& other);
	mat3<T>& operator=(mat3<T>&& other);

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

	T* operator[](const std::size_t index);
	const T* operator[](const std::size_t index) const;

	vec3<T> col(const std::size_t index) const;
	vec3<T> row(const std::size_t index) const;

	T data[9];
};

template<typename T>
mat3<T>::mat3() {
	std::memset(data, 0, sizeof(T) * 9);
}

template<typename T>
mat3<T>::mat3(const T value) {
	std::memset(data, 0, sizeof(T) * 9);
	data[0] = data[4] = data[8] = value;
}

template<typename T>
mat3<T>::mat3(const vec3<T>& v)  {
	data[0] = v[0]; data[1] = v[0]; data[2] = v[0];
	data[3] = v[1]; data[4] = v[1]; data[5] = v[1];
	data[6] = v[2]; data[7] = v[2]; data[8] = v[2];
}

template<typename T>
mat3<T>::mat3(const vec3<T>& col0, const vec3<T>& col1, const vec3<T>& col2) {
	data[0] = col0[0]; data[1] = col1[0]; data[2] = col2[0]; 
	data[3] = col0[1]; data[4] = col1[1]; data[5] = col2[1];
	data[6] = col0[2]; data[7] = col1[2]; data[8] = col2[2];
}

template<typename T>
mat3<T>::mat3(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] = other.data[i];
	}
}

template<typename T>
mat3<T>::mat3(mat3<T>&& other) {
	std::swap(data, other.data);
}

template<typename T>
mat3<T>& mat3<T>::operator=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] = other.data[i];
	}
	return *this;
}

template<typename T>
mat3<T>& mat3<T>::operator=(mat3<T>&& other) {
	std::swap(data, other.data);
	return *this;
}


template<typename T>
bool mat3<T>::operator==(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		if (data[i] != other.data[i]) return false;
	}
	return true;
}

template<typename T>
bool mat3<T>::operator!=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		if (data[i] == other.data[i]) return false;
	}
	return true;
}

template<typename T>
mat3<T> mat3<T>::operator+(const mat3<T>& other) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result.data[i] = data[i] + other.data[i];
	}
	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator+=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] += other.data[i];
	}
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator-(const mat3<T>& other) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result.data[i] = data[i] - other.data[i];
	}
	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator-=(const mat3<T>& other) {
	for (int i = 0; i < 9; ++i) {
		data[i] -= other.data[i];
	}
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator*(const mat3<T>& other) {
	mat3<T> result;

	result[0][0] = data[0] * other[0][0] + data[1] * other[1][0] + data[2] * other[2][0];
	result[0][1] = data[0] * other[0][1] + data[1] * other[1][1] + data[2] * other[2][1];
	result[0][2] = data[0] * other[0][2] + data[1] * other[1][2] + data[2] * other[2][2];
	result[1][0] = data[3] * other[0][0] + data[4] * other[1][0] + data[5] * other[2][0];
	result[1][1] = data[3] * other[0][1] + data[4] * other[1][1] + data[5] * other[2][1];
	result[1][2] = data[3] * other[0][2] + data[4] * other[1][2] + data[5] * other[2][2];
	result[2][0] = data[6] * other[0][0] + data[7] * other[1][0] + data[8] * other[2][0];
	result[2][1] = data[6] * other[0][1] + data[7] * other[1][1] + data[8] * other[2][1];
	result[2][2] = data[6] * other[0][2] + data[7] * other[1][2] + data[8] * other[2][2];

	return result;
}

template<typename T>
mat3<T>& mat3<T>::operator*=(const mat3<T>& other) {
	mat3<T> result;

	result[0][0] = data[0] * other[0][0] + data[1] * other[1][0] + data[2] * other[2][0];
	result[0][1] = data[0] * other[0][1] + data[1] * other[1][1] + data[2] * other[2][1];
	result[0][2] = data[0] * other[0][2] + data[1] * other[1][2] + data[2] * other[2][2];
	result[1][0] = data[3] * other[0][0] + data[4] * other[1][0] + data[5] * other[2][0];
	result[1][1] = data[3] * other[0][1] + data[4] * other[1][1] + data[5] * other[2][1];
	result[1][2] = data[3] * other[0][2] + data[4] * other[1][2] + data[5] * other[2][2];
	result[2][0] = data[6] * other[0][0] + data[7] * other[1][0] + data[8] * other[2][0];
	result[2][1] = data[6] * other[0][1] + data[7] * other[1][1] + data[8] * other[2][1];
	result[2][2] = data[6] * other[0][2] + data[7] * other[1][2] + data[8] * other[2][2];

	*this = result;
	return *this;
}

template<typename T>
mat3<T> mat3<T>::operator*(const T s) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result.data[i] = data[i] * s;
	}
	return result;
}

template<typename T>
mat3<T> mat3<T>::operator/(const T s) {
	mat3<T> result;
	for (int i = 0; i < 9; ++i) {
		result.data[i] = data[i] / s;
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
T* mat3<T>::operator[](const std::size_t index) {
	return &data[index * 3];
}

template<typename T>
const T* mat3<T>::operator[](const std::size_t index) const {
	return &data[index * 3];
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
	return m[0][0] + m[1][1] + m[2][2];
}

template<typename T>
T determinant(const mat3<T>& m) {
	return	m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1]
		+	m[0][1] * m[1][2] * m[2][0] - m[0][1] * m[1][0] * m[2][2]
		+	m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0];
}

template<typename T>
mat3<T> transpose(const mat3<T>& other) {
	mat3<T> tmat3(other);
	std::swap(tmat3[0][1], tmat3[1][0]);
	std::swap(tmat3[0][2], tmat3[2][0]);
	std::swap(tmat3[1][2], tmat3[2][1]);
	return tmat3;
}

template<typename T>
mat3<T> inverse(const mat3<T>& m) {
	T det = determinant(m);

	mat3<T> inverseMat;
	inverseMat[0][0] = +m[1][1] * m[2][2] - m[1][2] * m[2][1];
	inverseMat[0][1] = -m[0][1] * m[2][2] + m[0][2] * m[2][1];
	inverseMat[0][2] = +m[0][1] * m[1][2] - m[0][2] * m[1][1];
	inverseMat[1][0] = -m[1][0] * m[2][2] + m[1][2] * m[2][0];
	inverseMat[1][1] = +m[0][0] * m[2][2] - m[0][2] * m[2][0];
	inverseMat[1][2] = -m[0][0] * m[1][2] + m[0][2] * m[1][0];
	inverseMat[2][0] = +m[1][0] * m[2][1] - m[1][1] * m[2][0];
	inverseMat[2][1] = -m[0][0] * m[2][1] + m[0][1] * m[2][0];
	inverseMat[2][2] = +m[0][0] * m[1][1] - m[0][1] * m[1][0];

	inverseMat /= det;
	return inverseMat;
}

template<typename T>
bool isIdentity(const mat3<T>& m) {
	bool isDiagonalOne = m[0][0] == m[1][1] == m[2][2] == static_cast<T>(1);
	bool isOutsideNull = m[0][1] == m[0][2] == m[1][0] == m[1][2] == m[2][0] == m[2][1] == static_cast<T>(0);
	return isDiagonalOne && isOutsideNull;
}

// Floating point error propagation results in errors of signifact margin.
template<typename T>
bool isIdentityCorrected(const mat3<T>& m) {
	const T epsilon = 0.000001f;
	if (m[0][0] > 1.f + epsilon || m[0][0] < 1.f - epsilon) return false;
	if (m[1][1] > 1.f + epsilon || m[1][1] < 1.f - epsilon) return false;
	if (m[2][2] > 1.f + epsilon || m[2][2] < 1.f - epsilon) return false;
	if (m[0][1] > epsilon || m[0][1] < -epsilon) return false;
	if (m[0][2] > epsilon || m[0][2] < -epsilon) return false;
	if (m[1][0] > epsilon || m[1][0] < -epsilon) return false;
	if (m[1][2] > epsilon || m[1][2] < -epsilon) return false;
	if (m[2][0] > epsilon || m[2][0] < -epsilon) return false;
	if (m[2][1] > epsilon || m[2][1] < -epsilon) return false;

	return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, mat3<T>& mat) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			os << mat[i][j] << "\t";
		}
		os << '\n';
	}
	return os;
}

// Name alias the common matrix types 
using mat3i = mat3<int>;
using mat3u = mat3<unsigned>;
using mat3f = mat3<float>;
using mat3d = mat3<double>;

}