import matplotlib.pyplot as plt
import pandas as pd

def plot_data_exp_1():
    # Data extracted from the provided text
    matrix_sizes = [1000, 2000, 3000, 4000, 5000]
    execution_times = {
        "IJK": [1.6002916046660782, 29.254149544998654, 143.1010167939991, 568.2760933740016, 1045.5129461823333],
        "IKJ": [0.5732458700003917, 5.8859813139933976, 15.375990872002149, 33.47598306800501, 68.80277922700043],
        "JIK": [2.2925807959982194, 29.279876359003538, 186.2012974560057, 546.0251190810013, 1127.8649548510002],
        "JKI": [10.936536894000003, 168.15026908800002, 487.43331384300006, 1102.356476293, 2552.4953077140003],
        "KIJ": [1.010482917000445, 8.761503962999996, 25.367330447999848, 57.3959844230003, 108.27737673999945],
        "KJI": [12.569664709999998, 194.286668647, 691.547274269, 1595.1385624240002, 3459.499020405001],  # Same as IKJ, adjust if different, this is wrong
    }

    # Plot the data
    plt.figure(figsize=(10, 6))
    for loop_order, times in execution_times.items():
        plt.plot(matrix_sizes, times, marker='o', label=f'{loop_order}')

    # Customize the plot
    plt.title('Execution Time vs Matrix Size (Loop Permutations)')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('Execution Time (seconds)')
    plt.yscale('log')  # Use log scale for better visualization
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title="Loop Permutations", loc='upper left')

    # Save the plot to a file
    plt.savefig('./Plotted_Data/execution_time_vs_matrix_size.png', dpi=300)
    # plt.show()

def plot_data_exp_2():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Extract data from the DataFrame
    matrix_sizes = sorted(df['Row Dimension'].unique())
    execution_times = {loop_order: [] for loop_order in loop_orders}

    for loop_order_num, loop_order in enumerate(loop_orders):
        for size in matrix_sizes:
            # Filter rows for the specific loop order and matrix size
            time = df[(df['Permutation Number'] == loop_order_num) & (df['Row Dimension'] == size)]['Total User Time (seconds)']
            execution_times[loop_order].append(time.iloc[0] if not time.empty else None)

    # Plot the data
    plt.figure(figsize=(10, 6))
    for loop_order, times in execution_times.items():
        plt.plot(matrix_sizes, times, marker='o', label=f'{loop_order}')

    # Customize the plot
    plt.title('Execution Time vs Matrix Size From Perf')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('Execution Time (seconds)')
    plt.yscale('log')  # Use log scale for better visualization
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title="Loop Permutations", loc='upper left')

    # Save the plot to a file
    plt.savefig('./Plotted_Data/execution_time_vs_matrix_size_from_perf.png', dpi=300)
    # plt.show()


