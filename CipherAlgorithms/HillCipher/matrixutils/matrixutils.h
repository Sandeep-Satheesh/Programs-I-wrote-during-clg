#include <bits/stdc++.h>
using namespace std;

#ifndef MATRIXUTILS_H_INCLUDED
    #define MATRIXUTILS_H_INCLUDED
#endif // MATRIXUTILS_H_INCLUDED

vector<vector<int>> matrixMultiply(vector<vector<int>>& m1, vector<vector<int>>& m2);
int findDeterminant(vector<vector<int>>& m);
vector<vector<int>> getCofactor(vector<vector<int>> mat, int p, int q, int n);
vector<vector<int>> transposeMatrix(vector<vector<int>> mat);
