#include<iostream>
#include<stdlib.h>
#pragma GCC optimize (Ofast,O3,fast-math,no-stack-protector,unroll-loops)

using namespace std;

void printChessboard(int** chessboard, int& n) {
    cout<<"\n\t\t";
    cout.fill('-');
    cout.width(2*(n+1)); cout<<"\n\t\t";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout<<chessboard[i][j]<<' ';
        cout<<"\n\t\t";
        cout.fill('-');
    cout.width(2*(n+1)); cout<<"\n\t\t";
    }
}

bool anyQueenIsUnderAttack(int** chessboard, int& n) {
    //check for same row

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = j+1; k < n; k++)
                if (chessboard[i][k] == 1 && chessboard[i][j] == 1)
                    return true;

    //check for same col
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = j+1; k < n; k++)
                if (chessboard[j][i] == 1 && chessboard[k][i] == 1)
                    return true;

    //check for same diagonal
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++)
            if (chessboard[i][i] == 1 && chessboard[j][j] == 1)
                return true;
    return false;
}

bool NQueens(int **chessboard, int& n, int startCol) {

    if (startCol > n) return true;


    for (int i = 0; i < n; i++) {
        chessboard[i][startCol] = 1;
        if (anyQueenIsUnderAttack(chessboard, n))
            chessboard[i][startCol] = 0;
        else if (NQueens(chessboard, n, startCol+1))
            return true;
        else
            chessboard[i][startCol] = 0;
    }
    return false;
}

int main() {
    cout<<"\nEnter dimensions of chessboard (n x n): ";
    int n; cin>>n;
    if (n < 4) { cout<<"\nNot possible to solve the problem for any n < 4!\n\n"; return 0; }
    int **chessboard = new int*[n];
    for (int i = 0; i < n; i++) {
        chessboard[i] = new int[n];
        for (int j = 0; j < n; j++)
            chessboard[i][j] = 0;
    }
    NQueens(chessboard, n, 0);

    printChessboard(chessboard, n);

    for (int i = 0; i < n; i++)
        delete[] chessboard[i];
    delete chessboard;
    return 0;
}
