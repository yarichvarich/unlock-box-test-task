#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

/*
You are given a locked container represented as a
two-dimensional grid of boolean values (true =
locked, false = unlocked). Your task is to write
an algorithm that fully unlocks the box, i.e.,
transforms the entire matrix into all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
    - Use the SecureBox public API (toggle,
isLocked, getState).
    - Strategically toggle cells to reach a state
where all elements are false.
    - Return true if the box remains locked, false
if successfully unlocked. You are not allowed to
path or modify the SecureBox class.

Evaluation Criteria:
    - Functional correctness
    - Computational efficiency
    - Code quality, structure, and comments
    - Algorithmic insight and clarity
*/

// NOLINTBEGIN
class SecureBox
{
  private:
    std::vector<std::vector<bool>> box;

  public:
    //================================================================================
    // Constructor: SecureBox
    // Description: Initializes the secure box
    // with a given size and
    //              shuffles its state using a
    //              pseudo-random number generator
    //              seeded with current time.
    //================================================================================
    SecureBox(uint32_t y, uint32_t x) : ySize(y), xSize(x)
    {
        rng.seed(time(0));
        box.resize(y);
        for (auto &it : box)
            it.resize(x);
        shuffle();
    }

    //================================================================================
    // Method: toggle
    // Description: Toggles the state at position
    // (x, y) and also all cells in the
    //              same row above and the same
    //              column to the left of it.
    //================================================================================
    void toggle(uint32_t y, uint32_t x)
    {
        box[y][x] = !box[y][x];
        for (uint32_t i = 0; i < xSize; i++)
            box[y][i] = !box[y][i];
        for (uint32_t i = 0; i < ySize; i++)
            box[i][x] = !box[i][x];
    }

    //================================================================================
    // Method: isLocked
    // Description: Returns true if any cell
    //              in the box is true (locked);
    //              false otherwise.
    //================================================================================
    bool isLocked()
    {
        for (uint32_t x = 0; x < xSize; x++)
            for (uint32_t y = 0; y < ySize; y++)
                if (box[y][x])
                    return true;

        return false;
    }

    //================================================================================
    // Method: getState
    // Description: Returns a copy of the current
    // state of the box.
    //================================================================================
    std::vector<std::vector<bool>> getState() { return box; }

  private:
    std::mt19937_64 rng;
    uint32_t ySize, xSize;

    //================================================================================
    // Method: shuffle
    // Description: Randomly toggles cells in the
    // box to create an initial locked state.
    //================================================================================
    void shuffle()
    {
        for (uint32_t t = rng() % 1000; t > 0; t--)
            toggle(rng() % ySize, rng() % xSize);
    }
};
// NOLINTEND

//================================================================================
// Function: openBox
// Description: Your task is to implement this
// function to unlock the SecureBox.
//              Use only the public methods of
//              SecureBox (toggle, getState,
//              isLocked). You must determine the
//              correct sequence of toggle
//              operations to make all values in
//              the box 'false'. The function
//              should return false if the box is
//              successfully unlocked, or true if
//              any cell remains locked.
//================================================================================

//================================================================================
// SOLUTION:
//================================================================================

void printMatrix(const std::vector<std::vector<bool>> &state)
{
    for (auto &rowItems : state)
    {
        for (auto rowItem : rowItems)
        {
            std::cout << rowItem << " ";
        }
        std::cout << std::endl;
    }
}

