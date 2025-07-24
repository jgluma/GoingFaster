./clean_results.sh -f
directories=("2-B-Naive-Instrumentalizado" "3-Unrolling" "4-AVX2" "5-OpenMP" "6-AVX2UnrollingOpenMP" "7-Blocking" "8-AVX2BlockUnrollingOpenMP")
# Launch the tests for each directory
for dir in "${directories[@]}"; do
  ./launch_tests.sh $dir
done