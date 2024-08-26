#!/bin/bash

# Read commands from the command line
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 compression_algo input_file"
    exit 1
fi

compression_algo="$1"
input_file="$2"
total_runtime=0

# Run the compression algorithm three times
for i in {1..3}; do
    # Record the start time
    start_time=$(date +%s.%N)

    # Run the compression algorithm
    ./$compression_algo "$input_file"

    # Calculate the elapsed time
    end_time=$(date +%s.%N)
    runtime=$(echo "$end_time - $start_time" | bc)

    echo "Run $i took $runtime seconds"

    # Add to total runtime
    total_runtime=$(echo "$total_runtime + $runtime" | bc)
done

# Calculate the average runtime
average_runtime=$(echo "$total_runtime / 3" | bc)
echo "Compression took $runtime seconds"
