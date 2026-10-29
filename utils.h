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

	template<typename T>
	static T gcd(T a, T b) {
		if (a < 0) a = -a;
		if (b < 0) b = -b;
		while (a != 0 && b != 0) {
			if (a > b) {
				a %= b;
			}
			else {
				b %= a;
			}
		}
		if (a == 0) {return b;}
		return a;
	}

	template<typename T>
	static T lcm(T a, T b) {
		return a * b / gcd(a, b);
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

	template<typename T=int>
	static T Loop (T num, T min, T max) {
		T range = max - min + 1;
		T result = (num - min) % range;
		if (result < 0) {
			result += range;
		}
		return result + min;
	}

	template <typename T>
	static T FloorMod(T n, T mod) {
		T res = n % mod;
		if (res < 0) {
			res += mod;
		}
		return res;
	}

	template <typename T>
	static std::vector<T> minkovskySum (const std::vector<T>& a, const std::vector<T>& b) {
		std::vector<T> result;
		for (const T& ax : a) {
			for (const T& ay : b) {
				result.push_back(ax + ay);
			}
		}
		std::sort(result.begin(), result.end());
		result.erase(std::unique(result.begin(), result.end()), result.end());
		return result;
	}

	template <typename T>
	static std::unordered_set<T> minkovskySum (const std::unordered_set<T>& a, const std::unordered_set<T>& b) {
		std::unordered_set<T> result;
		for (const T& ax : a) {
			for (const T& ay : b) {
				result.insert(ax + ay);
			}
		}
		return result;
	}

	template <std::unsigned_integral T>
	constexpr static T bitmask(int n) {
		if (n == 0) return 0;
		return n == sizeof(T) * 8 ? std::numeric_limits<T>::max() : (T{1} << n) - 1;
	}

	template <std::unsigned_integral T>
	static std::pair<std::make_signed_t<T>, std::make_signed_t<T>>
	extendedEuclid(T a, T b) {
		using S = std::make_signed_t<T>;

		S old_r = static_cast<S>(a), r = static_cast<S>(b);
		S old_s = 1, s = 0;
		S old_t = 0, t = 1;

		while (r != 0) {
			S q = old_r / r;

			S temp = old_r - q * r;
			old_r = r;
			r = temp;

			temp = old_s - q * s;
			old_s = s;
			s = temp;

			temp = old_t - q * t;
			old_t = t;
			t = temp;
		}

		if (old_r < 0) {
			old_s = -old_s;
			old_t = -old_t;
		}

		return {old_s, old_t};
	}
	//ax mod m = 1
	template <std::unsigned_integral T>
	static T inverseMod(T a, T m) {
		using S = std::make_signed_t<T>;
		S sm = static_cast<S>(m);
		S sa = static_cast<S>(a);
		auto [x, y] = extendedEuclid(a,m);
		if (sa * x + sm * y != 1) return -1;
		return static_cast<T>((x % sm + sm) % sm);
	}
	template <std::unsigned_integral T>
	static std::optional<std::pair<T, T>> chineseSolver(std::pair<T, T> a, std::pair<T, T> b) {
		std::pair<T, T> result;
		result.second = lcm(a.second, b.second);

		T d = gcd(a.second, b.second);
		T diff = b.first - a.first;
		if (diff % d != 0) return std::nullopt;

		T diffd = diff / d;
		T ad = a.second / d;
		T bd = b.second / d;

		T inv = inverseMod(ad, bd);
		if (bd != 1 && inv == T(-1)) return std::nullopt;

		T k = bd == 1 ? 0 : FloorMod(inv * diffd, bd);
		result.first = FloorMod(a.first + k * a.second, result.second);

		return result;
	}

	template <std::unsigned_integral T>
	static std::optional<std::pair<T,T>> chineseSolver(const std::vector<std::pair<T, T>>& b) {
		if (b.empty()) return std::nullopt;
		if (b.size() == 1) return b[0];
		std::pair<T, T> result = b.front();
		for (int i=1; i<b.size(); i++) {
			auto op = chineseSolver(result, b[i]);
			if (!op.has_value()) return std::nullopt;
			result = op.value();
		}
		return result;
	}
};
