#include "matrixutils.h"

vector<vector<int>> getCofactor(vector<vector<int>> mat, int p, int q, int n) {
    int i = 0, j = 0;
    vector<vector<int>> temp(n - 1, vector<int> (n - 1, 0));
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            //  Copying into temporary matrix only those
            //  element which are not in given row and
            //  column
            if (row != p && col != q) {
                temp[i][j++] = mat[row][col];

                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
    return temp;
}

int findDeterminant(vector<vector<int>> mat, int sz) {
    int det = 0;
    if (sz == 1) return mat[0][0];
    else if (sz == 2) {
        return mat[1][1]*mat[0][0] - mat[0][1]*mat[1][0];
    }
    int sign = 1;
    for (int f = 0; f < sz; f++) {
        // Getting Cofactor of mat[0][f]
        vector<vector<int>> cofactor = getCofactor(mat, 0, f, sz);
        det += sign * mat[0][f] * findDeterminant(cofactor, sz - 1);

        // terms are to be added with alternate sign
        sign = -sign;
    }

    return det;
}
vector<vector<int>> matrixMultiply(vector<vector<int>>& m1, vector<vector<int>>& m2) {

    int a = m1.size(), b = m1[0].size(), c = m2.size(), d = m2[0].size();

    if (!(a && b && c && d) || b != c) {
            string msg = "MATRIXMULTIPLY: Cannot multiply ";
            msg += to_string(a);
            msg += 'x';
            msg += to_string(b);
            msg += " and ";
            msg += to_string(c);
            msg += 'x';
            msg += to_string(d);
            throw runtime_error(msg);
            return {};
    }

    vector<vector<int>> ans (a, vector<int>(d));

    for (int i = 0; i < a; i++) {
        for (int j = 0; j < d; j++) {
            for (int k = 0; k < c; k++) {
                ans[i][j] += (m1[i][k] * m2[k][j]);
            }
        }
    }
    return ans;
}

vector<vector<int>> transposeMatrix(vector<vector<int>> mat) {
    int m = mat.size(), n = mat[0].size();
    vector<vector<int>> transpose(n, vector<int>(m));

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            transpose[j][i] = mat[i][j];
        }
    }
    return transpose;
}
