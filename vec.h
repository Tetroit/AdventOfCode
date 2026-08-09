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
            result[i] = data[i] + other[i];
        }
        return result;
    }
    vec<size, T> operator-(const vec<size, T>& other) const {
        vec<size, T> result;
        for (size_t i = 0; i < size; i++) {
            result[i] = data[i] - other[i];
        }
        return result;
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
    void len() {
        T res = 0;
        for (size_t i = 0; i < size; i++) {
            res += abs(data[i]);
        }
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

    vec& operator+=(const vec& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec& operator-=(const vec& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    T len() const {
        return abs(x) + abs(y) + abs(z);
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

    vec& operator+=(const vec& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    vec& operator-=(const vec& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    T len() const {
        return abs(x) + abs(y);
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

using ivec3 = vec<3, int>;
using ivec2 = vec<2, int>;
using llvec3 = vec<3, long long>;
using llvec2 = vec<2, long long>;

using ivec3hash = vecHash<3, int>;
using ivec2hash = vecHash<2, int>;
using llvec3hash = vecHash<3, long long>;
using llvec2hash = vecHash<2, long long>;