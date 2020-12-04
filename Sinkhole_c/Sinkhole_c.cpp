// Sinkhole_c.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <map>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>  
#include <vector>
#include <math.h>
#include <string>  
#include <omp.h>

using namespace std;

#define M 1026
#define N 1026
#define size M*N



void initializebase(vector<vector<int>> &baselayer, double p);
void printlayer(vector<vector<int>> &layer);
void labelgroups(vector<vector<int>> &baselayer);
void groupcols(vector<vector<int>> &baselayer);
void grouprows(vector<vector<int>> &baselayer);
void computearea(vector<vector<int>> &baselayer, map<int, int> &area);
void computewidth(vector<vector<int>> &baselayer, map<int, int> &width, map<int, int> &nextwidth);
void computelength(vector<vector<int>> &baselayer, map<int, int> &length, map<int, int> &nextlength);
double dp(double p, double gamma, int A);
void updatelowerprob(vector<vector<double>> &baseprob, vector<vector<int>> &baselayer, map<int, double> &dpmap, double p);
void updateupperprob(vector<vector<double>> &upperprob, vector<vector<int>> &baselayer, map<int, int> &area, map<int, int> &length, map<int, int> &width, double alpha);

int main()
{
	vector<vector<int>> baselayer(N);
	for (int i = 0; i < N; i++) baselayer[i] = vector<int>(M);
	vector<vector<int>> upperlayer(N);
	for (int i = 0; i < N; i++) upperlayer[i] = vector<int>(M);
	vector<vector<double>> baseprob(N);
	for (int i = 0; i < N; i++) baseprob[i] = vector<double>(M);
	#pragma omp parallel for
	for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) baseprob[i][j] = 0;
	vector<vector<double>> upperprob(N);
	for (int i = 0; i < N; i++) upperprob[i] = vector<double>(M);
	#pragma omp parallel for
	for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) upperprob[i][j] = 0;



	double p = 0.01;
	double gamma = 0.06;
	double alpha = 0.03;
	int iteration = 15;
	initializebase(baselayer, p);
	//cout << "LOWER" << endl;
	//printlayer(baselayer);
/*	ofstream myfile;
	myfile.open("baselayer0.csv");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M-1; j++) myfile << baselayer[i][j] << ",";
		myfile << baselayer[i][M-1] << endl;
	}
	myfile.close();*/
	for (int i = 0; i < iteration; i++) {
		map<int, int> area;
		area[0] = 0;
		map<int, int> width;
		width[0] = 1; //Defining special params for zero assists with calculations later (no 0/0 and no more ifs)
		map<int, int> nextwidth;
		map<int, int> length;
		length[0] = 1;
		map<int, int> nextlength;
		map<int, double> dpmap;
        //        cout << "LABEL GROUPS" << endl;
		labelgroups(baselayer);
	//	cout << "GROUP COLS" << endl;
		groupcols(baselayer);
	//	cout << "GROUP ROWS" << endl;
		grouprows(baselayer);
	//	cout << "GROUP COLS" << endl;
		groupcols(baselayer);
	//	cout << "COMPUTE AREA" << endl;
		computearea(baselayer, area);
	//	cout << "COMPUTE WIDTH" << endl;
		computewidth(baselayer, width, nextwidth);
	//	cout << "COMPUTE LENGTH" << endl;
		computelength(baselayer, length, nextlength);

	//	printlayer(baselayer);


		for (auto elem : area)
		{
			dpmap[elem.first] = dp(p, gamma, elem.second);
		}

	/*	cout << "area" << endl;
		for (auto elem : area)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		dpmap[elem.first] = dp(p, gamma, elem.second);
		}

		cout << "width" << endl;
		for (auto elem : width)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		}

		cout << "length" << endl;
		for (auto elem : length)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		}

		cout << "dp" << endl;
		for (auto elem : dpmap)
		{
			std::cout << elem.first << " " << elem.second << "\n";
		}
*/
		updatelowerprob(baseprob, baselayer, dpmap, p);

/*		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) cout << baseprob[i][j] << " ";
			cout << endl;
		}
		cout << endl;
*/
		updateupperprob(upperprob, baselayer, area, length, width, alpha);
/*		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) cout << upperprob[i][j] << " ";
			cout << endl;
		}*/
		cout << endl;
		random_device rd; //non-deterministic generator
		mt19937 gen(rd());  // to seed mersenne twister

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				binomial_distribution<> dist(1, upperprob[i][j]); // distribute results between 1 and 6 inclusive.
				if (0 == upperlayer[i][j]) upperlayer[i][j] = dist(gen);
                                else upperlayer[i][j] = 1;
			}
		}

		/*myfile.open("upperlayer"+to_string(i)+".csv");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M-1; j++) myfile << upperlayer[i][j] << ",";
			myfile << upperlayer[i][M-1] << endl;
		}
		myfile.close();*/

