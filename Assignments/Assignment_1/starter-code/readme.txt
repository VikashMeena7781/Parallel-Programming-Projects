1. Preprocessing Matrix2 by Row
    Optimization: Group blocks in matrix2 by their row index (p) into a map matrix2_rows.
    Impact:
        Reduces the complexity of finding relevant block pairs from O(N*M) to O(N+M), where N is the number of blocks in matrix1 and 
        M is the number of blocks in matrix2. 

2.  IKJ Loop Order for Cache Locality
    Optimization: Use the IKJ loop order in the multiply_blocks method instead of the traditional IJK order.
    Impact:
        Improves cache locality by accessing elements of block1 and block2 in a more cache-friendly manner.
        Reduces cache misses by reusing cached values of block1[i][k] across multiple iterations of the inner loop.
        Provides a performance boost for large block sizes (m) due to better utilization of the CPU cache.

3. Efficient Task Granularity
    Optimization: Use fine-grained tasks for block multiplication and updates.
    Impact:
        Distributes the workload evenly across threads, improving load balancing.
        Maximizes parallel performance for large matrices.
    
4. Avoid Redundant Computations    
    Optimization: Skip block pairs where block1.col != block2.row.
    Impact:
        Avoids unnecessary multiplications by only processing relevant block pairs.
        Leverages sparsity to reduce computation time.    

5. Zero-Block Removal
    Optimization: Remove blocks that become entirely zero after multiplication.
    Impact:
        Reduces memory usage and avoids unnecessary computations in subsequent steps.
        Ensures the result matrix remains sparse.







