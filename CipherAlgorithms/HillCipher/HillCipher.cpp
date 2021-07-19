#include "cipherutils/matrixutils.h"
#include "cipherutils/matrixutils.cpp"

using namespace std;

#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

int modularMI(int a, int m) {
    for (int x = 1; x < m; x++) {
        if (((a%m)*(x%m))%m == 1) return x;
    }
    return -1;
}

vector<vector<int>> getKeyMatrix(string key) {
    int c = sqrt(key.length());
    vector<vector<int>> K(c, vector<int> (c));
    for (int i = 0; i < key.length(); i++) {
        key[i] -= 'A';
        K[i/c][i%c] = key[i];
    }
    return K;
}

vector<vector<vector<int>>> splitToMatrices(string key, int splitFactor) {
    vector<vector<vector<int>>> ans;
    for (int i = 0; i < key.length();) {
        int ct = 0;
        vector<vector<int>> keyElement;
        while (i < key.length() && ct < splitFactor) {
                key[i] -= 'A';
                vector<int> v(1, key[i]);
                keyElement.push_back(v);
                i++;
                ct++;
        }
        ans.push_back(keyElement);
    }
    return ans;
}

vector<vector<int>> findInverseMatrix(vector<vector<int>> m) {
    int determinant = findDeterminant(m, m.size());
    if (determinant == 0) {
        cout<<"\nERROR: Message cannot be decrypted! Inverse of key matrix does not exist.\n\n";
        return {};
    }
    //find cofactor matrix
    vector<vector<int>> cofactorMatrix(m.size(), vector<int>(m.size(), 0));
    int sign = 1;
    for (int i = 0; i < m.size(); i++)
        for (int j = 0; j < m.size(); j++) {
            vector<vector<int>> cofactor = getCofactor(m, i, j, m.size());
            cofactorMatrix[i][j] = sign * findDeterminant(cofactor, m.size() - 1);
            sign = -sign;
        }
    vector<vector<int>> adjointMatrix = transposeMatrix(cofactorMatrix);
    //if any element in adjoint matrix < 0, keep adding 26 to it until it becomes just >= 0.

    for (int i = 0; i < adjointMatrix.size(); i++) {
        for (int j = 0; j < adjointMatrix.size(); j++) {
            while (adjointMatrix[i][j] < 0) adjointMatrix[i][j] += 26;
        }
    }
    //find multiplicative inverse of determinant under mod 26
    int a = modularMI(determinant, 26);
    if (a <= 0) {
        cout<<"\nERROR: Message cannot be decrypted! Modular multiplicative inverse value does not exist for key matrix.\n\n";
        return {};
    }
    //inverse matrix => (a)*adj(A). mod by 26 to keep it within range of A-Z

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m.size(); j++) {
            adjointMatrix[i][j] *= a;
            adjointMatrix[i][j] %= 26;
        }
    }

    return adjointMatrix;
}

string encrypt(string key, string plaintext) {

    vector<vector<int>> K = getKeyMatrix(key);
    vector<vector<vector<int>>> P = splitToMatrices(plaintext, sqrt(key.length()));

    vector<vector<vector<int>>> encMat;
    for (int i = 0; i < P.size(); i++)
        encMat.push_back(matrixMultiply(K, P[i]));

    string encryptedMsg = "";

    for (int i = 0; i < encMat.size(); i++) {
        for (int j = 0; j < encMat[i].size(); j++) {
            encMat[i][j][0] %= 26;
            encMat[i][j][0] += 'A';
            encryptedMsg += (char)(encMat[i][j][0]);
        }
    }
    return encryptedMsg;
}

string decrypt(string key, string ciphertext) {
    vector<vector<int>> K = getKeyMatrix(key);
    vector<vector<vector<int>>> P = splitToMatrices(ciphertext, sqrt(key.length()));

    vector<vector<int>> Kinverse = findInverseMatrix(K);
    if (Kinverse.size() == 0) return "";

    string plaintextMsg = "";
    for (int i = 0; i < P.size(); i++) {
        vector<vector<int>> decodedElement = matrixMultiply(Kinverse, P[i]);

        for (int j = 0; j < decodedElement.size(); j++) {
            decodedElement[j][0] %= 26;
            plaintextMsg += (char)('A' + decodedElement[j][0]);
        }
    }
    return plaintextMsg;
}

int main() {
    int ch;
    string key;
    string text;

    cout<<"\t\tHILL CIPHER IMPLEMENTATION";
    cout<<"\n------------------------------------------------------\n\n";

    do {
        cout<<"\tMenu";
        cout<<"\n-----------------------\n";
        cout<<"\n1 - Set key"<<(key.size() > 0 ? (string(" (current key: '") + key + "')") : "")
            <<"\n2 - Encrypt"
            <<"\n3 - Decrypt"
            <<"\n4 - Exit"
            <<"\n\nMake your choice: ";

        cin>>ch;
        if (ch == 4) break;

        if (ch == 1) {
            cout<<"\nEnter key: ";
            cin>>key;

            int c = sqrt(key.length());
            if (c*c != key.length()) {
                cout<<"\nERROR: Key length is not a perfect square!";
                key = "";
                continue;
            }

            for (int i = 0; i < key.length(); i++) {
                if (!isalpha(key[i])) {
                    cout<<"\nERROR: Key must be only a single word, composed of alphabets!";
                    key = "";
                    break;
                }
                else key[i] = toupper(key[i]);
            }
            cout<<"\n\nKey stored successfully.\n\n";
        }
        else if (ch == 2) {
            if (key.length() == 0) { cout<<"\nERROR: Please set the key first!\n\n"; continue; }
            cout<<"\nEnter plaintext: ";
            cin>>ws;
            getline(cin, text);
            string s = encrypt(key, text);
            if (s.length() > 0) cout<<"\nEncrypted msg: "<<s<<"\n\n";
        }
        else if (ch == 3) {
            if (key.length() == 0) { cout<<"\nERROR: Please set the key first!\n\n"; continue; }
            cout<<"\nEnter ciphertext: ";
            cin>>text;
            string plaintextMsg = decrypt(key, text);
            if (plaintextMsg.length() > 0) cout<<"\nPlaintext msg: "<<plaintextMsg<<"\n\n";
        }
        else cout<<"\n\nInvalid input! Please retry!\n\n";

    } while (true);
    cout<<"\nExiting...\n\n";
    return 0;
}
