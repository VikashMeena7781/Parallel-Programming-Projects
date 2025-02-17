
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "check.h"

using namespace std;

// bool black_box() {
//     return true;
// }

vector<vector<int>> multiply_blocks(vector<vector<int>>& block1,
                                    vector<vector<int>>& block2, int m) {
    vector<vector<int>> product(m, vector<int>(m, 0));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < m; ++k) {
                product[i][j] += block1[i][k] * block2[k][j];
            }
        }
    }
    return product;
}

void removeMultiplesOf5(map<pair<int, int>, vector<vector<int>>>& matrixBlocks) {
    for (auto it = matrixBlocks.begin(); it != matrixBlocks.end(); ) {
        vector<vector<int>>& block = it->second;
        bool isBlockNonZero = false;

        for (auto& row : block) {
            for (auto& value : row) {
                if (value % 5 == 0) {
                    value = 0;
                }
                if (value != 0) {
                    isBlockNonZero = true;
                }
            }
        }

        if (!isBlockNonZero) {
            it = matrixBlocks.erase(it);
        } else {
            ++it;
        }
    }
}




bool is_square(map<pair<int, int>, vector<vector<int>>>& matrix1,
               map<pair<int, int>, vector<vector<int>>>& matrix2, int m) {

    removeMultiplesOf5(matrix1);
    map<pair<int, int>, vector<vector<int>>> squared_result;

    for (auto& [block_pos1, block1] : matrix1) {
        for (auto& [block_pos2, block2] : matrix1) {
            if (block_pos1.second == block_pos2.first) {
                vector<vector<int>> product = multiply_blocks(block1, block2, m);
                if (squared_result.find({block_pos1.first, block_pos2.second}) != squared_result.end()) {
                    vector<vector<int>>& existing_block = squared_result[{block_pos1.first, block_pos2.second}];
                    for (int i = 0; i < m; ++i) {
                        for (int j = 0; j < m; ++j) {
                            existing_block[i][j] += product[i][j];
                        }
                    }
                } else {
                    squared_result[{block_pos1.first, block_pos2.second}] = product;
                }
            }
        }
    }
    

    if (squared_result.size() != matrix2.size()) {
        return false;
    }

    for (auto& [block_pos, block] : squared_result) {
        if (matrix2.find(block_pos) == matrix2.end()) {
            return false;
        }

        vector<vector<int>>& block2 = matrix2.at(block_pos);
        if (block != block2) {
            return false;
        }
    }

    return true;
}

map<pair<int, int>, vector<vector<int>>> multiply_matrix(map<pair<int, int>, vector<vector<int>>>& matrix1,
                                                         map<pair<int, int>, vector<vector<int>>>& matrix2, int m) {

    map<pair<int, int>, vector<vector<int>>> result;

    for (auto& [block_pos1, block1] : matrix1) {
        for (auto& [block_pos2, block2] : matrix2) {
            if (block_pos1.second == block_pos2.first) {
                vector<vector<int>> product = multiply_blocks(block1, block2, m);
                if (result.find({block_pos1.first, block_pos2.second}) != result.end()) {
                    vector<vector<int>>& existing_block = result[{block_pos1.first, block_pos2.second}];
                    for (int i = 0; i < m; ++i) {
                        for (int j = 0; j < m; ++j) {
                            existing_block[i][j] += product[i][j];
                        }
                    }
                } else {
                    result[{block_pos1.first, block_pos2.second}] = product;
                }
            }
        }
    }
    // cout<<"Result size: "<<result.size()<<endl;
    return result;
}

