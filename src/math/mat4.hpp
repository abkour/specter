#pragma once
#include "vec3.hpp"
#include "vec4.hpp"

namespace specter {

// Defines a 4x4 matrix of type T in row-major order
template<typename T>
struct mat4 {

	mat4();
	mat4(const T value);
	mat4(const vec4<T>& v);
	mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3);
	mat4(const mat4<T>& other);
	mat4(mat4<T>&& other) noexcept;
	mat4<T>& operator=(const mat4<T>& other);
	mat4<T>& operator=(mat4<T>&& other);

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
	const T* operator[](const std::size_t index) const;

	vec4<T> col(const std::size_t index) const;
	vec4<T> row(const std::size_t index) const;

	T data[16];
};

// Name alias the common matrix types 
using mat4i = mat4<int>;
using mat4u = mat4<unsigned>;
using mat4f = mat4<float>;
using mat4d = mat4<double>;

template<typename T>
std::ostream& operator<<(std::ostream& os, const mat4<T>& m) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			os << m.data[i * 4 + j] << '\t';
		}
		os << '\n';
	}
	return os;
}

template<typename T>
mat4<T>::mat4() {
	std::memset(data, 0, sizeof(T) * 16);
	data[0] = data[5] = data[10] = data[15] = T();
}

template<typename T>
mat4<T>::mat4(const T value) {
	std::memset(data, 0, sizeof(T) * 16);
	data[0] = data[5] = data[10] = data[15] = value;
}

template<typename T>
mat4<T>::mat4(const vec4<T>& v) {
	for (int i = 0; i < 4; ++i) {
		data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = data[i * 4 + 3] = v[i];
	}
}

template<typename T>
mat4<T>::mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3) {
	for (int i = 0; i < 4; ++i) {
		data[i * 4] = col0[i]; data[i * 4 + 1] = col1[i]; data[i * 4 + 2] = col2[i]; data[i * 4 + 3] = col3[i];
	}
}

template<typename T>
mat4<T>::mat4(const mat4<T>& other) {
	std::memcpy(data, other.data, sizeof(T) * 16);
}

template<typename T>
mat4<T>::mat4(mat4<T>&& other) noexcept {
	std::swap(data, other.data);
}

template<typename T>
mat4<T>& mat4<T>::operator=(const mat4<T>& other) {
	std::memcpy(data, other.data, sizeof(T) * 16);
	return *this;
}

template<typename T>
mat4<T>& mat4<T>::operator=(mat4<T>&& other) {
	std::swap(data, other.data);
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
		result.data[i] = data[i] + other.data[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator-(const mat4<T>& other) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result.data[i] = data[i] - other.data[i];
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator*(const mat4<T>& other) {
	mat4<T> result;
	result.data[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8] + data[3] * other.data[12];
	result.data[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9] + data[3] * other.data[13];
	result.data[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10] + data[3] * other.data[14];
	result.data[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3] * other.data[15];

	result.data[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8] + data[7] * other.data[12];
	result.data[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9] + data[7] * other.data[13];
	result.data[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10] + data[7] * other.data[14];
	result.data[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7] * other.data[15];

	result.data[8] = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8] + data[11] * other.data[12];
	result.data[9] = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9] + data[11] * other.data[13];
	result.data[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10] + data[11] * other.data[14];
	result.data[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11] * other.data[15];

	result.data[12] = data[12] * other.data[0] + data[13] * other.data[4] + data[14] * other.data[8] + data[15] * other.data[12];
	result.data[13] = data[12] * other.data[1] + data[13] * other.data[5] + data[14] * other.data[9] + data[15] * other.data[13];
	result.data[14] = data[12] * other.data[2] + data[13] * other.data[6] + data[14] * other.data[10] + data[15] * other.data[14];
	result.data[15] = data[12] * other.data[3] + data[13] * other.data[7] + data[14] * other.data[11] + data[15] * other.data[15];
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
	result.data[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8] + data[3] * other.data[12];
	result.data[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9] + data[3] * other.data[13];
	result.data[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10] + data[3] * other.data[14];
	result.data[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3] * other.data[15];

	result.data[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8] + data[7] * other.data[12];
	result.data[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9] + data[7] * other.data[13];
	result.data[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10] + data[7] * other.data[14];
	result.data[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7] * other.data[15];

	result.data[8] = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8] + data[11] * other.data[12];
	result.data[9] = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9] + data[11] * other.data[13];
	result.data[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10] + data[11] * other.data[14];
	result.data[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11] * other.data[15];

	result.data[12] = data[12] * other.data[0] + data[13] * other.data[4] + data[14] * other.data[8] + data[15] * other.data[12];
	result.data[13] = data[12] * other.data[1] + data[13] * other.data[5] + data[14] * other.data[9] + data[15] * other.data[13];
	result.data[14] = data[12] * other.data[2] + data[13] * other.data[6] + data[14] * other.data[10] + data[15] * other.data[14];
	result.data[15] = data[12] * other.data[3] + data[13] * other.data[7] + data[14] * other.data[11] + data[15] * other.data[15];
	*this = result;
	return *this;
}

