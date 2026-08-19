#pragma once
#include <functional>
#include <array>


template<size_t size, typename T>
struct vec {
    std::array<T, size> data;
    bool operator==(const vec<size, T>& other) const {
        return data == other.data;
    }
    bool operator!=(const vec<size, T>& other) const {
        return data != other.data;
    }
    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const {
        return data[index];
    }
    vec<size, T> operator+(const vec<size, T>& other) const {
        vec<size, T> result;
        for (size_t i = 0; i < size; i++) {
            result.data[i] = data[i] + other[i];
        }
        return result;
    }
    vec<size, T> operator-(const vec<size, T>& other) const {
        vec<size, T> result;
        for (size_t i = 0; i < size; i++) {
            result.data[i] = data[i] - other[i];
        }
        return result;
    }

    vec operator* (const int val) {
        vec res;
        for (size_t i = 0; i < size; i++) {
            res.data[i] = data[i] * val;
        }
        return res;
    }

    vec operator/ (const int& val) {
        vec res;
        for (size_t i = 0; i < size; i++) {
            res.data[i] = data[i] / val;
        }
        return res;
    }
    void operator+=(const vec<size, T>& other) {
        for (size_t i = 0; i < size; i++) {
            data[i] += other[i];
        }
    }
    void operator-=(const vec<size, T>& other) {
        for (size_t i = 0; i < size; i++) {
            data[i] -= other[i];
        }
    }
    void operator*=(const int& val) {
        for (size_t i = 0; i < size; i++) {
            data[i] *= val;
        }
    }
    void operator/=(const int& val) {
        for (size_t i = 0; i < size; i++) {
            data[i] /= val;
        }
    }
    void len() {
        T res = 0;
        for (size_t i = 0; i < size; i++) {
            res += ::abs(data[i]);
        }
    }

    vec abs() {
        int res = 0;
        for (auto& i : data) {
            res += ::abs(i);
        }
        return res;
    }

};

template <typename  T>
struct vec<4, T> {
    union {
        struct {
            T x, y, z, w;
        };

        std::array<T, 4> data;
    };

    vec() : x{}, y{}, z{} {}

    vec(T x, T y, T z, T w)
        : x(x), y(y), z(z), w(w) {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    bool operator==(const vec& other) const {
        return data == other.data;
    }

    bool operator!=(const vec& other) const {
        return data != other.data;
    }

    vec operator+(const vec& other) const {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
            w + other.w
        };
    }

    vec operator-(const vec& other) const {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
            w - other.w
        };
    }

    vec operator*(const int& other) const {
        return{
            x * other,
            y * other,
            z * other,
            w * other
        };
    }

    vec operator/(const int& other) const {
        return{
            x / other,
            y / other,
            z / other,
            w / other
        };
    }

    void operator+=(const vec& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    void operator-=(const vec& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    void operator*=(const int& other) {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
    }

    void operator/=(const int& other) {
        x /= other;
        y /= other;
        z /= other;
        w /= other;
    }

    T len() const {
        return ::abs(x) + ::abs(y) + ::abs(z) + ::abs(w);
    }
    vec abs() {
        return {::abs(x), ::abs(y), ::abs(z), ::abs(w)};
    }
};

template<typename T>
struct vec<3, T> {
    union {
        struct {
            T x, y, z;
        };

        std::array<T, 3> data;
    };

    vec() : x{}, y{}, z{} {}

    vec(T x, T y, T z)
        : x(x), y(y), z(z) {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    bool operator==(const vec& other) const {
        return data == other.data;
    }

    bool operator!=(const vec& other) const {
        return data != other.data;
    }

    vec operator+(const vec& other) const {
        return {
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    vec operator-(const vec& other) const {
        return {
            x - other.x,
            y - other.y,
            z - other.z
        };
    }

    vec operator*(const int& other) const {
        return {
            x * other,
            y * other,
            z * other
        };
    }

    vec operator/(const int& other) const {
        return {
            x / other,
            y / other,
            z / other
        };
    }
    void operator+=(const vec& other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void operator-=(const vec& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    void operator*=(const int& other) {
        x *= other;
        y *= other;
        z *= other;
    }
    void operator/=(const int& other) {
        x /= other;
        y /= other;
        z /= other;
    }

    T len() const {
        return ::abs(x) + ::abs(y) + ::abs(z);
    }
    vec abs() {
        return {::abs(x), ::abs(y), ::abs(z)};
    }
};

template<typename T>
struct vec<2, T> {
    union {
        struct {
            T x, y;
        };

        std::array<T, 2> data;
    };

    vec() : x{}, y{} {}

    vec(T x, T y)
        : x(x), y(y) {}

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    bool operator==(const vec& other) const {
        return data == other.data;
    }

    bool operator!=(const vec& other) const {
        return data != other.data;
    }

    vec operator+(const vec& other) const {
        return {
            x + other.x,
            y + other.y
        };
    }

    vec operator-(const vec& other) const {
        return {
            x - other.x,
            y - other.y
        };
    }

    vec operator*(const int& other) const {
        return {
            x * other,
            y * other
        };
    }
    vec operator/(const int& other) const {
        return {
            x / other,
            y / other
        };
    }

    void operator+=(const vec& other) {
        x += other.x;
        y += other.y;
    }

    void operator-=(const vec& other) {
        x -= other.x;
        y -= other.y;
    }

    void operator*=(const int& other) {
        x *= other;
        y *= other;
    }
    void operator/=(const int& other) {
        x /= other;
        y /= other;
    }

    T len() const {
        return ::abs(x) + ::abs(y);
    }

    vec abs() {
        return {::abs(x), ::abs(y)};
    }
};


template <size_t size, typename T>
struct vecHash {
    size_t operator()(const vec<size, T>& val) const {
        std::hash<T> hasher;
        size_t result = 0;
        for (int i=0; i < size; i++) {
            result ^= hasher(val[i]);
        }
        return result;
    }
};

using ivec4 = vec<4, int>;
using ivec3 = vec<3, int>;
using ivec2 = vec<2, int>;
using llvec4 = vec<4, long long>;
using llvec3 = vec<3, long long>;
using llvec2 = vec<2, long long>;

using ivec4hash = vecHash<4, int>;
using ivec3hash = vecHash<3, int>;
using ivec2hash = vecHash<2, int>;
using llvec4hash = vecHash<4, long long>;
using llvec3hash = vecHash<3, long long>;
using llvec2hash = vecHash<2, long long>;