void print_matrix_map(map<pair<int, int>, vector<vector<int>>>& matrix_map) {
    for (auto& entry : matrix_map) {
        cout << "Block (" << entry.first.first << ", " << entry.first.second << "):\n";
        for (auto& row : entry.second) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}


bool is_kth_power(map<pair<int, int>, vector<vector<int>>>& matrix1,
                  map<pair<int, int>, vector<vector<int>>>& matrix2, int m, int k) {
    
    removeMultiplesOf5(matrix1);                
    map<pair<int, int>, vector<vector<int>>> result = matrix1;
    // cout<<"Matrix1 size: "<<matrix1.size()<<endl;
    for (int power = 1; power < k; ++power) {
        result = multiply_matrix(result, matrix1, m);
    }
    // cout<<"computed by checkscript\n";
    // print_matrix_map(result);
    // cout<<"Matrix2 Size: "<<matrix2.size()<<endl;
    if (result.size() != matrix2.size()) {
        cout<<"Size mismatched: "<<result.size()<<"  "<<matrix2.size()<<endl;
        return false;
    }

    for (auto& [block_pos, block] : result) {
        if (matrix2.find(block_pos) == matrix2.end()) {
            cout<<"Block not found\n";
            return false;
        }

        vector<vector<int>>& block2 = matrix2.at(block_pos);
        if (block != block2) {
            cout<<"Block mismatched\n";
            return false;
        }
    }

    return true;
}

bool has_non_zero_element(vector<vector<int>>& block) {
    for (auto& row : block)
        for (int val : row)
            if (val != 0)
                return true;
    return false;
}

int count_non_zero_blocks(map<pair<int, int>, vector<vector<int>>>& blocks) {
    int non_zero_count = 0;

    for (auto& entry : blocks) {
        vector<vector<int>>& block = entry.second;
        // cout<<"Block: "<<entry.first.first<<"  "<<entry.first.second<<endl;
        if (has_non_zero_element(block))
            non_zero_count++;
    }

    return non_zero_count;
}

// void print_matrix_map(map<pair<int, int>, vector<vector<int>>>& matrix_map) {
//     for (auto& entry : matrix_map) {
//         cout << "Block (" << entry.first.first << ", " << entry.first.second << "):\n";
//         for (auto& row : entry.second) {
//             for (int val : row) {
//                 cout << val << " ";
//             }
//             cout << endl;
//         }
//         cout << endl;
//     }
// }

vector<vector<int>> matMult(vector<vector<int>>& mat1, vector<vector<int>>& mat2){
    int n = mat1.size();
    vector<vector<int>> result(n, vector<int>(n, 0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}


int main() {
    int n = 9;
    int m = 3;
    int b = 3;
    int k = 2;

    srand(time(0));
    //map<pair<int, int>, vector<vector<int>>> blocks = generate_matrix(n, m, b);
    map<pair<int, int>, vector<vector<int>>> blocks = {
        {{0, 0}, {{1, 0, 2}, {0, 3, 0}, {2, 0, 4}}},
        {{0, 2}, {{0, 0, 0}, {0, 3, 1}, {0, 0, 5}}},
        {{2, 0}, {{0, 0, 0}, {0, 3, 0}, {0, 1, 5}}}
    };
    
    cout<<"Matrix generated\n";
    // print_matrix_map(blocks);
    // cout<<"hii"<<endl;
    if(count_non_zero_blocks(blocks)==blocks.size() && blocks.size()>=b)
        cout<<"You have generated the matrix correctly\n";
    else
        cout<<"You have NOT generated the matrix correctly\n";

    map<pair<int, int>, vector<vector<int>>> original_blocks = blocks;
    
    vector<float> s = matmul(blocks, n, m, k);
    cout<<"Matrix multiplication done\n";
    // print_matrix_map(blocks);


    
    // make the matrix out the blocks
    vector<vector<int>> mat(n, vector<int>(n, 0));
    for (const auto& block : original_blocks) {
        int i = block.first.first;  // Row index of the block
        int j = block.first.second; // Column index of the block
        
        // Copy block into the full matrix
        for (int x = 0; x < m; ++x) {
            for (int y = 0; y < m; ++y) {
                mat[i * m + x][j * m + y] = block.second[x][y]; // Place the block correctly
            }
        }
    }
    // cout<<"Matrix built\n";
    // print the matrix
    // for (const auto& row : mat) {
    //     for (int val : row) {
    //         cout << val << " ";
    //     }
    //     cout << endl;
    // }
    // cout<<"Matrix computed manually\n";
    // vector<vector<int>> result = mat;
    // for(int i = 1; i < k; i++){
    //     result = matMult(result, mat);
    // }
    // // print the result
    // for(auto &row : result){
    //     for(auto &val : row){
    //         cout<<val<<" ";
    //     }
    //     cout<<endl;
    // }

    // bool res = is_square(original_blocks, blocks, m);
    bool res = is_kth_power(original_blocks, blocks, m, k);

    if(res)
        cout<<"k = " <<k<<" " <<"Your function computed the kth power correctly\n";
    else
        cout<<"Your function did NOT compute the kth power correctly\n";
    cout << "Size of S = " << s.size()<<endl;
    // print the row statistics
    for(int i = 0; i < s.size(); i++){
        cout<<s[i]<<" \n";
    }

    return 0;
}

