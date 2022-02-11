#pragma once
#include "vec3.hpp"
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

	T* operator[](const std::size_t index);

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
	data[0][0] = data[0][1] = data[0][2] = data[0][3] = v[0];
	data[1][0] = data[1][1] = data[1][2] = data[1][3] = v[1];
	data[2][0] = data[2][1] = data[2][2] = data[2][3] = v[2];
	data[3][0] = data[3][1] = data[3][2] = data[3][3] = v[3];
}

template<typename T>
mat4<T>::mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3) {
	data[0][0] = col0[0]; data[1][0] = col0[1]; data[2][0] = col0[2]; data[3][0] = col0[3];
	data[0][1] = col1[0]; data[1][1] = col1[1]; data[2][1] = col1[2]; data[3][1] = col1[3];
	data[0][2] = col2[0]; data[1][2] = col2[1]; data[2][2] = col2[2]; data[3][2] = col2[3];
	data[0][3] = col3[0]; data[1][3] = col3[1]; data[2][3] = col3[2]; data[3][3] = col3[3];
}

template<typename T>
mat4<T>::mat4(const mat4<T>& other) {
	std::memcpy(data, other.data, sizeof(T) * 16);
}

template<typename T>
mat4<T>& mat4<T>::operator=(const mat4<T>& other) {
	std::memcpy(data, other.data, sizeof(T) * 16);
	return *this;
}

template<typename T>
bool mat4<T>::operator==(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		if (data[i] != other.data[i]) return false;
	}
	return true;
}
template<typename T>
bool mat4<T>::operator!=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		if (data[i] == other.data[i]) return false;
	}
	return true;
}

template<typename T>
mat4<T> mat4<T>::operator+(const mat4<T>& other) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] = data[i] + other.data[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator-(const mat4<T>& other) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result[i] = data[i] - other.data[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator*(const mat4<T>& other) {
	mat4<T> result;
	result[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8] + data[3] * other.data[12];
	result[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9] + data[3] * other.data[13];
	result[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10] + data[3] * other.data[14];
	result[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3] * other.data[15];

	result[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8] + data[7] * other.data[12];
	result[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9] + data[7] * other.data[13];
	result[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10] + data[7] * other.data[14];
	result[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7] * other.data[15];

	result[8] = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8] + data[11] * other.data[12];
	result[9] = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9] + data[11] * other.data[12];
	result[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10] + data[11] * other.data[12];
	result[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11] * other.data[12];

	result[12] = data[12] * other.data[0] + data[13] * other.data[4] + data[14] * other.data[8] + data[15] * other.data[12];
	result[13] = data[12] * other.data[1] + data[13] * other.data[5] + data[14] * other.data[9] + data[15] * other.data[13];
	result[14] = data[12] * other.data[2] + data[13] * other.data[6] + data[14] * other.data[10] + data[15] * other.data[14];
	result[15] = data[12] * other.data[3] + data[13] * other.data[7] + data[14] * other.data[11] + data[15] * other.data[15];
	return result;
}

template<typename T>
mat4<T>& mat4<T>::operator+=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] += other.data[i];
	}
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator-=(const mat4<T>& other) {
	for (int i = 0; i < 16; ++i) {
		data[i] -= other.data[i];
	}
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator*=(const mat4<T>& other) {
	mat4<T> result;
	result[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8] + data[3] * other.data[12];
	result[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9] + data[3] * other.data[13];
	result[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10] + data[3] * other.data[14];
	result[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3] * other.data[15];

	result[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8] + data[7] * other.data[12];
	result[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9] + data[7] * other.data[13];
	result[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10] + data[7] * other.data[14];
	result[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7] * other.data[15];

	result[8] = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8] + data[11] * other.data[12];
	result[9] = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9] + data[11] * other.data[12];
	result[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10] + data[11] * other.data[12];
	result[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11] * other.data[12];

	result[12] = data[12] * other.data[0] + data[13] * other.data[4] + data[14] * other.data[8] + data[15] * other.data[12];
	result[13] = data[12] * other.data[1] + data[13] * other.data[5] + data[14] * other.data[9] + data[15] * other.data[13];
	result[14] = data[12] * other.data[2] + data[13] * other.data[6] + data[14] * other.data[10] + data[15] * other.data[14];
	result[15] = data[12] * other.data[3] + data[13] * other.data[7] + data[14] * other.data[11] + data[15] * other.data[15];
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
T* mat4<T>::operator[](const std::size_t index) {
	return &data[index * 4];
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
	return m[0][0] + m[1][1] + m[2][2] + m[3][3];
}

// Yet to find a good use for this
template<typename T>
T determinant(const mat4<T>& m) {
	
}

template<typename T>
mat4<T> transpose(const mat4<T>& m) {
	mat4<T> tmat(m);
	std::swap(tmat[0][1], tmat[1][0]);
	std::swap(tmat[0][2], tmat[2][0]);
	std::swap(tmat[0][3], tmat[3][0]);
	std::swap(tmat[1][2], tmat[2][1]);
	std::swap(tmat[1][3], tmat[3][1]);
	std::swap(tmat[2][3], tmat[3][2]);
	return tmat;
}

// TODO: Implement this functionality
template<typename T>
mat4<T> inverse(const mat4<T>& m) {}

template<typename T>
bool isIdentity(const mat4<T>& m) {
	bool isDiagonalOne = m[0][0] == m[1][1] == m[2][2] == m[3][3] == static_cast<T>(1);
	bool isOutsideNull = m[0][1] == m[0][2] == m[0][3] == 
						 m[1][0] == m[1][2] == m[1][3] == 
						 m[2][0] == m[2][1] == m[2][3] == 
					     m[3][0] == m[3][1] == m[3][2] == static_cast<T>(0);
	return isDiagonalOne && isOutsideNull;
}

inline mat4<float> lookAt(const vec3f& pos, const vec3f& dir, const vec3f& up) {
	const vec3f t = normalize(dir - pos);
	const vec3f s = normalize(cross(t, vec3f(0.f, 1.f, 0.f)));
	const vec3f u = cross(s, t);
	mat4<float> result;
	result[0][0] = s.x;
	result[1][0] = s.y;
	result[2][0] = s.z;
	result[0][1] = u.x;
	result[1][1] = u.y;
	result[2][1] = u.z;
	result[0][2] = -t.x;
	result[1][2] = -t.y;
	result[2][2] = -t.z;
	result[3][0] = -dot(pos, s);
	result[3][1] = -dot(pos, u);
	result[3][2] = dot(pos, t);
	result[3][3] = 1.f;
	return result;
}

inline mat4<float> orthogonal(const float b, const float t, const float l, const float r, const float near, const float far) {
	mat4<float> result;
	result[0][0] = 2.f / (r - l);
	result[1][1] = 2.f / (t - b);
	result[2][2] = -2.f / (far - near);
	result[3][3] = 1.f;
	result[0][3] = -(r + l) / (r - l);
	result[1][3] = -(t + b) / (t - b);
	result[2][3] = -(far + near) / (far - near);
	return result;
}

inline mat4<float> perspective(const float fov, const float aspectRatio, const float near, const float far) {
	const float tanHalfFov = std::tan(fov / 2.f);
	mat4<float> result(0.f);
	result[0][0] = 1.f / (aspectRatio * tanHalfFov);
	result[1][1] = 1.f / tanHalfFov;
	result[2][2] = -(far + near) / (far - near);
	result[3][2] = (-2.f * far * near) / (far - near);
	result[2][3] = -1.f;
	return result;
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

// Name alias the common matrix types 
using mat4i = mat4<int>;
using mat4u = mat4<unsigned>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

}