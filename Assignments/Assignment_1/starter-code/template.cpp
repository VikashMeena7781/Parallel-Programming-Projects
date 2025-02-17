#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <omp.h>
#include "check.h"
#include <set>
#include <chrono>

using namespace std;


map<pair<int, int>, vector<vector<int>>> generate_matrix(int n, int m, int b) {
    map<pair<int, int>, vector<vector<int>>> matrix_map;
    int block_count = n / m;
    set<pair<int, int>> selected_blocks;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist_i(0, block_count - 1);
    uniform_int_distribution<int> dist_j(0, block_count - 1);

    while (selected_blocks.size() < b) {
        int i = dist_i(gen);
        int j = dist_j(gen);
        selected_blocks.insert({i, j});
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (auto block_pos : selected_blocks) {
                #pragma omp task firstprivate(block_pos) shared(matrix_map) if(black_box())
                {
                    vector<vector<int>> block(m, vector<int>(m));
                    random_device rd_local;
                    mt19937 gen_local(rd_local());
                    uniform_int_distribution<int> val_dist(INT32_MIN, INT32_MAX);
                    bool has_non_zero = false;

                    for (int x = 0; x < m; ++x) {
                        for (int y = 0; y < m; ++y) {
                            block[x][y] = val_dist(gen_local);
                            if (block[x][y] != 0) {
                                has_non_zero = true;
                            }
                        }
                    }
                    if (!has_non_zero) {
                        uniform_int_distribution<int> pos_dist(0, m-1);
                        int x = pos_dist(gen_local);
                        int y = pos_dist(gen_local);
                        block[x][y] = 1;
                    }

                    #pragma omp critical
                    {
                        matrix_map[block_pos] = block;
                    }
                }
            }
        }
    }

    return matrix_map;
}


vector<vector<int>> multiply_blocks(const vector<vector<int>>& block1,
                                    const vector<vector<int>>& block2, int m) {
    vector<vector<int>> product(m, vector<int>(m, 0));
    
    // Optimized loop ordering for better cache locality (IKJ)
    for (int i = 0; i < m; ++i) {
        for (int k = 0; k < m; ++k) {
            for (int j = 0; j < m; ++j) {
                product[i][j] += block1[i][k] * block2[k][j];
            }
        }
    }
    return product;
}


void update_block(vector<vector<int>>& existing_block, vector<vector<int>>& product, int m){
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            existing_block[i][j] += product[i][j];
        }
    }
}


void remove_zero_block(map<pair<int, int>, vector<vector<int>>> &result){

    for (auto it = result.begin(); it != result.end(); ) {
        bool non_zero = false;
        for (const auto& row : it->second) {
            for (int val : row) if (val != 0) { non_zero = true; break; }
            if (non_zero) break;
        }
        if (!non_zero) it = result.erase(it);
        else ++it;
    }
}

map<pair<int, int>, vector<vector<int>>> mul_matrix(map<pair<int, int>, vector<vector<int>>>& matrix1,
                                                    map<pair<int, int>, vector<vector<int>>>& matrix2, int m) {
    map<pair<int, int>, vector<vector<int>>> result;

    // Preprocess matrix2: Group blocks by their row index (p)
    map<int, vector<pair<int, vector<vector<int>>>>> matrix2_rows;
    for (const auto& [block_pos, block] : matrix2) {
        int p = block_pos.first;  
        int j = block_pos.second; 
        matrix2_rows[p].emplace_back(j, block);
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (const auto& [block_pos1, block1] : matrix1) {
                
                int i1 = block_pos1.first; 
                int p = block_pos1.second; 

                auto matrix2_blocks_iter = matrix2_rows.find(p);
                if (matrix2_blocks_iter == matrix2_rows.end()) continue;

                for (const auto& [j, block2] : matrix2_blocks_iter->second) {
                    #pragma omp task shared(result) firstprivate(i1, j, block1, block2) if(black_box())
                    {
                        vector<vector<int>> product = multiply_blocks(block1, block2, m);
                        #pragma omp critical
                        {
                            auto result_pos = make_pair(i1, j);
                            if (result.find(result_pos) != result.end()) {
                                update_block(result[result_pos], product, m);
                            } else {
                                result[result_pos] = product;
                            }
                        }
                    }
                }
            }
        }
    }

    remove_zero_block(result);
    return result;
}

