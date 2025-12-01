#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

struct Task
{
	int part(std::vector<int>& arr, int l, int r)
	{
		int et = arr[l];
		int i = l + 1;
		int j = r;
		while (true) {
			while (i <= r && arr[i] <= et) i++;
			while (arr[j] > et) j--;

			if (i >= j) break;
			std::swap(arr[i], arr[j]);
		}
		std::swap(arr[l], arr[j]);
		return j;
	}
	void sort(std::vector<int>& arr, int l, int r) {
		if (l >= r)
			return;
		int p = part(arr, l, r);

		sort(arr, l, p - 1);
		sort(arr, p + 1, r);
	}
	void sort(std::vector<int>& arr)
	{
		sort(arr, 0, arr.size() - 1);
	}
	void run() {

		int sum = 0;
		std::vector<int> right;
		std::vector<int> left;

		std::ifstream inputStream("input2024-01.txt");
		if (!inputStream.is_open()) {
			std::cerr << "Failed to open file!\n";
		}
		int r, l;
		while (inputStream >> l >> r)
		{
			right.push_back(r);
			left.push_back(l);
		}

		int size = right.size();

		sort(right);
		sort(left);

		for (int i = 0; i< size; i++)
		{
			sum += std::abs(right[i] - left[i]);
		}
		std::cout << sum << '\n';

		//task 2
		int r_it = 0;
		int l_it = 0;
		int score = 0;
		while (r_it < size && l_it < size)
		{
			r = right[r_it];
			l = left[l_it];
			if (r < l)
			{
				r_it++;
				continue;
			}
			if (l < r)
			{
				l_it++;
				continue;
			}
			if (l == r)
			{
				std::cout << "pair found" << '\n';
			}
			int r_cnt = 1;
			int l_cnt = 1;
			r_it++;
			l_it++;
			while (r_it < size && right[r_it] == r)
			{
				r_cnt++;
				r_it++;
			}
			while (l_it < size && left[l_it] == l) {
				l_cnt++;
				l_it++;
			}
			score += r_cnt * l_cnt * r;
		}

		std::cout << score << '\n';
	}
};