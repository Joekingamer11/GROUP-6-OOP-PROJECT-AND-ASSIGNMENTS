# Module Documentation: `stats`

**Header File:** `include/cppviz/stats.hpp`  
**Implementation File:** `src/cppviz/stats.cpp`  
**Subsystem:** Data Management  

---

## 1. Overview & Architectural Role

The `stats` module delivers mathematical routines operating on numerical vectors (`std::vector<double>`). It implements descriptive statistics, Pearson correlation matrix calculations, and 1,000-iteration non-parametric bootstrap confidence intervals.

---

## 2. API Contract & Data Structures

```cpp
namespace cppviz {

struct ConfidenceInterval {
    double mean = 0.0;
    double ci_lower = 0.0;
    double ci_upper = 0.0;
};

double mean(const std::vector<double>& values);
double variance(const std::vector<double>& values); // Sample variance (n-1)
double stddev(const std::vector<double>& values);
double pearson_correlation(const std::vector<double>& x, const std::vector<double>& y);

ConfidenceInterval bootstrap_ci(const std::vector<double>& values, 
                                std::size_t iterations = 1000, 
                                double confidence_level = 0.95);

std::vector<std::vector<double>> correlation_matrix(const Dataset& data, const std::vector<std::string>& columns);

} // namespace cppviz
```

---

## 3. Implementation Details & Key Algorithms

1. **Pearson Correlation ($r$):**
   $$r = rac{\sum (x_i - ar{x})(y_i - ar{y})}{\sqrt{\sum (x_i - ar{x})^2 \sum (y_i - ar{y})^2}}$$
2. **Non-Parametric Bootstrap CI:**
   Resamples $n$ items with replacement from `values` using `std::mt19937` (seeded deterministically across 1,000 iterations). Calculates resampled means, sorts them, and extracts the 2.5th and 97.5th percentiles to form a robust 95% confidence interval error bar.

---

## 4. Exception Handling

- Throws `InvalidArgument` if vectors are empty or if vector lengths mismatch for pairwise correlation.

---

## 5. C++ Usage Example

```cpp
#include "cppviz/stats.hpp"
#include <iostream>

int main() {
    std::vector<double> data = {12.0, 15.0, 14.0, 18.0, 22.0, 19.0, 25.0};
    cppviz::ConfidenceInterval ci = cppviz::bootstrap_ci(data);
    std::cout << "Mean: " << ci.mean << " [95% CI: " << ci.ci_lower << " - " << ci.ci_upper << "]\n";
    return 0;
}
```