void preprocess_blocks(map<pair<int, int>, vector<vector<int>>>& blocks){
    // Pre-process blocks: remove multiples of 5 and remove zero blocks
    for (auto it = blocks.begin(); it != blocks.end(); ) {
        auto& block = it->second;
        bool has_non_zero = false;
        for (auto& row : block) {
            for (auto& val : row) {
                if (val % 5 == 0) {
                    val = 0;
                }
                if (val != 0) {
                    has_non_zero = true;
                }
            }
        }
        if (!has_non_zero) {
            it = blocks.erase(it); // Erase and update iterator
        } else {
            ++it;
        }
    }
}


vector<int> calculate_B(map<pair<int, int>, vector<vector<int>>> &result, int m, int n){
    map<int,int> row_elements_in_nonzero_blocks;
    for (const auto& entry : result) {
        int i = entry.first.first; 
        for(int x=0;x<m;x++){
            row_elements_in_nonzero_blocks[i*m+x]+=m;
        }
    }
    vector<int> B(n, 0);
    for (int i = 0; i < n; ++i) {
        B[i] = row_elements_in_nonzero_blocks[i]; 
    }
    return B;
}

vector<float> calculate_row_statistics(vector<int> &P, vector<int> &B, int n){
    vector<float> row_statistics(n,0);
    for (int i = 0; i < n; ++i) {
        if (B[i] != 0) {
            row_statistics[i] = static_cast<float>(P[i]) / B[i];
        } else {
            row_statistics[i] = 0.0f; // If Bi is zero, set S[i] to zero
        }
    }
    return row_statistics;
}


vector<float> matmul(map<pair<int, int>, vector<vector<int>>>& blocks, int n, int m, int k) {
    vector<float> row_statistics;

    preprocess_blocks(blocks);

    map<pair<int, int>, vector<vector<int>>> result = blocks;
    for (int power = 1; power < k; ++power) {
        result = mul_matrix(result, blocks, m); 
    }

    if (k == 2) {

        vector<int> B = calculate_B(result,m,n);

        vector<int> P(n, 0); 
        map<int, vector<pair<int, vector<vector<int>>>>> row_map; 
        for (const auto& entry : blocks) {
            int i = entry.first.first;  
            int p = entry.first.second; 
            row_map[i].emplace_back(p, entry.second);
        }

        #pragma omp parallel
        {
            #pragma omp single
            {
                for (const auto& row_entry : row_map) {
                    int i = row_entry.first;
                    for (const auto& block1_entry : row_entry.second) {
                        int p = block1_entry.first;
                        const auto& block1 = block1_entry.second;
                        if (row_map.find(p) == row_map.end()) continue;

                        for (const auto& block2_entry : row_map[p]) {
                            int j = block2_entry.first;
                            const auto& block2 = block2_entry.second;

                            #pragma omp task firstprivate(i, p, j, block1, block2) shared(P) if(black_box())
                            {
                                // Compute contributions to P (non-zero multiplications involving row i)
                                for (int x = 0; x < m; ++x) {
                                    for (int k = 0; k < m; ++k) {
                                        if (block1[x][k] == 0) continue;
                                        for (int y = 0; y < m; ++y) {
                                            if (block2[k][y] != 0) {
                                                int global_row = i * m + x;
                                                #pragma omp atomic
                                                P[global_row]++; // Increment Pi for the corresponding row i
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                #pragma omp taskwait // Ensure all tasks complete before proceeding
            }
        }

        row_statistics = calculate_row_statistics(P,B,n);
    }
    blocks = result;
    return row_statistics;
}


