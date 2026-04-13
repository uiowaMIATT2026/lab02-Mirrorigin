# 1. Problem Background
- **Objective:** Register two 2D images containing circles. 
- **Requirement:** The program will be **used more than 1 million times**. Therefore, the criteria for success should be balanced with computational throughput and minimal overhead. 
- **Solution:** The target transformation consists of Translation and Isotropic Scaling.

# 2. Potential ITK Solutions
## 1) Iterative Intensity-Based Registration
`itk::ImageRegistrationMethodv4`
- **Mechanism:** Uses a metric (e.g., Mean Squares), an optimizer (e.g., Gradient Descent), and an interpolator to iteratively align the moving image to the fixed image. 
- **Pros:** Highly versatile; the standard approach for complex medical image registration. 
- **Cons:** Extremely computationally expensive. Iterative resampling of the image grid will make processing 1,000,000 pairs prohibitively slow. 
- **Verdict:** Rejected due to severe performance bottlenecks for this simple and specific geometric problem.

## 2) Image Moments Analysis
`itk::ImageMomentsCalculator`
- **Mechanism:** Calculates the geometric moments (zeroth, first, and second order) of the image intensities to analytically determine the center of gravity (centroid) and total mass (area).
- **Pros:**
  * **Non-iterative:** Computes parameters in a single pass ($O(N)$ complexity, where $N$ is the number of pixels).
  * **Analytical Solution:** Translation is the exact vector difference between centroids. Scale is the square root of the ratio of their zeroth moments (areas).
  * **Extreme Speed:** Microsecond-level execution time per image.
- **Cons:** Sensitive to heavy background noise or asymmetrical artifacts. However, in this assignment, we only focus on noise-free geometric circles on clean backgrounds. This vulnerability can be ignored.
- **Verdict:** Selected as the Optimal Solution.

# 3. Implementation Strategy
Given the 1-million-run constraint, `itk::ImageMomentsCalculator` is the only reasonable choice that satisfies both accuracy and extreme throughput requirements.

## 3.1 Mathematical Formulation

- **Translation ($T$):** Calculated as the difference between the physical Center of Gravity ($c$) of the fixed and moving images.
  
  $$T = c_{fixed} - c_{moving}$$
- **Scale ($S$):** Calculated using the Total Mass ($M_{00}$, representing the area for uniform intensity circles).
  
  $$S = \sqrt{\frac{M_{00, fixed}}{M_{00, moving}}}$$

## 3.2 Software Optimization
To further minimize overhead for the 1-million-run requirement:

- **C++ Execution:** The solution is implemented in raw C++ using ITK. For $1,000,000$ executions, raw C++ delivers high speed and efficient memory management.
- **Removal of CLI Frameworks:** Frameworks like `SlicerExecutionModel` (SEM) were intentionally excluded to eliminate XML parsing and CLI initialization overhead, significantly reducing application startup time. 
- **Compiler Optimization:** The build system (CMake) is configured to mandate a Release build to leverage maximum compiler optimizations (e.g., `-O3`), ensuring peak performance during the millions of executions.

# 4 Data Generation and Validation

- **Data Generation:** We implement a synthetic data generator, where images are generated with a defined physical spacing ($1.0\, \text{mm/pixel}$) to ensure the mm units in this problem statement are correctly mapped to the voxel grid.
- **Algorithm Validation:**  The success of the algorithm can be validated by its quantitative capability to inversely recover the exact transformation parameters (Translation $T = [150, 150]$ mm, Isotropic Scale $S = 2.0$) directly from the discrete pixel grid. The benchmark for success can be defined as achieving zero or sub-pixel error (i.e., a parameter deviation strictly less than the physical spacing of a single pixel).