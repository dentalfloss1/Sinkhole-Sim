// Sinkhole_c.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <algorithm>  

using namespace std;

#define M 32
#define N 32
#define size M*N

int main()
{
	int **baselayer = (int **)malloc(N * sizeof(int));
	for(int i=0;i<N;i++) baselayer[i] = (int *)malloc(M * sizeof(int));
	
	int *holearea = (int *)malloc(size * sizeof(int));
	int *holelength = (int *)malloc(size * sizeof(int));
	int *holewidth = (int *)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++) {
		holearea[i] = 0;
		holewidth[i] = 0;
		holelength[i] = 0;
	}
	double p = 0.4;
	//float **islandlabels = (float **)malloc(N * sizeof(float));
	//for (int i = 0; i < N; i++) islandlabels[i] = (float *)malloc(M * sizeof(float));
	/**/
	random_device rd; //non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister
	binomial_distribution<> dist(1, p); // distribute results between 1 and 6 inclusive.
	

	for (int i = 1; i < N-1; i++) {
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
		baselayer[N-1][i] = 0;
			
	}
	for (int i = 0; i < N; i++) {
		baselayer[i][0] = 0;
		baselayer[i][M-1] = 0;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) cout << baselayer[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	int counter = 0;
	/*for (int i = 1; i < N; i++) {
		for (int j = 1; j < M; j++) {
			if (0 != baselayer[i][j]) {
				counter++;
				baselayer[i][j] += counter - 1;
				if (0 != baselayer[i][j - 1]) baselayer[i][j] = baselayer[i][j - 1];
			}
		}
	}
	for (int j = 1; j < M; j++) {
		for (int i = 1; i < N; i++) {
			if ((0 != baselayer[i][j]) && (0 != baselayer[i-1][j])) {
				baselayer[i][j] = baselayer[i - 1][j];
			}
		}
	}
	for (int i = 1; i < N; i++) {
		int minval = 0;
		int startlabel = M;
		int stoplabel = 1;
		for (int j = 1; j < M; j++) {
			if ((0 != baselayer[i][j]) && (0 != baselayer[i][j + 1])) {
				minval = min(baselayer[i][j + 1], baselayer[i][j]);
				cout << minval << endl;
				startlabel = min(startlabel, j);
				stoplabel = max(stoplabel, j);
			}
			if ((0 != baselayer[i][j-1]) && (0 != baselayer[i][j]) && (0 == baselayer[i][j + 1])){
				for (int k = startlabel; k < stoplabel + 1; k++) baselayer[i][k] = minval;
			}
			else minval = 0;
		}
	}*/

	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			if (0 != baselayer[i][j]) {
				counter++;
				cout << counter << endl;
				cout << counter - 1 << endl;
				cout << baselayer[i][j] + counter - 1 << endl;
				baselayer[i][j] += counter - 1;
				if (0 != baselayer[i - 1][j]) {
					//cout << j << endl;
					//This is a place where two threads can split work
					for (int k = j; k < M; k++){
						if ((0 != baselayer[i][k]) && (baselayer[i][k] != baselayer[i - 1][j])) {
							baselayer[i][k] = baselayer[i - 1][j];
							if (0 != baselayer[i - 1][k]) {
								for (int l = i - 1; l > 0; l--) {
									if (0 != baselayer[l][k]) {
										baselayer[l][k] = baselayer[i - 1][j];
									}
									else break;
								}
							}
						}
						else break;
					}
					for (int k = j; k > 0; k--) {
						if ((0 != baselayer[i][k]) && (baselayer[i][k] != baselayer[i - 1][j])) {
							baselayer[i][k] = baselayer[i - 1][j];
							if (0 != baselayer[i - 1][k]) {
								for (int l = i - 1; l > 0; l--) {
									if (0 != baselayer[l][k]) {
										baselayer[l][k] = baselayer[i - 1][j];
									}
									else break;
								}
							}
						}
						else break;
					}
				}
				if (0 != baselayer[i][j - 1]) baselayer[i][j] = baselayer[i][j - 1];
				
			}
		}
	}
	int lastholewidth = 0;
	int largestwidth = 0;
	for (int i = 1; i < N-1; i++) {
		for (int j = 1; j < M-1; j++) {
			holearea[baselayer[i][j]]++;
			if ((0 == baselayer[i][j]) && (0 != baselayer[i][j-1])) {
				lastholewidth = max(baselayer[i][j-1] - lastholewidth, lastholewidth);
				holewidth[baselayer[i][j-1]] = ;
			}
			holewidth[baselayer[i][j]]++;

		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) cout << baselayer[i][j] << " ";
		cout << endl;
	}
				
	for (int i = 0; i < size; i++) if (0 != holearea[i]) cout << i << "," << holearea[i] << endl;
	cout << "widths" << endl;
	for (int i = 0; i < size; i++) if (0 != holewidth[i]) cout << i << "," << holewidth[i] << endl;
    cout << "Hello World!\n";
	free(baselayer);
	free(holearea);
	free(holelength);
	free(holewidth);
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
