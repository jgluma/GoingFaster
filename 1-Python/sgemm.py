import numpy as np

# Description of the implementation
sgemm_desc = "A naive Python version"

# Function to perform square matrix multiplication using NumPy
def square_sgemm(A: np.ndarray, B: np.ndarray, C: np.ndarray) -> None:
    """
    This routine performs a sgemm operation using NumPy arrays
    C := C + A * B
    where A, B, and C are square matrices.
    
    Args:
        A: Matrix A as a 2D NumPy array
        B: Matrix B as a 2D NumPy array
        C: Matrix C as a 2D NumPy array (modified in-place)
    """
    n = A.shape[0]

    # Naive implementation (triple loop)
    for i in range(n):
        for j in range(n):
            for k in range(n):
                C[i, j] += A[i, k] * B[k, j]