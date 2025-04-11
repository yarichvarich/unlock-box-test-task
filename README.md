# SecureBox Unlocking Task

## Overview
This repository contains a solution to the SecureBox unlocking task. The problem involves unlocking a two-dimensional grid where toggling a cell affects the entire row and column. The objective is to transform the entire grid into `false` values.

## Task Description
The SecureBox provides the following public API:
- `toggle(y, x)`: Toggles the state at position `(y, x)`, along with all cells in the same row and column.
- `isLocked()`: Returns `true` if any cell in the grid is `true` (locked), otherwise returns `false`.
- `getState()`: Returns the current state of the grid.

The goal is to determine the correct sequence of toggle operations to unlock the grid fully.

## Solution Approach
The key insight is that toggling a cell twice has no effect (acts like XOR(1)). Therefore, a solution always exists since the `shuffle` function also operates using `toggle`. The approach follows these steps:

1. **Mathematical Representation**: The problem is represented as a system of linear equations over modulo 2.
2. **Coefficient Matrix Construction**: A matrix of size `(x*y) x (x*y)` is created, where each row represents how a toggle affects the entire grid.
3. **Result Vector Formation**: A vector of size `(x*y)` represents the initial state of the box.
4. **Gaussian Elimination in Modulo 2**: A modified Gaussian elimination algorithm solves the system.
5. **Handling Infinite Solutions**: If multiple solutions exist, a basis is chosen, and one valid solution is applied.

## Complexity Analysis
The overall complexity of the algorithm is determined by the Gaussian elimination on a matrix of size `(x*y) x (x*y)`. The complexity of Gaussian elimination is **O((x*y)^3)** in the worst case. Thus, the unlocking algorithm runs in **O((x*y)^3)** time complexity.

For comparison, a brute-force approach would have a complexity of **O(2^(x*y))**, making it infeasible for large grids. This demonstrates that the proposed solution is significantly more efficient.

## Implementation
The algorithm consists of the following steps:
- Constructing the coefficient matrix and result vector.
- Performing Gaussian elimination in modulo 2.
- Applying the computed toggle operations.
- Checking if the box is successfully unlocked.

## Requirements
- C++17 or later
- Standard Template Library (STL)

## Installation & Compilation
```sh
# Clone the repository
git clone https://github.com/yarichvarich/unlock-box-tet-task.git
cd repository-name

# Compile the code
g++ -o securebox main.cpp -std=c++17
```

## Usage
```sh
./securebox <rows> <cols>
```
Example:
```sh
./securebox 5 5
```

## Contribution
Feel free to fork this repository and submit pull requests for improvements.

## License
This project is licensed under the [MIT License](LICENSE).
