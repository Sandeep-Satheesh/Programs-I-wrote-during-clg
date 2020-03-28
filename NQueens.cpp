#include<iostream>
//#pragma GCC optimize ("Ofast,O3,fast-math,no-stack-protector,unroll-loops") //unnecessary optimizations, does not affect program logic...

using namespace std;

void printChessboard(int** chessboard, int& n) {
    //Prints 1 if there's a queen at that position, 0 otherwise.
    cout<<"\nChessboard (1 implies queen should be placed there): ";
    cout<<"\n\n\t\t+";
    cout.fill('-');
    cout.width(2*n + 1);
    cout<<"+\n";
    for (int i = 0; i < n; i++) {
        cout<<"\t\t|";
        for (int j = 0; j < n; j++)
            cout<<chessboard[i][j]<<' ';
        cout<<"\b|\n";
    }
    cout<<"\t\t+";
    cout.fill('-');
    cout.width(2*n + 1);
    cout<<"+\n";
}

int getStartColOrRow(int row, int col, int n, bool colIsRequired) {
    //This function helps to select the right row and col for starting the loop to check for diagonal presence of attacking piece (for the loop scanning from bottom left to top right.
    //Returns the starting row from top right to begin iteration.
    while (row < n - 1 && col > 0) {
        row++;
        col--;
    }
    if (colIsRequired) return col;
    else return row;
}

bool placedQueenIsUnsafe(int**& chessboard, int& insertedRow, int& insertedCol, int& n) {
    //checks if the currently placed queen is safe in the board.

    //check for attacking piece in same row
    for (int j = 0; j < n; j++)
            if (j != insertedCol && chessboard[insertedRow][j])
                return true;
    //check for attacking piece in same col
    for (int i = 0; i < n; i++)
        if (i != insertedRow && chessboard[i][insertedCol])
                return true;

    //check for attacking piece in same diagonal

    if (insertedRow == insertedCol) {
        //it's on the main diagonal
        for (int i = 0; i < n; i++)
            if (i != insertedRow && i != insertedCol && chessboard[i][i]) {
                return true;
            }
    }
    else if (insertedCol + insertedRow == n - 1) {
        //it's on the other diagonal
        for (int i = 0; i < n; i++)
            if (i != insertedRow && n - 1 - i != insertedCol && chessboard[i][n - i - 1])
                return true;
    }
   if (insertedRow > insertedCol) {
         //it's in the triangular region under the main diagonal

         //scanning in the direction of the main diagonal - from top left to bottom right
        for (int i = insertedRow - insertedCol, j = 0; j < n && i < n; i++, j++) //insertedRow - insertedCol gives the starting row no. of the diagonal corresponding to that element.
            if (i != insertedRow && j != insertedCol && chessboard[i][j])
                return true;

        //scanning in the direction of the other diagonal - from bottom left to top right
        for (int i = getStartColOrRow(insertedRow, insertedCol, n, false), j = getStartColOrRow(insertedRow,insertedCol, n, true); j < n && i >= 0; i--, j++) {
            if (i != insertedRow && j != insertedCol && chessboard[i][j])
                return true;
        }
    }
    else {
        //it's in the triangular region above the main diagonal

        //scanning in the direction of main diagonal - from top left to bottom right
        for (int j = insertedCol - insertedRow, i = 0; j < n && i < n; i++, j++) //insertedCol - insertedRow gives the starting col no. of the diagonal corresponding to that element.
            if (i != insertedRow && j != insertedCol && chessboard[i][j])
                return true;
        //scanning in the direction of the other diagonal - from bottom left to top right

        for (int i = getStartColOrRow(insertedRow, insertedCol, n, false), j = getStartColOrRow(insertedRow,insertedCol,n, true); i >= 0 && j < n; j++, i--)
            if (j != insertedCol && j != insertedCol && chessboard[i][j])
                return true;
    }
    //all conditions checked; no danger if this point has been reached.
    return false;
}

bool NQueens(int**& chessboard, int& n, int startCol = 0) {
    //main logic of the code. returns true if it was able to place all the queens in the right places. else false.

    //this is base condition to stop the recursion
    if (startCol >= n) return true; //We've already solved the whole problem!


    for (int i = 0; i < n; i++) {
        chessboard[i][startCol] = 1; //place a queen at that position and see if it's safe
        if (placedQueenIsUnsafe(chessboard, i, startCol, n))

            chessboard[i][startCol] = 0; //undo changes and go to next row

        else if (NQueens(chessboard, n, startCol+1)) //the place was safe, therefore check if we can place the next queen in the next col.
            return true; //that position was also safe; this goes on in a recursive fashion until we reach base case.

        else // not safe position; undo changes and continue...
            chessboard[i][startCol] = 0;
    }
    return false; //executed when finished checking all rows in for loop and still finding no correct placement; i.e. when we need to backtrack.
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
    //Call fn...
    NQueens(chessboard, n);

    printChessboard(chessboard, n);

    //Cleaning up...
    for (int i = 0; i < n; i++)
        delete[] chessboard[i];
    delete chessboard;
    return 0;
}
