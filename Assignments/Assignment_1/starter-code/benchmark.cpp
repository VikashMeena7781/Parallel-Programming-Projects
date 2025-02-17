#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <chrono>
#include <fstream>
#include <omp.h>
#include "check.h"


using namespace std;
using namespace std::chrono;

// Function to run experiments and record timings
void run_benchmark(int n, int m, int k, const vector<int>& b_values, const string& output_file) {
    ofstream csv(output_file);
    if (!csv.is_open()) {
        cerr << "Failed to open " << output_file << endl;
        return;
    }

    // Write CSV header
    csv << "b,2 cores,4 cores,8 cores,16 cores,32 cores,40 cores" << endl;

    for (int b : b_values) {
        csv << b;
        // for (int cores : core_counts) {
        // cout<<"Running for b = "<<b<<" and cores = "<<2<<endl;
        // omp_set_num_threads(cores); // Set number of OpenMP threads

        // Warm-up run to avoid cold-start bias
        map<pair<int, int>, vector<vector<int>>> warmup_blocks = generate_matrix(n, m, b);
        matmul(warmup_blocks, n, m, k);
        
        map<pair<int, int>, vector<vector<int>>> blocks = generate_matrix(n, m, b);
        // Actual timed run
        auto start = high_resolution_clock::now();
        matmul(blocks, n, m, k);
        auto end = high_resolution_clock::now();

        // Calculate duration in milliseconds
        double duration = duration_cast<milliseconds>(end - start).count();
        csv << "," << duration;
        cout<<"Time "<<duration<<endl;
        // }
        csv << endl;
    }
    csv.close();
}

int main() {
    // Fixed parameters
    int n = 100000; // Matrix size
    int m = 16;     // Block size
    int k = 2;      // Exponent

    // Varying parameters
    // vector<int> core_counts = {2, 4, 8,16,32,40}; // Cores to test
    vector<int> b_values = {64, 256, 1024, 4096};     // Non-zero blocks (2^6, 2^8, 2^10, 2^12)

    // Run experiments and save to data.csv
    run_benchmark(n, m, k, b_values, "data.csv");

    return 0;
}