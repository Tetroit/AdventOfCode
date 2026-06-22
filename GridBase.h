#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

template <typename T>
class GridBase {
public:
    virtual ~GridBase() = default;

    [[nodiscard]] virtual int getWidth() const = 0;
    [[nodiscard]] virtual int getHeight() const = 0;
    [[nodiscard]] virtual const T& getRef(int x, int y) const = 0;
    [[nodiscard]] virtual T& getRef(int x, int y) = 0;
    [[nodiscard]] virtual T get(int x, int y) const;

    virtual void set(int x, int y, T val) = 0;
    virtual void clear(T value) {
        for (std::size_t y = 0; y < getHeight(); ++y) {
            for (std::size_t x = 0; x < getWidth(); ++x) {
                set(x, y, value);
            }
        }
    }
    [[nodiscard]] virtual bool inRange(int x, int y) const {
        if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight()) {
            return false;
        }
        return true;
    }
    [[nodiscard]] virtual std::pair<int, int> clamp(int x, int y) const {
        return {std::clamp(x,0, getWidth()-1) ,std::clamp(y,0, getHeight()-1)};
    }

    virtual void print() {
        for (int i=0; i<getHeight(); ++i) {
            for (int j=0; j<getWidth(); ++j) {
                std::cout << get(j,i);
            }
            std::cout << std::endl;
        }
    }
    virtual void print(std::function<char(T)> font) {
        for (int i=0; i<getHeight(); ++i) {
            for (int j=0; j<getWidth(); ++j) {
                std::cout << font(get(j, i));
            }
            std::cout << std::endl;
        }
    }
    virtual int countNeighbours8(int x, int y, std::function<bool(T)> pred) {
        int cnt = 0;
        for (int xi = x-1; xi <= x+1; ++xi) {
            if (xi < 0 || xi >= getWidth()) {
                continue;
            }
            for (int yi = y-1; yi <= y+1; ++yi) {
                if (yi < 0 || yi >= getHeight() || (yi == y && xi == x)) {
                    continue;
                }
                if (pred(get(xi, yi))) {cnt++;}
            }
        }
        return cnt;
    }
    virtual int countNeighbours4(int x, int y, std::function<bool(T)> pred) {
        int cnt = 0;
        if (inRange(x+1, y)) {
            if (pred(get(x+1, y))) {cnt++;}
        }
        if (inRange(x-1, y)) {
            if (pred(get(x-1, y))) {cnt++;}
        }
        if (inRange(x, y+1)) {
            if (pred(get(x, y+1))) {cnt++;}
        }
        if (inRange(x, y-1)) {
            if (pred(get(x, y-1))) {cnt++;}
        }
        return cnt;
    }

    virtual std::vector<std::pair<int,int>> getNeighbourCoords8(int x, int y) {
        std::vector<std::pair<int,int>> neighbours;
        for (int xi = x-1; xi <= x+1; ++xi) {
            if (xi < 0 || xi >= getWidth()) {
                continue;
            }
            for (int yi = y-1; yi <= y+1; ++yi) {
                if (yi < 0 || yi >= getHeight() || (yi == y && xi == x)) {
                    continue;
                }
                neighbours.emplace_back(xi,yi);
            }
        }
        return neighbours;
    }
    virtual std::vector<std::pair<int,int>> getNeighbourCoords4(int x, int y) {
        std::vector<std::pair<int,int>> neighbours;
        if (inRange(x+1, y)) {
            neighbours.emplace_back(x+1, y);
        }
        if (inRange(x-1, y)) {
            neighbours.emplace_back(x-1, y);
        }
        if (inRange(x, y+1)) {
            neighbours.emplace_back(x, y+1);
        }
        if (inRange(x, y-1)) {
            neighbours.emplace_back(x, y-1);
        }
        return neighbours;
    }
    virtual int count(std::function<bool(T)> pred) {
        int cnt = 0;
        for (int x = 0; x < getWidth(); x++) {
            for (int y = 0; y < getHeight(); y++) {
                if (pred(get(x,y))) cnt++;
            }
        }
        return cnt;
    }
    template <typename Fn>
    requires std::invocable<Fn&, T>
    void foreach(Fn&& fn) {
        for (int x = 0; x < getWidth(); x++) {
            for (int y = 0; y < getHeight(); y++) {
                fn(getRef(x, y));
            }
        }
    }
    template <typename Fn>
    requires std::invocable<Fn&, int, int>
    void foreachCoord(Fn&& fn) {
        for (int y = 0; y < getHeight(); ++y) {
            for (int x = 0; x < getWidth(); ++x) {
                std::invoke(fn, x, y);
            }
        }
    }
    void fillFromLine(int y, std::string line, std::function<T(char)> convert) {
        for (int x = 0; x < line.length(); ++x) {
            set(x,y, convert(line[x]));
        }
    }
    int dijkstra(const int& startX, const int& startY, const int& endX, const int& endY,
        std::function<int(T)> costFunc,
        std::function<bool(T)> isWall) {

        using weight_t = int;
        // std::array<std::array<weight_t, width>, height> weights;
        std::vector<std::vector<weight_t>> weights;
        weights.resize(getHeight());
        for (int y = 0; y < getHeight(); ++y) {
            weights[y].resize(getWidth());
        }
        std::vector<std::tuple<weight_t, int, int>> queue;

        auto getCost = [&](int x, int y) -> int {
            return costFunc(get(x,y));
        };
        auto getWeight = [&](int x, int y) -> int {
            return weights[y][x];
        };
        auto setWeight = [&](int x, int y, weight_t weight) -> void {
            weights[y][x] = weight;
        };
        queue.emplace_back(getCost(startX, startY), startX, startY);
        while (!queue.empty()) {
            auto [score, x, y] = queue.front();
            std::vector<std::tuple<int, int, int>> toAdd;

            for (auto[nx, ny] : getNeighbourCoords4(x, y)) {

                if (isWall(get(nx,ny)))
                    continue;

                int weight = score + getCost(nx, ny);
                if (getWeight(nx, ny) > weight) {
                    setWeight(nx, ny);
                    toAdd.emplace_back(weight, nx, ny);
                }
            }

            queue.erase(queue.begin());

            for (auto& t : toAdd) {
                int weight = std::get<0>(t);
                for (auto it = queue.begin(); it<queue.end(); it++) {
                    if (weight < std::get<0>(*it))
                        queue.insert(it, t);
                }
            }
        }

        return weights[endX][endY];
    }
};