def plot_data_exp_3():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Extract data for plotting
    matrix_sizes = sorted(df['Row Dimension'].unique())
    cache_hit_rates = {loop_order: [] for loop_order in loop_orders}

    for loop_order_num, loop_order in enumerate(loop_orders):
        for size in matrix_sizes:
            # Filter rows for the specific loop order and matrix size
            cache_hit_rate = df[
                (df['Permutation Number'] == loop_order_num) & (df['Row Dimension'] == size)
            ]['Cache Hit Rate (%)']
            cache_hit_rates[loop_order].append(cache_hit_rate.iloc[0] if not cache_hit_rate.empty else None)

    # Plot the data
    plt.figure(figsize=(10, 6))
    for loop_order, rates in cache_hit_rates.items():
        plt.plot(matrix_sizes, rates, marker='o', label=f'{loop_order}')

    # Customize the plot
    plt.title('Cache Hit Rate vs Matrix Size (Loop Permutations)')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('Cache Hit Rate (%)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title="Loop Permutations", loc='best')

    # Save the plot to a file
    plt.savefig('./Plotted_Data/cache_hit_rate_vs_matrix_size.png', dpi=300)
    # plt.show()

def plot_data_exp_4():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Initialize dictionaries to store averages
    avg_cache_hit_rates = {}
    avg_execution_times = {}

    # Compute averages for each loop permutation
    for loop_order_num, loop_order in enumerate(loop_orders):
        subset = df[df['Permutation Number'] == loop_order_num]
        avg_cache_hit_rates[loop_order] = subset['Cache Hit Rate (%)'].mean()
        avg_execution_times[loop_order] = subset['Total User Time (seconds)'].mean()

    # Determine the best loop permutations
    best_cache_hit_permutation = max(avg_cache_hit_rates, key=avg_cache_hit_rates.get)
    best_execution_time_permutation = min(avg_execution_times, key=avg_execution_times.get)

    # Report results
    print("Loop Permutations Analysis:")
    print(f"Best Cache Hit Rate: {best_cache_hit_permutation} ({avg_cache_hit_rates[best_cache_hit_permutation]:.2f}%)")
    print(f"Best Execution Time: {best_execution_time_permutation} ({avg_execution_times[best_execution_time_permutation]:.2f} seconds)")

    # Check if the best permutation depends on matrix size
    best_permutations_by_size = {}
    for size in sorted(df['Row Dimension'].unique()):
        subset = df[df['Row Dimension'] == size]
        best_cache_hit = subset.loc[subset['Cache Hit Rate (%)'].idxmax()]['Permutation Number']
        best_exec_time = subset.loc[subset['Total User Time (seconds)'].idxmin()]['Permutation Number']
        best_permutations_by_size[size] = {
            "Best Cache Hit": loop_orders[int(best_cache_hit)],
            "Best Execution Time": loop_orders[int(best_exec_time)],
        }

    print("\nBest Permutations by Matrix Size:")
    for size, results in best_permutations_by_size.items():
        print(f"Matrix Size {size}: {results}")

    # Discussion
    print("\nDiscussion:")
    print(
        "The best permutation depends on the interplay between cache efficiency and execution time. "
        "For smaller matrices, memory access patterns might not have as much of an impact on performance, "
        "whereas for larger matrices, cache misses can significantly degrade execution time. "
        "Thus, the best permutation may vary with matrix size due to differences in computational load and memory access patterns."
    )


def plot_data_exp_5():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Calculate the global average cache hit rate for each loop permutation
    avg_cache_hit_rate = (
        df.groupby('Permutation Number')['Cache Hit Rate (%)'].mean().reset_index()
    )

    # Find the loop permutation with the best (highest) average cache hit rate
    best_avg_cache_hit_row = avg_cache_hit_rate.loc[avg_cache_hit_rate['Cache Hit Rate (%)'].idxmax()]

    # Find the global best execution time across all matrix sizes
    global_best_exec_time = df.loc[df['Total User Time (seconds)'].idxmin()]

    # Print global best results
    print("Loop Permutations Analysis:")
    print(f"Best Cache Hit Rate (Averaged): {loop_orders[int(best_avg_cache_hit_row['Permutation Number'])]} "
          f"({best_avg_cache_hit_row['Cache Hit Rate (%)']:.2f}%)")
    print(f"Best Execution Time: {loop_orders[int(global_best_exec_time['Permutation Number'])]} "
          f"({global_best_exec_time['Total User Time (seconds)']:.2f} seconds)\n")

    # Dictionary to store the best results by matrix size
    best_permutations_by_size = {}

    # Iterate over each matrix size
    for size in sorted(df['Row Dimension'].unique()):
        subset = df[df['Row Dimension'] == size]

        # Find the best cache hit rate and execution time for the current matrix size
        best_cache_hit_row = subset.loc[subset['Cache Hit Rate (%)'].idxmax()]
        best_exec_time_row = subset.loc[subset['Total User Time (seconds)'].idxmin()]

        # Store the results for this matrix size
        best_permutations_by_size[size] = {
            "Best Cache Hit": f"{loop_orders[int(best_cache_hit_row['Permutation Number'])]} & "
                              f"{best_cache_hit_row['Cache Hit Rate (%)']:.2f}%",
            "Best Execution Time": f"{loop_orders[int(best_exec_time_row['Permutation Number'])]} & "
                                   f"{best_exec_time_row['Total User Time (seconds)']:.2f} sec"
        }

    # Print the results by matrix size
    print("Best Permutations by Matrix Size:")
    for size, results in best_permutations_by_size.items():
        print(f"Matrix Size {size}: {results}")




def modify_csv():
    # Read the CSV file
    df = pd.read_csv('data.csv')
    # Modify the data as needed, we need to get Total User Time (seconds) and % of User Time Taken by Matrix Multiplication
    # and then update the User Time in Matrix Multiplication by multiplying the Total User Time with the % of User Time Taken by Matrix Multiplication
    df["User Time in Matrix Multiplication"] = (
        df["Total User Time (seconds)"] * df["% of User Time Taken by Matrix Multiplication"] / 100
    )

    # Save the updated dataframe back to the CSV file
    df.to_csv('data_1.csv', index=False)
    print("CSV file updated successfully!")


def plot_data_exp_mm_vs_time():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Extract data from the DataFrame
    matrix_sizes = sorted(df['Row Dimension'].unique())
    mm_times = {loop_order: [] for loop_order in loop_orders}

    for loop_order_num, loop_order in enumerate(loop_orders):
        for size in matrix_sizes:
            # Filter rows for the specific loop order and matrix size
            time = df[(df['Permutation Number'] == loop_order_num) & (df['Row Dimension'] == size)]['User Time in Matrix Multiplication']
            mm_times[loop_order].append(time.iloc[0] if not time.empty else None)

    # Plot the data
    plt.figure(figsize=(10, 6))
    for loop_order, times in mm_times.items():
        plt.plot(matrix_sizes, times, marker='o', label=f'{loop_order}')

    # Customize the plot
    plt.title('Matrix Multiplication Time vs Matrix Size')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('Matrix Multiplication Time (seconds)')
    plt.yscale('log')  # Use log scale for better visualization
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title="Loop Permutations", loc='upper left')

    # Save the plot to a file
    plt.savefig('./Plotted_Data/mm_time_vs_matrix_size.png', dpi=300)
    # plt.show()


