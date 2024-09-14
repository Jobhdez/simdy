import numpy as np
import time

matrix1 = np.random.rand(1024, 1024)
matrix2 = np.random.rand(1024, 1024)

start_time = time.time()

result = matrix1 + matrix2

end_time = time.time()

time_taken = end_time - start_time
print(f"Time taken for matrix addition: {time_taken:.6f} seconds")
