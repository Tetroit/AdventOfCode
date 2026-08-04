#pragma once
#include <functional>

struct ivec2 {
    int x;
    int y;
    ivec2(int _x, int _y) : x(_x), y(_y) {}
    bool operator ==(const ivec2 &other) const {
        return x == other.x && y == other.y;
    }
    bool operator !=(const ivec2 &other) const {
        return x != other.x || y != other.y;
    }
    ivec2 operator +(const ivec2 &other) const {
        return {x + other.x, y + other.y};
    }
    ivec2 operator -(const ivec2 &other) const {
        return {x - other.x, y - other.y};
    }
    void operator +=(const ivec2 &other) {
        x += other.x;
        y += other.y;
    }
    void operator -=(const ivec2 &other) {
        x -= other.x;
        y -= other.y;
    }
    [[nodiscard]] int len() const {return x + y;}
};

struct ivec2hash {
    size_t operator()(const ivec2& val) const {
        std::hash<int> hasher;
        return hasher(val.x) ^ hasher(val.y);
    }
};