/*		cout << "UPPER" << endl;
		for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) cout << upperlayer[i][j] << " ";
				cout << endl;
			}
*/
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				binomial_distribution<> dist(1, baseprob[i][j]); // distribute results between 1 and 6 inclusive.
				if (0 == baselayer[i][j]) baselayer[i][j] = dist(gen);
				else baselayer[i][j] = 1;
			}
		}
//		cout << "LOWER" << endl;
//		printlayer(baselayer);
	/*	myfile.open("baselayer" + to_string(i+1) + ".csv");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M-1; j++) myfile << baselayer[i][j] << ",";
			myfile << baselayer[i][M-1] << endl;
		}
		myfile.close();*/
	}


  //  cout << "Hello World!\n";
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
	random_device rd; //non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister
	binomial_distribution<> dist(1, p); // distribute results between 1 and 6 inclusive.

	#pragma omp parallel for
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) baselayer[i][j] = dist(gen); //baselayer[i][j] = 0;    // pass the generator to the distribution.
	}
	/*for (int i = 1; i < 20; i++) baselayer[i][2] = 1;
	for (int i = 1; i < 20; i++) baselayer[i][4] = 1;
	baselayer[5][1] = 1;
	baselayer[5][3] = 1;
	baselayer[5][5] = 1;
	baselayer[7][5] = 1;
	baselayer[7][6] = 1;
	baselayer[7][7] = 1;
	baselayer[6][7] = 1;
	baselayer[5][7] = 1;*/
	#pragma omp parallel for
	for (int i = 0; i < M; i++) {
		baselayer[0][i] = 0;
		baselayer[N - 1][i] = 0;

	}
	#pragma omp parallel for
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
	#pragma omp parallel for
	for (int j = 1; j < M-1; j++) {
		for (int i = 1; i < N-1; i++) {
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
	#pragma omp parallel for
	for (int i = 1; i < N-1; i++) {
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

void computearea(vector<vector<int>> &baselayer, map<int, int> &area) {
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			if ((area.find(baselayer[i][j]) == area.end()) && (0 != baselayer[i][j])) area[baselayer[i][j]] = 0;
			if (0 != baselayer[i][j]) area[baselayer[i][j]]++;
		}
	}
}


void computewidth(vector<vector<int>> &baselayer, map<int, int> &width, map<int, int> &nextwidth) {
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

}


void computelength(vector<vector<int>> &baselayer, map<int, int> &length, map<int, int> &nextlength) {
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
}

double dp(double p, double gamma, int A) {
	double dpmax = (1.0 - p) / 4.0;
	return dpmax * (1 - exp(-gamma * (double)A));
}

void updatelowerprob(vector<vector<double>> &baseprob, vector<vector<int>> &baselayer, map<int, double> &dpmap, double p) {
	#pragma omp parallel for
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			baseprob[i][j] = p + dpmap[baselayer[i - 1][j]] + dpmap[baselayer[i + 1][j]] + dpmap[baselayer[i][j - 1]] + dpmap[baselayer[i][j + 1]];
		}
	}
}

void updateupperprob(vector<vector<double>> &upperprob, vector<vector<int>> &baselayer, map<int, int> &area, map<int, int> &length, map<int, int> &width, double alpha) {
	#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			double r = ((double)area[baselayer[i][j]]) / ((double)length[baselayer[i][j]] + (double)width[baselayer[i][j]]);
			upperprob[i][j] = 1 - exp(-alpha * r);
		}
	}
}



