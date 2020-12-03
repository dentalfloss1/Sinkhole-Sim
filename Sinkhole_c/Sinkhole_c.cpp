// Sinkhole_c.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <map>
#include <iostream>
#include <random>
#include <algorithm>  
#include <vector>


using namespace std;

#define M 32
#define N 32
#define size M*N



void initializebase(vector<vector<int>> &baselayer, double p);
void printlayer(vector<vector<int>> &layer);
void labelgroups(vector<vector<int>> &baselayer);
void groupcols(vector<vector<int>> &baselayer);
void grouprows(vector<vector<int>> &baselayer);

int main()
{
	vector<vector<int>> baselayer(N);
	for (int i = 0; i < N; i++) baselayer[i] = vector<int>(M);

	map<int, int> area;
	map<int, int> width;
	map<int, int> nextwidth;
	map<int, int> length;
	map<int, int> nextlength;
	double p = 0.4;

	initializebase(baselayer, p);
	printlayer(baselayer);

	labelgroups(baselayer);

	groupcols(baselayer);
	grouprows(baselayer);
	groupcols(baselayer);
	
	int lastholewidth = 0;
	int largestwidth = 0;
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			if (area.find(baselayer[i][j]) == area.end()) area[baselayer[i][j]] = 1;
			area[baselayer[i][j]]++;
		}
	}
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			if ((0 == baselayer[i][j - 1]) && (baselayer[i][j] != 0) && (0 == baselayer[i][j + 1])) {
				if (nextwidth.find(baselayer[i][j]) == nextwidth.end()) nextwidth[baselayer[i][j]] = 1;
				if (width.find(baselayer[i][j]) == width.end()) width[baselayer[i][j]] = 1;

			}
			else if ((0 != baselayer[i][j - 1]) && (0 == baselayer[i][j])) {
				width[baselayer[i][j - 1]] = max(width[baselayer[i][j - 1]], nextwidth[baselayer[i][j - 1]]);
				nextwidth[baselayer[i][j - 1]] = 0;
			}
			else if (baselayer[i][j] != 0) {
				if (nextwidth.find(baselayer[i][j]) == nextwidth.end()) nextwidth[baselayer[i][j]] = 0;
				if (width.find(baselayer[i][j]) == width.end()) width[baselayer[i][j]] = 0;
				nextwidth[baselayer[i][j]]++;
			}
			
		}
	}
	for (int j = 1; j < M - 1; j++) {
		for (int i = 1; i < N - 1; i++) {
			if ((0 == baselayer[i - 1][j]) && (baselayer[i][j] != 0) && (0 == baselayer[i + 1][j])) {
				if (nextlength.find(baselayer[i][j]) == nextlength.end()) nextlength[baselayer[i][j]] = 1;
				if (length.find(baselayer[i][j]) == length.end()) length[baselayer[i][j]] = 1;

			}
			else if ((0 != baselayer[i - 1][j]) && (0 == baselayer[i][j])) {
				length[baselayer[i - 1][j]] = max(length[baselayer[i - 1][j]], nextlength[baselayer[i - 1][j]]);
				nextlength[baselayer[i - 1][j]] = 0;
			}
			else if (baselayer[i][j] != 0) {
				if (nextlength.find(baselayer[i][j]) == nextlength.end()) nextlength[baselayer[i][j]] = 0;
				if (length.find(baselayer[i][j]) == length.end()) length[baselayer[i][j]] = 0;
				nextlength[baselayer[i][j]]++;
			}

		}
	}
	printlayer(baselayer);
				
	for (auto elem : area)
	{
		std::cout << elem.first << " " << elem.second << "\n";
	}
	for (auto elem : width)
	{
		std::cout << elem.first << " " << elem.second << "\n";
	}
	for (auto elem : length)
	{
		std::cout << elem.first << " " << elem.second << "\n";
	}
    cout << "Hello World!\n";
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


void initializebase(vector<vector<int>> &baselayer, double p) {
	//random_device rd; //non-deterministic generator
	//mt19937 gen(rd());  // to seed mersenne twister
	//binomial_distribution<> dist(1, p); // distribute results between 1 and 6 inclusive.


	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) baselayer[i][j] = 0;    //baselayer[i][j] = dist(gen);// pass the generator to the distribution.
	}
	for (int i = 1; i < 20; i++) baselayer[i][2] = 1;
	for (int i = 1; i < 20; i++) baselayer[i][4] = 1;
	baselayer[5][1] = 1;
	baselayer[5][3] = 1;
	baselayer[5][5] = 1;
	baselayer[7][5] = 1;
	baselayer[7][6] = 1;
	baselayer[7][7] = 1;
	baselayer[6][7] = 1;
	baselayer[5][7] = 1;
	for (int i = 0; i < M; i++) {
		baselayer[0][i] = 0;
		baselayer[N - 1][i] = 0;

	}
	for (int i = 0; i < N; i++) {
		baselayer[i][0] = 0;
		baselayer[i][M - 1] = 0;
	}
}

void printlayer(vector<vector<int>> &layer) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) cout << layer[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

void labelgroups(vector<vector<int>> &baselayer) {
	int counter = 0;
	for (int i = 1; i < N; i++) {
		for (int j = 1; j < M; j++) {
			if (0 != baselayer[i][j]) {
				counter++;
				baselayer[i][j] += counter - 1;
				if (0 != baselayer[i][j - 1]) baselayer[i][j] = baselayer[i][j - 1];
			}
		}
	}
}

void groupcols(vector<vector<int>> &baselayer) {
	for (int j = 1; j < M; j++) {
		for (int i = 1; i < N; i++) {
			if ((0 != baselayer[i][j]) && (0 != baselayer[i - 1][j])) {
				baselayer[i][j] = min(baselayer[i - 1][j], baselayer[i][j]);
			}
		}
		for (int i = N - 1; i > 0; i--) {
			if ((0 != baselayer[i][j]) && (0 != baselayer[i + 1][j])) {
				baselayer[i][j] = min(baselayer[i + 1][j], baselayer[i][j]);
			}
		}
	}
}

void grouprows(vector<vector<int>> &baselayer) {
	for (int i = 1; i < N; i++) {
		for (int j = 1; j < M; j++) {
			if ((0 != baselayer[i][j]) && (0 != baselayer[i][j + 1]) && (baselayer[i][j + 1] < baselayer[i][j])) {
				for (int k = 0; k < M; k++) if (baselayer[i][k] == baselayer[i][j]) baselayer[i][k] = baselayer[i][j + 1];
			}
			if ((0 != baselayer[i][j]) && (0 != baselayer[i][j - 1]) && (baselayer[i][j - 1] < baselayer[i][j])) {
				for (int k = 0; k < M; k++) if (baselayer[i][k] == baselayer[i][j]) baselayer[i][k] = baselayer[i][j - 1];
			}
		}
	}
}



