# k-means++ Clustering

A high-performance implementation of the k-means++ clustering algorithm in C with a Python interface. This project combines C's computational efficiency with Python's ease of use for fast clustering on large datasets.

## Overview

This project implements the **k-means++ initialization algorithm** combined with standard k-means clustering. K-means++ improves upon basic k-means by using a smarter centroid initialization strategy, which often results in better clustering outcomes and faster convergence.

The implementation features:
- **C implementation** (`kmeansmodule.c`) for fast k-means++ initialization and clustering computation
- **Python interface** (`kmeanspp.py`) that handles data processing, validation, and I/O
- **Bonus analysis tool** (`bonus.py`) for finding optimal cluster numbers using the elbow method

## Features

- ✅ k-means++ intelligent centroid initialization
- ✅ Fast C-based clustering algorithm
- ✅ CSV data input support
- ✅ Configurable iterations and epsilon (convergence threshold)
- ✅ Elbow method analysis for optimal cluster selection
- ✅ Input validation for parameters

## Requirements

- Python 3.x
- pandas
- numpy
- matplotlib (for bonus visualization)
- scikit-learn (for bonus.py)
- C compiler (gcc/clang) for building the extension module

## Installation

1. Clone the repository:
```bash
git clone https://github.com/NitzanZacharia/SoftwareProj-2.git
cd SoftwareProj-2
```

2. Build the C extension:
```bash
python setup.py build_ext --inplace
```

This compiles `kmeansmodule.c` and creates the `mykmeanspp` Python module.

## Usage

### Basic K-means++ Clustering

```bash
python kmeanspp.py <k> [max_iter] <eps> <input_file1> <input_file2>
```

**Parameters:**
- `k`: Number of clusters (must be integer, 2 ≤ k < n)
- `max_iter`: Maximum number of iterations (optional, default=300; must be 2-999)
- `eps`: Convergence threshold epsilon (must be ≥ 0)
- `input_file1`: First CSV file (contains IDs and features)
- `input_file2`: Second CSV file (contains IDs and features)

**Example:**
```bash
python kmeanspp.py 3 100 0.001 data1.csv data2.csv
```

**Output:**
1. First line: Comma-separated indices of chosen initial centroids
2. Following lines: Final cluster centers (rounded to 4 decimal places)

### Elbow Method Analysis (Bonus)

Analyze the iris dataset to find the optimal number of clusters:

```bash
python bonus.py
```

This generates `elbow.png` showing the elbow curve and the detected optimal cluster point.

## Project Structure

```
SoftwareProj-2/
├── README.md              # This file
├── setup.py              # Python setup script for C extension
├── kmeanspp.py           # Python interface for data processing and I/O
├── kmeansmodule.c        # C implementation of k-means++ algorithm
├── bonus.py              # Elbow method analysis tool
└── [data files]          # Input CSV files
```

## How It Works

### K-means++ Initialization
1. Randomly select first centroid from data points
2. For each of k-1 remaining centroids:
   - Calculate distance from each point to nearest centroid
   - Select next centroid with probability proportional to squared distance
   - This reduces likelihood of poor initialization

### Iterative Clustering (C Implementation)
- Performed by `mykmeanspp.fit()` in the C extension
- Assigns points to nearest centroid
- Updates centroids as mean of assigned points
- Continues until convergence (change < epsilon) or max iterations reached

## File Format

Input CSV files should have:
- **First column**: ID/identifier for each data point
- **Remaining columns**: Numerical features for clustering

Example:
```
1,2.5,3.1,4.2
2,2.3,3.0,4.5
3,2.6,2.9,4.1
```

## Language Composition

- **C**: 71.6% (k-means++ algorithm implementation)
- **Python**: 28.4% (interface, data processing, analysis tools)

## Error Handling

The program validates input parameters and returns error codes:
- Invalid k value: `"Invalid number of clusters!"`
- Invalid iterations: `"Invalid maximum iteration!"`
- Invalid epsilon: `"Invalid epsilon!"`
- General errors: `"An Error Has Occurred"`

## Notes

- Uses fixed random seed (1234) for reproducible initialization
- CSV files are merged on first column (ID) using inner join
- Data is sorted by ID before clustering
- Centroids are output with exactly 4 decimal places
