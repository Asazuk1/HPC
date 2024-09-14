import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 15})

fig, ax = plt.subplots(figsize=(15, 10))
x = [8, 16, 32, 64, 128, 256, 512, 1024, 2048]
gflops_naive  = [1.024000e+00,  
                 8.192000e-01,
                 8.977534e-01,  
                 9.395842e-01,  
                 8.841282e-01,  
                 8.407946e-01,  
                 6.094061e-01,  
                 3.858319e-01,  
                 1.259148e-01] 
gflops_cblas  = [1.084746e-03, 
                 2.730667e+00,
                 1.092267e+01,
                 1.638400e+01,
                 2.016492e+01, 
                 2.304563e+01, 
                 2.271796e+01, 
                 2.298913e+01, 
                 2.334012e+01] 
gflops_multithread = [4.785047e-03, 
                      3.640889e-02,
                      3.413333e-01, 
                      1.210827e+00, 
                      2.772177e+00, 
                      3.700312e+00, 
                      3.621975e+00, 
                      3.458601e+00, 
                      7.029853e-01 ]
gflops_openmp = [1.021039e-04, 
                 8.614090e-03,
                 2.093802e-01, 
                 1.735478e-01, 
                 2.385567e-01, 
                 2.905775e-01, 
                 2.615440e-01, 
                 2.821116e-01, 
                 2.693810e-01]

plt.plot(x, gflops_naive, label='naive', color='blue', linestyle='-', linewidth=3.0)
plt.plot(x, gflops_cblas, label='cblas', color='green', linestyle='-.', linewidth=3.0)
plt.plot(x, gflops_multithread, label='multithread', color='red', linestyle=':', linewidth=3.0)
plt.plot(x, gflops_openmp, label='openmp', color='purple', linestyle=':', linewidth=3.0)

plt.title('dgemm')
plt.xlabel('matrix size')
plt.ylabel('gflops')
plt.legend()

plt.savefig('result.png')
plt.show()