void printVector(const std::vector<bool> &v)
{
    for (auto item : v)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

uint32_t convertToUpperDim(uint32_t row, uint32_t col, uint32_t dim) { return row * dim + col; }

const std::pair<uint32_t, uint32_t> convertToLowerDim(uint32_t upperDim, uint32_t dim)
{
    return std::pair<uint32_t, uint32_t>(upperDim / dim, upperDim % dim);
}

const std::vector<std::vector<bool>>
generateCoefficientMatrix(const std::vector<std::vector<bool>> &state, uint32_t rows, uint32_t cols)
{
    std::vector<std::vector<bool>> result =
        std::vector<std::vector<bool>>(rows * cols, std::vector<bool>(rows * cols, 0));

    for (uint32_t rowUpper = 0; rowUpper < result.size(); ++rowUpper)
    {
        for (uint32_t colUpper = 0; colUpper < result[rowUpper].size(); ++colUpper)
        {
            const auto coeffPos = convertToLowerDim(colUpper, cols);
            const auto resultPos = convertToLowerDim(rowUpper, cols);

            result[rowUpper][colUpper] =
                coeffPos.first == resultPos.first || coeffPos.second == resultPos.second;
        }
    }

    return result;
}

const std::vector<bool> generateCoefficientVector(const std::vector<std::vector<bool>> &state,
                                                  uint32_t rows, uint32_t cols)
{
    std::vector<bool> result(rows * cols, false);

    for (uint32_t row = 0; row < rows; ++row)
    {
        for (uint32_t col = 0; col < cols; ++col)
        {
            const auto pos = convertToUpperDim(row, col, cols);
            result[pos] = state[row][col];
        }
    }

    return result;
}

std::vector<bool> gaussEliminationMod2(std::vector<std::vector<bool>> matrix, std::vector<bool> rhs)
{
    int n = matrix.size();    // Number of rows
    int m = matrix[0].size(); // Number of columns (variables)

    std::vector<int> pivotColumn(m, -1); // Tracks pivot positions
    std::vector<bool> solution(m, 0);    // Stores the solution

    int rank = 0; // Number of independent rows

    // Forward elimination
    for (int col = 0; col < m; ++col)
    {
        int pivotRow = -1;

        // Find a row with a leading 1 in this column
        for (int row = rank; row < n; ++row)
        {
            if (matrix[row][col])
            {
                pivotRow = row;
                break;
            }
        }

        // If no pivot found, move to the next column
        if (pivotRow == -1)
            continue;

        // Swap rows to bring pivot row to the top
        swap(matrix[rank], matrix[pivotRow]);
        swap(rhs[rank], rhs[pivotRow]);

        // Mark this column as a pivot column
        pivotColumn[col] = rank;

        // Eliminate other rows
        for (int row = 0; row < n; ++row)
        {
            if (row != rank && matrix[row][col])
            {
                for (int k = col; k < m; ++k)
                {
                    matrix[row][k] = matrix[row][k] ^ matrix[rank][k]; // XOR row
                }
                rhs[row] = rhs[row] ^ rhs[rank]; // Update RHS
            }
        }
        ++rank; // Move to the next row
    }

    // Check for inconsistency (row of zeroes with RHS = 1)
    for (int row = rank; row < n; ++row)
    {
        if (rhs[row])
        {
            std::cout << "No solution\n";
            return {}; // No solution exists
        }
    }

    // Back-substitution: Compute at least one solution
    for (int col = 0; col < m; ++col)
    {
        if (pivotColumn[col] != -1)
        {
            // Pivot variables are determined by elimination
            solution[col] = rhs[pivotColumn[col]];
        }
        else
        {
            // Free variables (if any) are set to 0
            solution[col] = 0;
        }
    }

    return solution; // Return a particular solution
}

std::vector<std::vector<bool>> transposeMatrix(const std::vector<std::vector<bool>> &matrix)
{
    if (matrix.empty())
        return {}; // Handle empty matrix case

    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<bool>> transposed(cols, std::vector<bool>(rows, false));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            transposed[j][i] = matrix[i][j];
        }
    }

    return transposed;
}

auto openBox(uint32_t y, uint32_t x) -> bool
{
    SecureBox box(y, x);

    auto state = box.getState();
    auto isTransposed = false;

    if (y > x)
    {
        std::swap(y, x);
        isTransposed = true;
        state = transposeMatrix(state);
    }

    auto generatedMatrix = generateCoefficientMatrix(state, y, x);
    auto generatedVector = generateCoefficientVector(state, y, x);
    // Applying solution

    const auto solution = gaussEliminationMod2(generatedMatrix, generatedVector);

    for (uint32_t upperDim = 0; upperDim < solution.size(); ++upperDim)
    {
        if (solution[upperDim])
        {
            const auto tooggleCoord = convertToLowerDim(upperDim, x);

            if (!isTransposed)
            {
                box.toggle(tooggleCoord.first, tooggleCoord.second);
            }
            else
            {
                box.toggle(tooggleCoord.second, tooggleCoord.first);
            }
        }
    }

    return box.isLocked();
}

int main(int argc, char *argv[])
{
    uint32_t y = std::atol(argv[1]);
    uint32_t x = std::atol(argv[2]);

    bool state = openBox(y, x);

    if (state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return 0;
}
