import os
import subprocess
import re

def perf_data():
    # Define the combinations of permutation numbers and row dimensions
    permutations = [5]
    row_dimensions = [5000]

    # Paths to the executable and scripts
    main_executable = "./main"
    input_path = "./input_path/"
    output_path = "./output_path/"
    temp_script = "temp.py"

    # Ensure paths exist
    if not os.path.exists(input_path):
        os.makedirs(input_path)
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    # Iterate through each combination of permutation and row dimension
    for permutation in permutations:
        for row_dimension in row_dimensions:
            # Construct the perf command
            print(f"Running script for permutation {permutation} and for row size of {row_dimension}")
            perf_command = [
                "perf", "stat", "-e", "task-clock,cycles,instructions,cache-references,cache-misses", "-d",
                main_executable, str(permutation), str(row_dimension), str(row_dimension), str(row_dimension),
                input_path, output_path
            ]
            
            # Run the perf command and redirect both stdout and stderr to output.txt
            with open("output.txt", "w") as output_file:
                subprocess.run(perf_command, stderr=subprocess.STDOUT, stdout=output_file)
            
            # Run the temp.py script to process the output.txt file and append data to data.csv
            subprocess.run(["python3", temp_script])
            

    print("All combinations processed successfully!")


def gprof_data():
    # Path to the gprof report
    file_path = "gprof_report.txt"
    # Initialize a dictionary to store the extracted data
    function_times = {}
    # Define the pattern to extract function names and % time values
    pattern = re.compile(r"\[\d+\]\s+(\d+\.\d+)\s+\d+\.\d+\s+\d+\.\d+\s+\d+\s+([^\[\]]+)")
    # Read the gprof report
    with open(file_path, "r") as file:
        for line in file:
            match = pattern.search(line)
            if match:
                time_percentage = match.group(1)
                function_name = match.group(2).strip()
                function_times[function_name] = time_percentage
                print(function_name, time_percentage)

    # Filter specific functions of interest
    functions_of_interest = ["matrixMultiplyIJK", "readMatrix", "writeMatrix"]
    filtered_results = {fn: function_times.get(fn, "Not Found") for fn in functions_of_interest}

    # Output the results
    for function, time in filtered_results.items():
        print(f"{function} --> {time}")

if __name__ == "__main__":
    # Run the gprof_data function
    gprof_data()