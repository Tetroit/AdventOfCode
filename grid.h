#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>

template <std::size_t X, std::size_t Y, typename T>
class Grid {
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

    void clear(T value) {
        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                grid[y][x] = value;
            }
        }
    }

    void fillFromLine(int y, std::string line, std::function<T(char)> convert) {
        for (int x = 0; x < line.length(); ++x) {
            grid[y][x] = convert(line[x]);
        }
    }

    bool inRange(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return false;
        }
        return true;
    }
    T get(int x, int y) {
        assert(inRange(x, y));
        return grid[y][x];
    }
    void set(int x, int y, T val) {
        grid[y][x] = val;
    }
    std::pair<int, int> clamp(int x, int y) {
        return {std::clamp(x,0, (int)X-1) ,std::clamp(y,0, (int)Y-1)};
    }
    std::vector<std::pair<int,int>> getNeighbourCoords8(int x, int y) {
        std::vector<std::pair<int,int>> neighbours;
        for (int xi = x-1; xi <= x+1; ++xi) {
            if (xi < 0 || xi >= width) {
                continue;
            }
            for (int yi = y-1; yi <= y+1; ++yi) {
                if (yi < 0 || yi >= height || (yi == y && xi == x)) {
                    continue;
                }
                neighbours.emplace_back(xi,yi);
            }
        }
        return neighbours;
    }
    std::vector<std::pair<int,int>> getNeighbourCoords4(int x, int y) {
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
    int countNeighbours8(int x, int y, std::function<bool(T)> pred) {
        int cnt = 0;
        for (int xi = x-1; xi <= x+1; ++xi) {
            if (xi < 0 || xi >= width) {
                continue;
            }
            for (int yi = y-1; yi <= y+1; ++yi) {
                if (yi < 0 || yi >= height || (yi == y && xi == x)) {
                    continue;
                }
                if (pred(get(xi, yi))) {cnt++;}
            }
        }
        return cnt;
    }
    int countNeighbours4(int x, int y, std::function<bool(T)> pred) {
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
    int count(std::function<bool(T)> pred) {
        int cnt = 0;
        for (const auto& arr : grid) {
            for (const auto& val : arr) {
                if (pred(val)) cnt++;
            }
        }
        return cnt;
    }
    template <typename Fn>
    requires std::invocable<Fn&, T>
    void foreach(Fn&& fn) {
        for (const auto& arr : grid) {
            for (const auto& val : arr) {
                std::invoke(fn, val);
            }
        }
    }
    template <typename Fn>
    requires std::invocable<Fn&, int, int>
    void foreachCoord(Fn&& fn) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::invoke(fn, x, y);
            }
        }
    }

    void print() {
        for (int i=0; i<height; ++i) {
            for (int j=0; j<width; ++j) {
                std::cout << grid[i][j];
            }
            std::cout << std::endl;
        }
    }
    void print(std::function<char(T)> font) {
        for (int i=0; i<height; ++i) {
            for (int j=0; j<width; ++j) {
                std::cout << font(grid[i][j]);
            }
            std::cout << std::endl;
        }
    }

    int dijkstra(const int& startX, const int& startY, const int& endX, const int& endY,
        std::function<int(T)> costFunc,
        std::function<bool(T)> isWall) {

        using weight_t = int;
        std::array<std::array<weight_t, width>, height> weights;
		std::vector<std::tuple<weight_t, int, int>> queue;

        auto getCost = [&](int x, int y) -> int {
            return costFunc(grid[startY][startX]);
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