template<typename T>
mat4<T> mat4<T>::operator*(const T s) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result.data[i] = data[i] * s;
	}
	return result;
}

template<typename T>
mat4<T> mat4<T>::operator/(const T s) {
	mat4<T> result;
	for (int i = 0; i < 16; ++i) {
		result.data[i] = data[i] / s;
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
const T* mat4<T>::operator[](const std::size_t index) const {
	return &data[index * 4];
}

template<typename T>
vec4<T> mat4<T>::col(const std::size_t index) const {
	return vec4<T>(data[index], data[index + 4], data[index + 8], data[index + 12]);
}

template<typename T>
vec4<T> mat4<T>::row(const std::size_t index) const {
	return vec4<T>(data[index * 4], data[index * 4 + 1], data[index * 4 + 2], data[index * 4 + 3]);
}

template<typename T>
T trace(const mat4<T>& m) {
	return m[0][0] + m[1][1] + m[2][2] + m[3][3];
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

template<typename T>
bool isIdentity(const mat4<T>& m) {
	bool isDiagonalOne = m[0][0] == m[1][1] == m[2][2] == m[3][3] == static_cast<T>(1);
	bool isOutsideNull0 = m[0][1] == m[0][2] == m[0][3] == static_cast<T>(0);
	bool isOutsideNull1 = m[1][0] == m[1][2] == m[1][3] == static_cast<T>(0);
	bool isOutsideNull2 = m[2][0] == m[2][1] == m[2][3] == static_cast<T>(0);
	bool isOutsideNull3 = m[3][0] == m[3][1] == m[3][2] == static_cast<T>(0);

	return isDiagonalOne && isOutsideNull0 && isOutsideNull1 && isOutsideNull2 && isOutsideNull3;
}

// Floating point error propagation results in errors of signifact margin.
template<typename T>
bool isIdentityCorrected(const mat4<T>& m) {
	const T epsilon = 0.000001f;
	if (m[0][0] > 1.f + epsilon || m[0][0] < 1.f - epsilon) return false;
	if (m[1][1] > 1.f + epsilon || m[1][1] < 1.f - epsilon) return false;
	if (m[2][2] > 1.f + epsilon || m[2][2] < 1.f - epsilon) return false;
	if (m[3][3] > 1.f + epsilon || m[3][3] < 1.f - epsilon) return false;
	if (m[0][1] > epsilon || m[0][1] < -epsilon) return false;
	if (m[0][2] > epsilon || m[0][2] < -epsilon) return false;
	if (m[0][3] > epsilon || m[0][3] < -epsilon) return false;
	if (m[1][0] > epsilon || m[1][0] < -epsilon) return false;
	if (m[1][2] > epsilon || m[1][2] < -epsilon) return false;
	if (m[1][3] > epsilon || m[1][3] < -epsilon) return false;
	if (m[2][0] > epsilon || m[2][0] < -epsilon) return false;
	if (m[2][1] > epsilon || m[2][1] < -epsilon) return false;
	if (m[2][3] > epsilon || m[2][3] < -epsilon) return false;
	if (m[3][0] > epsilon || m[3][0] < -epsilon) return false;
	if (m[3][1] > epsilon || m[3][1] < -epsilon) return false;
	if (m[3][2] > epsilon || m[3][2] < -epsilon) return false;

	return true;
}

inline mat4f lookAt(const vec3f& pos, const vec3f& dir, const vec3f& up) {
	const vec3f t = normalize(dir - pos);
	const vec3f s = normalize(cross(t, vec3f(0.f, 1.f, 0.f)));
	const vec3f u = cross(s, t);
	mat4f result;
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

inline mat4f orthogonal(const float b, const float t, const float l, const float r, const float near, const float far) {
	mat4f result;
	result[0][0] = 2.f / (r - l);
	result[1][1] = 2.f / (t - b);
	result[2][2] = -2.f / (far - near);
	result[3][3] = 1.f;
	result[0][3] = -(r + l) / (r - l);
	result[1][3] = -(t + b) / (t - b);
	result[2][3] = -(far + near) / (far - near);
	return result;
}

inline mat4f perspective(const float fov, const float aspectRatio, const float near, const float far) {
	const float tanHalfFov = std::tan(fov / 2.f);
	mat4f result(0.f);
	result[0][0] = 1.f / (aspectRatio * tanHalfFov);
	result[1][1] = 1.f / tanHalfFov;
	result[2][2] = -(far + near) / (far - near);
	result[3][2] = (-2.f * far * near) / (far - near);
	result[2][3] = -1.f;
	return result;
}

template<typename T>
inline mat4<T> scale(mat4<T> m, const T s) {
	m[0][0] *= s;
	m[1][1] *= s;
	m[2][2] *= s;
	return m;
}

template<typename T>
inline mat4<T> translate(mat4<T> m, const vec3<T>& v) {
	m[3][0] += v.x;
	m[3][1] += v.y;
	m[3][2] += v.z;
	return m;
}

}