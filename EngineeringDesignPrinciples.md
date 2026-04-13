# Problem Background
- **Objective:** Register two 2D images containing circles. 
- **Requirement:** The program will be **used more than 1 million times**. Therefore, the criteria for success should be balanced with computational throughput and minimal overhead. 
- **Solution:** The target transformation consists of Translation and Isotropic Scaling.

# Potential ITK Solutions
## A. Iterative Intensity-Based Registration
`itk::ImageRegistrationMethodv4`
- **Mechanism:** Uses a metric (e.g., Mean Squares), an optimizer (e.g., Gradient Descent), and an interpolator to iteratively align the moving image to the fixed image. 
- **Pros:** Highly versatile; the standard approach for complex medical image registration. 
- **Cons:** Extremely computationally expensive. Iterative resampling of the image grid will make processing 1,000,000 pairs prohibitively slow. 
- **Verdict:** Rejected due to severe performance bottlenecks for this simple and specific geometric problem.

## B. Image Moments Analysis
`itk::ImageMomentsCalculator`
- **Mechanism:** Calculates the geometric moments (zeroth, first, and second order) of the image intensities to analytically determine the center of gravity (centroid) and total mass (area).
- **Pros:**
  * **Non-iterative:** Computes parameters in a single pass ($O(N)$ complexity, where $N$ is the number of pixels).
  * **Analytical Solution:** Translation is the exact vector difference between centroids. Scale is the square root of the ratio of their zeroth moments (areas).
  * **Extreme Speed:** Microsecond-level execution time per image.
- **Cons:** Sensitive to heavy background noise or asymmetrical artifacts. However, in this assignment, we only focus on noise-free geometric circles on clean backgrounds. This vulnerability can be ignored.
- **Verdict:** Selected as the Optimal Solution.