template<typename T>
T GridBase<T>::get(int x, int y) const {
    return getRef(x, y);
}

template <std::size_t X, std::size_t Y, typename T>
class Grid : public GridBase<T> {
    std::array<std::array<T, X>, Y> grid;
public:
    using Type = T;

    static constexpr std::size_t width  = X;
    static constexpr std::size_t height = Y;

    Grid() = default;
    ~Grid() = default;

    template <typename... Args>
    requires (sizeof...(Args) == X * Y)
    Grid(Args&&... vals) {
        std::array<T, X * Y> input{ static_cast<T>(vals)... };

        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                grid[y][x] = input[y * width + x];
            }
        }
    }

    template <std::size_t X1, std::size_t Y1, typename T1>
    requires std::same_as<T, T1>
    explicit Grid (const Grid<X1,Y1,T1>& other) {
        int overlapX = std::min(X, X1);
        int overlapY = std::min(Y, Y1);
        for (int i=0; i<overlapX; ++i) {
            for (int j=0; j<overlapY; ++j) {
                grid[j][i] = other.grid[j][i];
            }
        }
    }

    template <std::size_t X1, std::size_t Y1, typename T1>
    requires std::same_as<T, T1>
    void copyFrom (const Grid<X1,Y1,T1>& other) {
        int overlapX = std::min(X, X1);
        int overlapY = std::min(Y, Y1);
        for (int i=0; i<overlapX; ++i) {
            for (int j=0; j<overlapY; ++j) {
                grid[i][j] = other.grid[i][j];
            }
        }
    }

    Grid<X, Y, T> createCopy() {
        return Grid<X, Y, T>(grid);
    }

    [[nodiscard]] constexpr int getWidth() const override {
        return width;
    }
    [[nodiscard]] constexpr int getHeight() const override {
        return height;
    }
    constexpr T get(int x, int y) const override {
        assert(inRange(x, y));
        return grid[y][x];
    }
    constexpr T& getRef(int x, int y) override {
        assert(inRange(x, y));
        return grid[y][x];
    }
    constexpr const T& getRef(int x, int y) const override {
        assert(inRange(x, y));
        return grid[y][x];
    }
    constexpr void set(int x, int y, T val) override {
        grid[y][x] = val;
    }

    constexpr void clear(T value) override {
        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                grid[y][x] = value;
            }
        }
    }
    [[nodiscard]] constexpr bool inRange(int x, int y) const override {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return false;
        }
        return true;
    }
    [[nodiscard]] constexpr std::pair<int, int> clamp(int x, int y) const override {
        return {std::clamp(x,0, (int)X-1) ,std::clamp(y,0, (int)Y-1)};
    }
};

template <typename T>
class DynamicGrid final : public GridBase<T> {
    std::vector<std::vector<T>> grid;
public:
    [[nodiscard]] int getWidth() const override {
        if (grid.empty()) return 0;
        return grid[0].size();
    }
    [[nodiscard]] int getHeight() const override {
        return grid.size();
    }
    [[nodiscard]] const T & getRef(int x, int y) const override {
        return grid[y][x];
    }
    [[nodiscard]] T & getRef(int x, int y) override {
        return grid[y][x];
    }
    void set(int x, int y, T val) override {
        grid[y][x] = val;
    }
};
