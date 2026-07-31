#pragma once

#include <string>
#include <vector>

template <typename T>
struct UnorderedSetHash {
	size_t operator()(const std::unordered_set<T>& set) const {
		size_t hash = 0;
		for (auto& x : set) {
			hash ^= x;
		}
		return hash;
	}
};

class Utils {
public:
	static std::vector<std::string> split(const std::string& str, char sep)
	{
		auto substr = str;
		std::vector<std::string> res;
		size_t pos = substr.find(sep);
		while (pos != std::string::npos) {
			res.push_back(substr.substr(0, pos));
			substr = substr.substr(pos + 1, substr.size() - pos - 1);
			pos = substr.find(sep);
		}
		if (!substr.empty())
			res.push_back(substr);
		return res;
	}

	static std::vector<std::string> split(const std::string& str, const std::string& sep)
	{
		auto substr = str;
		std::vector<std::string> res;
		size_t pos = substr.find(sep);
		while (pos != std::string::npos) {
			res.push_back(substr.substr(0, pos));
			substr = substr.substr(pos + sep.length(), substr.size() - pos - sep.length());
			pos = substr.find(sep);
		}
		if (!substr.empty())
			res.push_back(substr);

		return res;
	}

	static std::vector<int> findAll(const std::string& str, const std::string& key) {
		size_t it = 0;
		std::vector<int> res;
		while (it < str.length()) {
			it = str.find(key, it);
			if (it != std::string::npos) {
				res.push_back(it);
				it += key.length();
			}
		}
		return res;
	}

	template<typename T>
	static constexpr int digitsIn (T num) {
		if (num == 0) return 1;
		int res = 0;
		while (num != 0) {
			res++;
			num /= 10;
		}
		return res;
	}

	template<typename T>
	static constexpr T powInt(T num, int power) {
		T res = 1;
		for (int i = 0; i < power; i++) {
			res *= num;
		}
		return res;
	}

	template<typename T>
	static constexpr void Log(const std::vector<T>& arr) {
		for (const T& el : arr) {
			std::cout << el << " ";
		}
		std::cout << '\n';
	}
	template<typename T, typename U>
	static constexpr void Log(const std::unordered_map<T,U>& arr) {
		for (const auto& pair : arr) {
			std::cout << pair.first << ' ' << pair.second << '\n';
		}
		std::cout << '\n';
	}

	template<typename T>
	static bool inRange(T width, T height, T x, T y) {
		if (x < width && y < height && x >= 0 && y >= 0) {
			return  true;
		}
		return false;
	}


	inline static const std::vector<std::pair<int, int>> UtilFacingVec {
		{1,0},
		{0,-1},
		{-1,0},
		{0,1},
	};
	template<typename T>
	static void MoveInDir(T& x, T& y, int dir) {
		auto [dx, dy] = UtilFacingVec.at(dir);
		x = x + dx;
		y = y + dy;
	}

	static std::vector<std::tuple<int, int, int>>::const_iterator UtilInsert(const std::vector<std::tuple<int, int, int>>& arr, int order) {
		for ( int i=0; i<arr.size(); i++ ) {
			if (order < std::get<0>(arr.at(i))) {
				return arr.cbegin() + i;
			}
		}
		return arr.end();
	}

	static int UtilDijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, std::vector<std::pair<int,int>> starts, const int& endX, const int& endY) {
		//weight, x, y
		std::vector<std::tuple<int, int, int>> queue;

		for (auto& [x,y] : starts) {
			queue.emplace(UtilInsert(queue, weights[x][y]), weights[x][y], x, y);
		}
		while (!queue.empty()) {
			auto [score, x, y] = queue.front();
			std::vector<std::tuple<int, int, int>> toAdd;

			for (int i=0; i<4; i++) {

				auto& [dx, dy] = UtilFacingVec.at(i);

				int nx = x + dx, ny = y + dy;

				if (!inRange(width, height, nx, ny)) {
					continue;
				}

				if (weights[nx][ny] == -1)
					continue;

				int weight = score + 1;
				if (weights[nx][ny] > weight) {
					weights[nx][ny] = weight;
					toAdd.emplace_back(weight, nx, ny);
				}
			}

			queue.erase(queue.begin());

			for (auto& t : toAdd) {
				int weight = std::get<0>(t);
				queue.emplace(UtilInsert(queue, weight), t);
			}
		}

		return weights[endX][endY];
	}


	static int UtilDijkstra(std::vector<std::vector<int>>& weights, const int& width, const int& height, const int& startX, const int& startY, const int& endX, const int& endY) {

		weights[startX][startY] = 0;

		std::vector<std::pair<int,int>> starts {
			{startX, startY}
		};
		UtilDijkstra(weights, width, height, starts, endX, endY);

		return weights[endX][endY];
	}

	static int Loop (int num, int min, int max) {
		int range = max - min + 1;
		int result = (num - min) % range;
		if (result < 0) {
			result += range;
		}
		return result + min;
	}

	static int FloorMod(int n, int mod) {
		int res = n % mod;
		if (res < 0) {
			res += mod;
		}
		return res;
	}
};