def plot_data_exp_cpi_vs_mt_size():
    # Read the CSV file
    csv_file = './data.csv'  # Replace with the actual path to your CSV file
    df = pd.read_csv(csv_file)

    # Map permutation numbers to loop order names
    loop_orders = ["IJK", "IKJ", "JIK", "JKI", "KIJ", "KJI"]

    # Extract data from the DataFrame
    matrix_sizes = sorted(df['Row Dimension'].unique())
    cpi_values = {loop_order: [] for loop_order in loop_orders}

    for loop_order_num, loop_order in enumerate(loop_orders):
        for size in matrix_sizes:
            # Filter rows for the specific loop order and matrix size
            cpi = df[(df['Permutation Number'] == loop_order_num) & (df['Row Dimension'] == size)]['Instructions per Cycle']
            cpi_values[loop_order].append(cpi.iloc[0] if not cpi.empty else None)

    # Plot the data
    plt.figure(figsize=(10, 6))
    for loop_order, cpi in cpi_values.items():
        plt.plot(matrix_sizes, cpi, marker='o', label=f'{loop_order}')

    # Customize the plot
    plt.title('Instruction per cycle vs Matrix Size')
    plt.xlabel('Matrix Size (N x N)')
    plt.ylabel('CPI')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend(title="Loop Permutations", loc='upper left')

    # Adjust Y-axis for CPI range
    plt.ylim(0, 3)  # Set Y-axis limits to range from 0 to 3
    plt.xticks(matrix_sizes)
    plt.yticks([0, 0.5, 1, 1.5, 2, 2.5, 3])  # Set appropriate ticks for the CPI range

    # Save the plot to a file
    plt.savefig('./Plotted_Data/cpi_vs_matrix_size.png', dpi=300)
    # plt.show()




if __name__ == "__main__":
    plot_data_exp_5()
    # plot_data_exp_3()
    # modify_csv()
    # plot_data_exp_mm_vs_time()  # Call the function to plot the data
    # plot_data_exp_cpi_vs_mt_size()
