import sys
import time
import numpy as np
from typing import List, Union

# include sgemm from sgemm.py
import sgemm

# Example usage and testing
def test_sgemm():
    """Test function to verify the implementations work correctly"""
    n = 512
    # Get size from arguments
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
    print(f"Description:\t {sgemm.sgemm_desc} on matrices of size {n} x {n}\n")

    # Generate random numpy matrices
    np.random.seed(0)  # For reproducibility
    A = np.random.rand(n, n).astype(np.float32)
    B = np.random.rand(n, n).astype(np.float32)
    C = np.zeros((n, n), dtype=np.float32)
    
    # Measure time
    start_time = time.time()
    #C += np.dot(A, B)
    sgemm.square_sgemm(A, B, C)
    elapsed_time = time.time() - start_time
    print(f"Time {elapsed_time:.6f} s\n")
    
if __name__ == "__main__":
    test_sgemm()