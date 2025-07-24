# Find and remove all the files starting with "output_size_" in every directory
find . -type f -name "output_size_*" -exec rm -f {} +

# Execute make clean in all directories that have a Makefile
echo "Running make clean in directories with Makefile..."
find . -name "Makefile" -execdir make clean \; 2>/dev/null

# if -f argument is provided, then find and remove every csv files in every directory
if [ "$1" == "-f" ]; then
    find . -type f -name "*.csv" -exec rm -f {} +
fi
