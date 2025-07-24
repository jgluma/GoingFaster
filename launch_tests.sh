# Check if directory argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <directory>"
    echo "Example: $0 3-Unrolling"
    exit 1
fi

# Get the target directory from command line argument
target_dir="$1"

# Different unrolling factors
UNROLL_VALUES=1
# Different block sizes
BLOCKSIZE_VALUES=1
# Different number of threads
NTHREADS_VALUES=1
# Check if arguments DOUNROLL, DOBLOCKING and/or DOTHREADS are provided
if [ $# -ge 2 ]; then
  # Loop through all arguments starting from the second one
  for arg in "${@:2}"; do
    case "$arg" in
      "DOUNROLL")
        UNROLL_VALUES=(2 4 8 16 32)
        echo "Enabling unrolling tests with values: ${UNROLL_VALUES[*]}"
        ;;
      "DOBLOCKING")
        BLOCKSIZE_VALUES=(4 8 16 32)
        echo "Enabling blocking tests with values: ${BLOCKSIZE_VALUES[*]}"
        ;;
      "DOTHREADS")
        NTHREADS_VALUES=(1 2 4 8)
        echo "Enabling threading tests with values: ${NTHREADS_VALUES[*]}"
        ;;
      *)
        echo "Warning: Unknown argument '$arg' ignored"
        echo "Valid arguments: DOUNROLL, DOBLOCKING, DOTHREADS"
        ;;
    esac
  done
fi

# Enter script directory first to get the correct path
script_dir="$(dirname "$0")"
echo "Script directory: $script_dir"

# Change to the target directory relative to script directory
full_path="$script_dir/$target_dir"
echo "Changing directory to: $full_path"

if [ ! -d "$full_path" ]; then
    echo "Error: Directory '$full_path' does not exist"
    exit 1
fi

cd "$full_path" || exit 1
echo "Current directory: $(pwd)"

# Process the output files and store the results in a comma separated file output.csv 
# Rename the output.csv file to a random name to avoid overwriting if it already exists
if [ -f output.csv ]; then
    mv output.csv "output_$(date +%s).csv"
fi
# Print the header
echo "Unrolling,Blocking,Nthreads,Size,Instructions,CPU Cycles,Cache References,Cache Misses,Branch Instructions,Branch Misses,Time" > output.csv

# Create a directory to store the output files if it doesn't exist
output_dir="output_files"
mkdir -p "$output_dir"

# Run the executable with different size values
size_values=(128 256 512 1024 2048 4096)

