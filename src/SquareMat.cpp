//talgov44@gmail.com

#include "SquareMat.hpp"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <sstream>

using namespace std;

namespace mat 
{

    /**
    Default constructor - asks for the mat size and values and creates it
    Parameters:
        none
    Return:
        -
    */
    SquareMat::SquareMat()
    {
        // Getting the mat size
        cout << "Enter the size of the mat: ";
        cin >> size;

        if (cin.fail() || (size <= 0)) // Invalid input
        {
            throw invalid_argument("Error: size must be a positive integer.");
        }

        // Creating the mat:
        mat = new double*[size];

        for (int i=0 ; i<size ; i++)
        {
            mat[i] = new double[size];
        }

        cin.ignore();

        // Getting the values
        for (int i= 0 ; i < size ; i++) {
            string matLine;

            cout << "Enter " << size << " numbers, separated by space:\n";
            getline(cin, matLine);
            stringstream ss(matLine);

            double matNum;
            int lineCount = 0;

            while (ss >> matNum) {
                if(lineCount >= size) {
                    throw invalid_argument("Error: row " + to_string(i + 1) + " has too many values");
                }

                mat[i][lineCount] = matNum;
                lineCount++;
            }

            if(lineCount < size) {
                throw invalid_argument("Error: row " + to_string(i + 1) + " has wrong number of values");
            }
        }
    }

    /**
    Constructor - creates a square matrix (identity) of given size
    Parameters:
        sizeMat - the size
    Return:
        -
    */
    SquareMat::SquareMat(int sizeMat) : size(sizeMat)
    {
        if (sizeMat < 0) // Size cannot be negative
        {
            throw invalid_argument("Matrix size cannot be negative.");
        }

        if (sizeMat == 0) // sizeMat is 0, initialize as an empty matrix.
        {
            mat = nullptr;
            return;
        }

        mat = new double*[size];

        for (int i = 0; i < size; ++i)
        {
            mat[i] = new double[size];

            for (int j = 0; j < size; ++j)
            {
                mat[i][j] = (i == j) ? 1.0 : 0.0; // Initialize as identity matrix
            }
        }
    }

    /**
    Copy Constructor
    Parameters:
        secondMat - the mat to copy
    Return:
        -
    */
    SquareMat::SquareMat(const SquareMat& secondMat) : size(secondMat.size)
    {
        if (secondMat.mat == nullptr) // Empty mat
        {
            mat = nullptr;
            return;
        }

        mat = new double*[size];

        for (int i = 0; i < size; ++i)
        {
            mat[i] = new double[size];

            for (int j = 0; j < size; ++j)
            {
                mat[i][j] = secondMat.mat[i][j]; // Deep copy the elements
            }
        }
    }

    /**
    Destructor for deleting the mat
    Parameters:
        none
    Return:
        -
    */
    SquareMat::~SquareMat()
    {
        if (mat) // Check if the mat is not null
        {
            for (int i = 0; i < size; ++i)
            {
                delete[] mat[i]; // Delete each row
            }

            delete[] mat; // Delete the array of row pointers
            mat = nullptr; // Set to nullptr
        }
    }

    /**
    This function performs the assignment operator (copy semantics for dynamic memory)
    Parameters:
        none
    Return:
        SquareMat& - the mat
    */
    SquareMat& SquareMat::operator=(const SquareMat& secondMat)
    {
        if (this == &secondMat) // Handle self-assignment
        {
            return *this;
        }

        // Deallocate old memory
        if (mat)
        {
            for (int i = 0; i < size; ++i)
            {
                delete[] mat[i];
            }

            delete[] mat;
        }

        // Copy size
        size = secondMat.size;

        // Allocate new memory
        if (size == 0) // Handle 0x0 mat
        {
            mat = nullptr;
        }
        else
        {
            mat = new double*[size];

            for (int i = 0; i < size; ++i)
            {
                mat[i] = new double[size];

                // Deep copy the elements
                for (int j = 0; j < size; ++j)
                {
                    mat[i][j] = secondMat.mat[i][j];
                }
            }
        }

        return *this;
    }

