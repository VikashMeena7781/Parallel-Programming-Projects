#include<bits/stdc++.h>
using namespace std;

// Function to read a matrix from a binary file
void readMatrix(const string &path, vector<double> &matrix, size_t rows, size_t cols) {
    ifstream input(path, ios::binary);
    if (!input) {
        cerr << "Error opening file: " << path << endl;
        exit(1);
    }
    matrix.resize(rows * cols);
    input.read(reinterpret_cast<char *>(matrix.data()), rows * cols * sizeof(double));
    input.close();
}

// Function to write a matrix to a binary file
void writeMatrix(const string &path, const vector<double> &matrix, size_t rows, size_t cols) {
    ofstream output(path, ios::binary);
    if (!output) {
        cerr << "Error opening file: " << path << endl;
        exit(1);
    }
    output.write(reinterpret_cast<const char *>(matrix.data()), rows * cols * sizeof(double));
    output.close();
}

// Function for IJK loop order
void matrixMultiplyIJK(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t i = 0; i < A_rows; ++i) {
        for (size_t j = 0; j < B_cols; ++j) {
            for (size_t k = 0; k < A_cols; ++k) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

// Function for IKJ loop order
void matrixMultiplyIKJ(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t i = 0; i < A_rows; ++i) {
        for (size_t k = 0; k < A_cols; ++k) {
            for (size_t j = 0; j < B_cols; ++j) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

// Function for JIK loop order
void matrixMultiplyJIK(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t j = 0; j < B_cols; ++j) {
        for (size_t i = 0; i < A_rows; ++i) {
            for (size_t k = 0; k < A_cols; ++k) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

// Function for JKI loop order
void matrixMultiplyJKI(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t j = 0; j < B_cols; ++j) {
        for (size_t k = 0; k < A_cols; ++k) {
            for (size_t i = 0; i < A_rows; ++i) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

// Function for KIJ loop order
void matrixMultiplyKIJ(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t k = 0; k < A_cols; ++k) {
        for (size_t i = 0; i < A_rows; ++i) {
            for (size_t j = 0; j < B_cols; ++j) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

// Function for KJI loop order
void matrixMultiplyKJI(const vector<double> &A, const vector<double> &B, vector<double> &C,
                       size_t A_rows, size_t A_cols, size_t B_cols) {
    for (size_t k = 0; k < A_cols; ++k) {
        for (size_t j = 0; j < B_cols; ++j) {
            for (size_t i = 0; i < A_rows; ++i) {
                C[i * B_cols + j] += A[i * A_cols + k] * B[k * B_cols + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        cerr << "Usage: ./main <type> <mtx_A_rows> <mtx_A_cols> <mtx_B_cols> <input_path> <output_path>\n";
        return 1;
    }

    int type = stoi(argv[1]);
    size_t A_rows = stoul(argv[2]);
    size_t A_cols = stoul(argv[3]);
    size_t B_cols = stoul(argv[4]);
    string input_path = argv[5];
    string output_path = argv[6];

    // Read matrices
    vector<double> A, B, C(A_rows * B_cols, 0.0);
    readMatrix(input_path + "/mtx_A.bin", A, A_rows, A_cols);
    readMatrix(input_path + "/mtx_B.bin", B, A_cols, B_cols);

    // Call the appropriate function based on the type
    switch (type) {
        case 0:
            matrixMultiplyIJK(A, B, C, A_rows, A_cols, B_cols);
            break;
        case 1:
            matrixMultiplyIKJ(A, B, C, A_rows, A_cols, B_cols);
            break;
        case 2:
            matrixMultiplyJIK(A, B, C, A_rows, A_cols, B_cols);
            break;
        case 3:
            matrixMultiplyJKI(A, B, C, A_rows, A_cols, B_cols);
            break;
        case 4:
            matrixMultiplyKIJ(A, B, C, A_rows, A_cols, B_cols);
            break;
        case 5:
            matrixMultiplyKJI(A, B, C, A_rows, A_cols, B_cols);
            break;
        default:
            cerr << "Invalid type. Must be 0-5.\n";
            return 1;
    }

    // Write result matrix
    writeMatrix(output_path + "/mtx_C.bin", C, A_rows, B_cols);

    return 0;
}