# Loop through each combination of unrolling factor, block size, and number of threads
for unroll in "${UNROLL_VALUES[@]}"; do
  for blocksize in "${BLOCKSIZE_VALUES[@]}"; do
    for nthreads in "${NTHREADS_VALUES[@]}"; do
      # Set the environment variables for unrolling factor, block size, and number of threads
      compilation_flags="UNROLL=$unroll BLOCKSIZE=$blocksize NTHREADS=$nthreads"
      make ${compilation_flags[@]}
      # Check if the benchmark executable was created successfully
      if [ ! -f benchmark ]; then
        echo "Error: 'benchmark' executable not found. Make sure the build was successful."
        exit 1
      fi

      # Loop through each size value and execute the benchmark 10 times
      for size in "${size_values[@]}"; do
        # Initialize variables to accumulate results
        total_instructions=0
        total_cycles=0
        total_cache_references=0
        total_cache_misses=0
        total_branch_instructions=0
        total_branch_misses=0
        total_time=0
        for i in {1..10}; do 
          # Clear variables for each iteration
          instructions=0
          cycles=0
          cache_references=0
          cache_misses=0
          branch_instructions=0
          branch_misses=0
          time=0
          echo -ne "Running benchmark with unroll $unroll, blocksize $blocksize, nthreads $nthreads, size $size, iteration $i\r"
          # Save the output to a file in the output directory
          temp_output_file="$output_dir/output_size_${size}_iteration_${i}_unroll_${unroll}_blocksize_${blocksize}_nthreads_${nthreads}.txt"
          # Remove the file if it already exists to avoid appending to old data
          [ -f "$temp_output_file" ] && rm "$temp_output_file"
          # Run the benchmark with the current size and iteration
          # The output will include the number of instructions, cycles, cache references, cache misses,
          # branch instructions, branch misses, and time taken for the execution
          2>&1  ./benchmark "$size" "-p" > "$temp_output_file"
          instructions=$(grep "Instructions:" "$temp_output_file" | awk '{print $2}' | tr -d ',' | grep -o '[0-9]*')
          instructions=${instructions:-0}
          cycles=$(grep "CPU Cycles:" "$temp_output_file" | awk '{print $3}' | tr -d ',' | grep -o '[0-9]*')
          cycles=${cycles:-0}
          cache_references=$(grep "Cache References:" "$temp_output_file" | awk '{print $3}' | tr -d ',' | grep -o '[0-9]*')
          cache_references=${cache_references:-0}
          cache_misses=$(grep "Cache Misses:" "$temp_output_file" | awk '{print $3}' | tr -d ',' | grep -o '[0-9]*')
          cache_misses=${cache_misses:-0}
          branch_instructions=$(grep "Branch Instructions:" "$temp_output_file" | awk '{print $3}' | tr -d ',' | grep -o '[0-9]*')
          branch_instructions=${branch_instructions:-0}
          branch_misses=$(grep "Branch Misses:" "$temp_output_file" | awk '{print $3}' | tr -d ',' | grep -o '[0-9]*')
          branch_misses=${branch_misses:-0}
          time=$(grep "Time" "$temp_output_file" | awk '{print $2}' | tr -d ',' | grep -o '[0-9.]*')
          time=${time:-0}
          # Sum the values across all iterations for this size
          total_instructions=$(echo "$total_instructions + $instructions" | bc)
          total_cycles=$(echo "$total_cycles + $cycles" | bc)
          total_cache_references=$(echo "$total_cache_references + $cache_references" | bc)
          total_cache_misses=$(echo "$total_cache_misses + $cache_misses" | bc)
          total_branch_instructions=$(echo "$total_branch_instructions + $branch_instructions" | bc)
          total_branch_misses=$(echo "$total_branch_misses + $branch_misses" | bc)
          total_time=$(echo "$total_time + $time" | bc)
        done
        # Calculate averages for this size
        avg_instructions=$(echo "scale=2; $total_instructions / 10" | bc)
        avg_cycles=$(echo "scale=2; $total_cycles / 10" | bc)
        avg_cache_references=$(echo "scale=2; $total_cache_references / 10" | bc)
        avg_cache_misses=$(echo "scale=2; $total_cache_misses / 10" | bc)
        avg_branch_instructions=$(echo "scale=2; $total_branch_instructions / 10" | bc)
        avg_branch_misses=$(echo "scale=2; $total_branch_misses / 10" | bc)
        avg_time=$(echo "scale=4; $total_time / 10" | bc)
        # Print the averages
        # clear the contents of the current line
        echo -ne "                                                                                           \r"
        echo "Average for size $size:"
        echo "  Instructions: $avg_instructions"
        echo "  CPU Cycles: $avg_cycles"
        echo "  Cache References: $avg_cache_references"
        echo "  Cache Misses: $avg_cache_misses"
        echo "  Branch Instructions: $avg_branch_instructions"
        echo "  Branch Misses: $avg_branch_misses"
        echo "  Time: $avg_time seconds"
        # Append the averages to the output.csv file
        echo "$unroll,$blocksize,$nthreads,$size,$avg_instructions,$avg_cycles,$avg_cache_references,$avg_cache_misses,$avg_branch_instructions,$avg_branch_misses,$avg_time" >> output.csv
      done
    done
  done
done

