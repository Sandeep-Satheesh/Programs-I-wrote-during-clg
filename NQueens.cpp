#include<iostream>
// #pragma GCC optimize (Ofast,O3,fast-math,no-stack-protector,unroll-loops) //unnecessary optimizations...

using namespace std;

void printChessboard(int** chessboard, int& n) {
    //Prints 1 if there's a queen at that position, 0 otherwise.
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
    //check if the board has any queen that is under attack by any other queen in the board.
    
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
    //main logic of the code. returns true if it was able to place all the queens in the right places. else false.
    
    //base condition...
    if (startCol > n) return true;


    for (int i = 0; i < n; i++) {
        chessboard[i][startCol] = 1; //place a queen at that position and see if it's safe
        if (anyQueenIsUnderAttack(chessboard, n))
            chessboard[i][startCol] = 0; //undo changes and go to next row
        else if (NQueens(chessboard, n, startCol+1)) //the place was safe, therefore check if we can place the next queen in the next col.
            return true; //that position was also safe; this goes on in a recursive fashion until we reach base case.
        else
            chessboard[i][startCol] = 0; // not safe position; undo changes...
    }
    return false; //executed when finished checking all rows in for loop and still finding no correct placement; need to backtrack.
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
