import re
import csv
import os

# Read performance data from the output file
with open("output.txt", "r") as file:
    perf_data = file.read()

# Extract values using regular expressions
permutation_match = re.search(r"'./main (\d+)", perf_data)
row_dimension_match = re.search(r"'./main \d+ (\d+)", perf_data)
user_time_match = re.search(r"(\d+\.\d+) seconds user", perf_data)
cache_misses_match = re.search(r"(\d+) +cache-misses:u", perf_data)
cache_references_match = re.search(r"(\d+) +cache-references:u", perf_data)
instructions_per_cycle_match = re.search(r"(\d+\.\d+) +insn per cycle", perf_data)

# Parse the extracted data
permutation_number = int(permutation_match.group(1)) if permutation_match else None
row_dimension = int(row_dimension_match.group(1)) if row_dimension_match else None
user_time = float(user_time_match.group(1)) if user_time_match else None
cache_misses = int(cache_misses_match.group(1)) if cache_misses_match else None
cache_references = int(cache_references_match.group(1)) if cache_references_match else None
instructions_per_cycle = float(instructions_per_cycle_match.group(1)) if instructions_per_cycle_match else None

# Calculate the cache hit rate
if cache_references and cache_misses is not None:
    cache_hit_rate = (1 - (cache_misses / cache_references)) * 100
else:
    cache_hit_rate = None

# Placeholder values for missing columns
user_time_in_mm = 0  # User time in matrix multiplication
percentage_user_time_mm = 0  # % of user time taken by matrix multiplication

# Data to append
row_data = [
    permutation_number,
    row_dimension,
    user_time,
    user_time_in_mm,
    percentage_user_time_mm,
    cache_misses,
    cache_hit_rate,
    instructions_per_cycle,
]

# File path
csv_file_path = "data.csv"

# Write or append to the CSV file
file_exists = os.path.exists(csv_file_path)

with open(csv_file_path, mode="a", newline="") as file:
    writer = csv.writer(file)
    # Write the header only if the file does not already exist
    if not file_exists:
        writer.writerow([
            "Permutation Number",
            "Row Dimension",
            "Total User Time (seconds)",
            "User Time in Matrix Multiplication",
            "% of User Time Taken by Matrix Multiplication",
            "Total Cache Misses",
            "Cache Hit Rate (%)",
            "Instructions per Cycle",
        ])
    # Append the row data
    writer.writerow(row_data)

print("Data appended to data.csv successfully!")
