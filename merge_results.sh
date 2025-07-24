# Read every output.csv file in the current directory and its subdirectories
# and merge them into a single output.csv file in the current directory
# Append the name of the directory to the first column of each row
output_file="output.csv"
# Remove the output file if it already exists to avoid appending to old data
[ -f "$output_file" ] && rm "$output_file"
# Write the header to the output file
echo "Version,Size,Instructions,CPU Cycles,Cache References,Cache Misses,Branch Instructions,Branch Misses,Time" > "$output_file"
# Find all output.csv files
find . -type f -name "output.csv" | while read -r file; do
    # Get the directory name (just the last part of the path)
    dir_name=$(basename "$(dirname "$file")")
    # Read the content of the output.csv file, skipping the header
    tail -n +2 "$file" | while read -r line; do
        # Append the directory name to the first column of each row
        echo "$dir_name,$line"
    done >> "$output_file"
done