    /**
    This function and template are used for iterating over the mat and applying different changes to it
    *The modifiable and non const version
    Parameters:
        func - the function that applies to each element (getting the itereting parameters and the element)
    Returns:
        none
    */
    template <typename Func>
    void SquareMat::map(Func func)
    {
        if (mat == nullptr || size == 0) // 0x0 mat
        {
            return;
        }

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                func(i, j, mat[i][j]); // The passed functions
            }
        }
    }

    /**
    This function and template are used for iterating over the mat and applying different changes to it
    *The const version
    Parameters:
        func - the function that applies to each element (getting the itereting parameters and the element)
    Returns:
        none
    */
    template <typename Func>
    void SquareMat::map(Func func) const
    {
        if (mat == nullptr || size == 0) // 0x0 mat
        {
            return;
        }

        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                func(i, j, mat[i][j]); // The passed functions
            }
        }
    }

    /**
    This function sums the mat elements
    Parameters:
        none
    Returns:
        double - the sum
    */
    double SquareMat::sumElements() const
    {
        double sum = 0; // The sum

        if (mat == nullptr || size == 0) // 0x0 mat - size 0
        {
            return 0.0;
        }

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning for i, j
            sum += element; // Add each element to the sum
        });

        return sum;
    }

    /**
    This function gets the cofactor of mat[p][q] in temp
    *Const function
    Parameters:
        src_mat_data - the source matrix data (dynamic double**)
        temp_mat_data - the temp mat to store cofactor (dynamic double**)
        p - row index to exclude
        q - column index to exclude
        n - the current size of the matrix (sub-matrix size)
    Returns:
        none
    */
    void SquareMat::getCofactor(const double* const* src_mat_data, double** temp_mat_data,
        int p, int q, int n) const
    {
        int i = 0, j = 0;

        for (int row = 0; row < n; row++) // Looping through the source matrix
        {
            for (int col = 0; col < n; col++)
            {
                // Copying the elements that aren't in the given row 'p' and column 'q' into a temp new mat
                if (row != p && col != q)
                {
                    temp_mat_data[i][j++] = src_mat_data[row][col];

                    // If a row in temp_mat_data is filled, move to the next one
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    /**
    This function gets the determinant of a mat
    *Recursive and const function
    Parameters:
        mat_data - the matrix data (dynamic double**)
        n - the current size of the matrix (sub-matrix size)
    Returns:
        double - the determinant
    */
    double SquareMat::determinantOfMatrix(const double* const* mat_data, int n) const
    {
        double determinant_result = 0; // Initialize result

        if (n == 1) // If the mat contains a single element - its value
        {
            return mat_data[0][0];
        }

        if (n <= 0 || mat_data == nullptr) // Invalid mat - 0
        {
            return 0.0;
        }

        // Dynamically allocate temp to store cofactors
        double** temp = new double*[n];

        for (int i = 0; i < n; ++i)
        {
            temp[i] = new double[n];
        }

        int sign = 1; // To store sign multiplier

        // Iterating over the first row
        for (int f = 0; f < n; f++)
        {
            // Getting Cofactor of mat_data[0][f]
            getCofactor(mat_data, temp, 0, f, n);
            determinant_result += sign * mat_data[0][f] * determinantOfMatrix(temp, n - 1);

            sign = -sign;
        }

        // Deallocate the temp matrix
        for (int i = 0; i < n; ++i)
        {
            delete[] temp[i];
        }

        delete[] temp;

        return determinant_result;
    }


    /**
    This function adds another mat's values to this one
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator+(const SquareMat& secondMat) const
    {
        if (size != secondMat.size)
        {
            throw invalid_argument("Matrix sizes must match for addition.");
        }

        SquareMat resultMat(size); // Create result matrix of the correct size

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = element + secondMat.mat[i][j]; // Adds the second mat's value
        });

        return resultMat;
    }

    /**
    This function substructs another mat's values from this one
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator-(const SquareMat& secondMat) const
    {
        if (size != secondMat.size)
        {
            throw invalid_argument("Matrix sizes must match for subtraction.");
        }

        SquareMat resultMat(size); // Create result matrix of the correct size

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = element - secondMat.mat[i][j]; // Substracts the second mat's value
        });

        return resultMat;
    }

    /**
    This function corresponds each element of the mat (unary negation)
    *Const function
    Parameters:
        none
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator-() const
    {
        SquareMat resultMat(size); // Create result matrix of the correct size

        map([&](int i, int j, double element)
        {
            (void)i; (void)j, (void)element; // Suppress unused warning
            double curr_val = -mat[i][j]; // The current value

            // Normalizing -0 to 0 (fmod might produce -0.0)
            if (curr_val == 0.0)
            {
                curr_val = 0.0;
            }

            resultMat.mat[i][j] = curr_val;
        });

        return resultMat;
    }

    /**
    This function performs multiplication between two matrices
    (sum of the products of matching row and column elements)
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator*(const SquareMat& secondMat) const
    {
        // For matrix multiplication A*B, A's columns must match B's rows.
        // For SquareMat, this means sizes must be equal.
        if (size != secondMat.size)
        {
            throw invalid_argument("Matrix sizes must match for multiplication.");
        }

        SquareMat resultMat(size); // Create result matrix of the correct size

        // Iterating over the rows of first matrix
        for (int i = 0; i < size; i++)
        {
            // Iterating over the columns of second matrix
            for (int j = 0; j < size; j++)
            {
                resultMat.mat[i][j] = 0; // Resetting the result element for current position

                // Iterating over the columns of first matrix (or rows of second) for the dot product
                for (int k = 0; k < size; k++)
                {
                    resultMat.mat[i][j] += mat[i][k] * secondMat.mat[k][j];
                }
            }
        }

        return resultMat;
    }

    /**
    This function multiplies each mat element with a given scaler
    *Const function
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator*(int scaler) const
    {
        SquareMat resultMat(size); // Create result matrix of the correct size

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = element * scaler; // Multiplying
        });

        return resultMat;
    }

    /**
    This function multiplies each mat element with a given scaler
    *Friend function version to allow using "scaler * mat"
    Parameters:
        scaler - the scaler
        matrix - the current mat
    Returns:
        SquareMat - the result mat
    */
    SquareMat operator*(int scaler, const SquareMat& matrix)
    {
        SquareMat resultMat(matrix.size);

        // Can't use map here because it's a friend function and map is a member, Manually iterate:
        for (int i = 0; i < matrix.size; i++)
        {
            for (int j = 0; j < matrix.size; j++)
            {
                resultMat.mat[i][j] = matrix.mat[i][j] * scaler; // Multiplying
            }
        }

        return resultMat;
    }

    /**
    This function multiplies each mat element with another mat's matching element (in index)
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator%(const SquareMat& secondMat) const
    {
        if (size != secondMat.size)
        {
            throw invalid_argument("Matrix sizes must match for element-wise multiplication.");
        }

        SquareMat resultMat(size); // Create result matrix of the correct size

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = element * secondMat.mat[i][j]; // Multiplying
        });

        return resultMat;
    }

    /**
    This function calculates the modulo of each mat element with a given scaler
    *Const function
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator%(int scaler) const
    {
        SquareMat resultMat(size);

        if (scaler == 0) // Exception for division by zero
        {
            throw invalid_argument("Modulo by zero is undefined");
        }

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = fmod(element, static_cast<double>(scaler)); // Floating-point modulo for doubles
        });

        return resultMat;
    }

    /**
    This function calculates the division of each mat element with a given scaler
    *Const function
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator/(int scaler) const
    {
        SquareMat resultMat(size);

        if (scaler == 0) // Exception for division by zero
        {
            throw invalid_argument("Cannot divide by zero");
        }

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = element / static_cast<double>(scaler); // Dividing
        });

        return resultMat;
    }

    /**
    This function calculates the pow of each mat element with a given scaler
    *Const function
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator^(int scaler) const
    {
        SquareMat resultMat(size);

        map([&](int i, int j, double element)
        {
            (void)i; (void)j; // Suppress unused warning
            resultMat.mat[i][j] = pow(element, static_cast<double>(scaler)); // element^scaler
        });

        return resultMat;
    }

    /**
    This function performs a *prefix* increment for each mat element (increases each by one)
    and returns the updated mat
    Parameters:
        none
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator++()
    {
        map([&](int i, int j, double& element)
        {
            (void)i; (void)j; // Suppress unused warning
            element += 1.0;
        });

        return *this;
    }

    /**
    This function performs a *postfix* increment for each mat element (increases each by one)
    and returns the mat before the update
    Parameters:
        int to perform the postfix (dummy parameter)
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator++(int)
    {
        SquareMat temp = *this; // Using copy constructor to save the state before increment

        map([&](int i, int j, double& element)
        {
            (void)i; (void)j; // Suppress unused warning
            element += 1.0;
        });

        return temp;
    }

    /**
    This function performs a *prefix* decrement for each mat element (decreases each by one)
    and returns the updated mat
    Parameters:
        none
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator--()
    {
        map([&](int i, int j, double& element)
        {
            (void)i; (void)j; // Suppress unused warning
            element -= 1.0;
        });

        return *this; // Return the modified current object
    }

    /**
    This function performs a *postfix* decrement for each mat element (decreases each by one)
    and returns the mat before the update
    Parameters:
        int to perform the postfix (dummy parameter)
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator--(int)
    {
        SquareMat temp = *this; // Using copy constructor to save the state before decrement

        map([&](int i, int j, double& element)
        {
            (void)i; (void)j; // Suppress unused warning
            element -= 1.0;
        });

        return temp;
    }

    /**
    This function swaps the mat's row with its columns (transposes the matrix)
    *Const function
    Parameters:
        none
    Returns:
        SquareMat - the result mat
    */
    SquareMat SquareMat::operator~() const
    {
        SquareMat resultMat(size);

        // Iterating over the mat
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                resultMat.mat[i][j] = mat[j][i]; // Swapping row and column indices
            }
        }

        return resultMat;
    }

    /**
    This function adds access by index option (e.g., mat[row][col])
    *The modifiable and non const version to allow updates to the element
    Parameters:
        index - the row index
    Returns:
        double* - a pointer to the row (to allow subsequent column access)
    */
    double* SquareMat::operator[](int index)
    {
        if (mat == nullptr || size == 0) // Empty/uninitialized mat
        {
            throw out_of_range("Matrix is not initialized or is empty.");
        }

        if ((index < 0) || (index >= size)) // Check bounds
        {
            throw out_of_range("Row index out of bounds");
        }

        return mat[index];
    }

    /**
    This function adds access by index option (e.g., const_mat[row][col])
    *The const, read-only version
    Parameters:
        index - the row index
    Returns:
        const double* - a const pointer to the row (read-only access)
    */
    const double* SquareMat::operator[](int index) const
    {
        if (mat == nullptr || size == 0) // Empty/uninitialized mat
        {
            throw out_of_range("Matrix is not initialized or is empty.");
        }

        if ((index < 0) || (index >= size)) // Check bounds using 'size'
        {
            throw out_of_range("Row index out of bounds");
        }

        return mat[index];
    }

    // Functions using the sumElements function:

    /**
    This function checks whether the mat's elements sum is equal to another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if sums are equal, false otherwise
    */
    bool SquareMat::operator==(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return false;
        }

        if (size == 0 && secondMat.size == 0) // Both are 0x0
        {
            return true;
        }

        return (sumElements() == secondMat.sumElements());
    }

    /**
    This function checks whether the mat's elements sum is not equal to another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if sums are not equal, false otherwise
    */
    bool SquareMat::operator!=(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return true;
        }

        if (size == 0 && secondMat.size == 0) //Both are 0x0
        {
            return false;
        }

        return (sumElements() != secondMat.sumElements());
    }

    /**
    This function checks whether the mat's elements sum is bigger than another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if this matrix's sum is greater, false otherwise
    */
    bool SquareMat::operator>(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return false;
        }

        return (sumElements() > secondMat.sumElements());
    }

    /**
    This function checks whether the mat's elements sum is smaller than another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if this matrix's sum is smaller, false otherwise
    */
    bool SquareMat::operator<(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return false;
        }

        return (sumElements() < secondMat.sumElements());
    }

    /**
    This function checks whether the mat's elements sum is bigger or equal to another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if this matrix's sum is greater or equal, false otherwise
    */
    bool SquareMat::operator>=(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return false;
        }

        return (sumElements() >= secondMat.sumElements());
    }

    /**
    This function checks whether the mat's elements sum is smaller or equal to another's sum
    *Const function
    Parameters:
        secondMat - the second mat
    Returns:
        bool - true if this matrix's sum is smaller or equal, false otherwise
    */
    bool SquareMat::operator<=(const SquareMat& secondMat) const
    {
        if (size != secondMat.size) // Different sizes
        {
            return false;
        }

        return (sumElements() <= secondMat.sumElements());
    }

    /**
    This function calculates the determinant of the mat
    *Const friend function
    Parameters:
        matrix - the matrix for which to calculate the determinant
    Returns:
        double - the determinant
    */
    double operator!(const SquareMat& matrix)
    {
        if (matrix.size == 0) // Different sizes
        {
            return 0.0;
        }

        return matrix.determinantOfMatrix(matrix.mat, matrix.size);
    }

    /**
    This function updates each mat element with the addition of another's (by matching indexes)
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator+=(const SquareMat& secondMat)
    {
        *this = *this + secondMat;
        return *this; // Return reference to modified object
    }

    /**
    This function updates each mat element with the subtraction of another's (by matching indexes)
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator-=(const SquareMat& secondMat)
    {
        *this = *this - secondMat;
        return *this; // Return reference to modified object
    }

    /**
    This function updates the mat with the multiplication of another's (matrix multiplication)
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator*=(const SquareMat& secondMat)
    {
        *this = *this * secondMat;
        return *this; // Return reference to modified object
    }

    /**
    This function updates each mat element with the multiplication of a given scaler
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator*=(int scaler)
    {
        *this = *this * scaler;
        return *this; // Return reference to modified object
    }

    /**
    This function updates each mat element with the division of a given scaler
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator/=(int scaler)
    {
        *this = *this / scaler;
        return *this; // Return reference to modified object
    }

    /**
    This function updates each mat element with the element-wise multiplication of another's
    Parameters:
        secondMat - the second mat
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator%=(const SquareMat& secondMat)
    {
        *this = *this % secondMat;
        return *this; // Return reference to modified object
    }

    /**
    This function updates each mat element with its modulo by a given scaler
    Parameters:
        scaler - the scaler
    Returns:
        SquareMat& - reference to the modified current object
    */
    SquareMat& SquareMat::operator%=(int scaler)
    {
        *this = *this % scaler;
        return *this; // Return reference to modified object
    }

    /**
    This function prints the mat to an output stream
    *Friend function to allow using << ("cout << mat")
    Parameters:
        os - the output stream for prints
        matrix - the matrix to print
    Returns:
        ostream& - to allow chaining of output operations (e.g., cout << mat1 << mat2)
    */
    ostream& operator<<(ostream& os, const SquareMat& matrix)
    {
        if (matrix.mat == nullptr || matrix.size == 0) // Invalid mat
        {
            os << "[]";
            return os;
        }

        ios_base::fmtflags original_flags = os.flags(); // Save original flags
        os << fixed << setprecision(2); // Set format

        for (int i = 0; i < matrix.size; i++)
        {
            for (int j = 0; j < matrix.size; j++)
            {
                // Space between elements
                if (j > 0)
                {
                    os << " ";
                }

                os << matrix.mat[i][j]; // The element
            }

            os << endl; // Next line after each row
        }

        os.flags(original_flags); // Restore original flags
        return os;
    }

}