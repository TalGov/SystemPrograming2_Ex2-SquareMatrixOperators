//talgov44@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "SquareMat.hpp"

#include <limits>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;
using std::numeric_limits;
using namespace mat;

/**
This function fills a mat with incrementing numbers from a starting value
Parameters:
    mat - the mat
    start - the starting value
Return:
    none
*/
void fillMat(SquareMat& mat, double start = 1.0) 
{
    if (mat.getSize() == 0) //No elements to change
    {
        return;
    }

    for (int i = 0; i < mat.getSize(); i++)
    {
        for (int j = 0; j < mat.getSize(); j++)
        {
            mat[i][j] = start + j + (i * mat.getSize()); //Using operator[]
        }
    }
}

/**
This function sets all the mat elements to 0
Parameters:
    mat - the mat
Return:
    none
*/
void zeroMat(SquareMat& mat) 
{
    if (mat.getSize() == 0) //No elements to reset
    {
        return;
    }

    for (int i = 0; i < mat.getSize(); i++)
    {
        for (int j = 0; j < mat.getSize(); j++)
        {
            mat[i][j] = 0.0; //Using operator[]
        }
    }
}

/**
This function sets an identity mat
Parameters:
    mat - the mat
Return:
    none
*/
void identityMat(SquareMat& mat) 
{
    if (mat.getSize() == 0) //No elements to change
    {
        return;
    }

    zeroMat(mat); // First resetting all the elements to 0

    for (int i = 0; i < mat.getSize(); i++)
    {
        mat[i][i] = 1.0; //Using operator[]
    }
}

/**
This function fills a mat with NANs
Parameters:
    mat - the mat
Return:
    none
*/
void nanMat(SquareMat& mat) 
{
    if (mat.getSize() == 0) //No elements to change
    {
        return;
    }

    for (int i = 0; i < mat.getSize(); i++)
    {
        for (int j = 0; j < mat.getSize(); j++)
        {
            mat[i][j] = NAN; //Using operator[]
        }
    }
}

/**
This function fills a mat with infinity values
Parameters:
    mat - the mat
Return:
    none
*/
void infMat(SquareMat& mat) 
{
    if (mat.getSize() == 0) //No elements to change
    {
        return;
    }

    for (int i = 0; i < mat.getSize(); i++)
    {
        for (int j = 0; j < mat.getSize(); j++)
        {
            mat[i][j] = numeric_limits<double>::infinity(); //Using operator[]
        }
    }
}


/**
This function build a new matrix from given values
It calls the constructor and gets a mock values from the code to simulate user input
Parameters:
    input - the "user" input
Return:
    SquareMat - the new mat
*/
SquareMat buildMockMatrix(const std::string& input)
{
    // Creates the input:
    std::istringstream mockInput(input);
    std::streambuf* origCin = std::cin.rdbuf();
    std::cin.rdbuf(mockInput.rdbuf()); // Directing the cin to the mock input

    SquareMat mat; // Creates the mat with the default constructor

    std::cin.rdbuf(origCin); // Redirecting the cin to the user input
    
    return mat;
}

TEST_CASE("Constructors and Destructor") 
{
    SUBCASE("Parameterized Constructor - Identity Matrix Initialization (3x3)") 
    {
        cout << endl << endl << "~ Constructors and Destructor:" << endl;
        int test_size = 3;
        SquareMat id_mat(test_size); // Explicitly construct with size
        SquareMat expected_id(test_size); // Create another for comparison

        CHECK(id_mat.getSize() == test_size);
        identityMat(expected_id); // Fill it as identity using helper
        
        for(int i = 0; i < test_size; ++i) 
        {
            for(int j = 0; j < test_size; ++j) 
            {
                CHECK(id_mat[i][j] == doctest::Approx(expected_id[i][j]));
            }
        }
    }

    SUBCASE("Parameterized Constructor - Zero Size") 
    {
        // This should now pass as SquareMat(0) is handled without throwing
        SquareMat zero_size_mat(0);
        CHECK(zero_size_mat.getSize() == 0);
        CHECK(zero_size_mat.sumElements() == doctest::Approx(0.0));
    }

    SUBCASE("Parameterized Constructor - Negative Size Throws") 
    {
        CHECK_THROWS_AS(SquareMat(-1), invalid_argument);
    }

    SUBCASE("Copy Constructor") 
    {
        cout << endl << endl;
        SquareMat mat = buildMockMatrix("3\n5 6 7\n8 9 10\n11 12 13");
        
        SquareMat copy = mat; // Call copy constructor
        CHECK(copy.getSize() == mat.getSize());

        for(int i = 0; i < mat.getSize(); ++i) 
        {
            for(int j = 0; j < mat.getSize(); ++j) 
            {
                CHECK(copy[i][j] == doctest::Approx(mat[i][j]));
            }
        }

        mat[0][0] = 99.0; // Use double literal
        CHECK(copy[0][0] == doctest::Approx(5.0)); // Use double literal
    }

    SUBCASE("Assignment Operator") 
    {
        cout << endl << endl;
        SquareMat mat = buildMockMatrix("3\n5 6 7\n8 9 10\n11 12 13");
        SquareMat assigned(mat.getSize()); // Explicitly construct with size
        assigned = mat; // Call assignment operator

        CHECK(assigned.getSize() == mat.getSize());
        for(int i = 0; i < mat.getSize(); ++i) 
        {
            for(int j = 0; j < mat.getSize(); ++j) 
            {
                CHECK(assigned[i][j] == doctest::Approx(mat[i][j]));
            }
        }

        mat[0][0] = 100.0; // Use double literal
        CHECK(assigned[0][0] == doctest::Approx(5.0)); // Use double literal

        // Assignment from a different sized matrix
        SquareMat small_mat(2);
        SquareMat large_mat(4);
        
        large_mat = small_mat; // Assign smaller to larger

        CHECK(large_mat.getSize() == small_mat.getSize());

        // Compare elements individually
        for (int i = 0; i < large_mat.getSize(); ++i) 
        { 
            for (int j = 0; j < large_mat.getSize(); ++j) 
            {
                CHECK(large_mat[i][j] == small_mat[i][j]);
            }
        }

        small_mat = large_mat; // Assign larger to smaller
        CHECK(small_mat.getSize() == large_mat.getSize());

        // Compare elements individually
        for (int i = 0; i < small_mat.getSize(); ++i) 
        { 
            for (int j = 0; j < small_mat.getSize(); ++j) 
            {
                CHECK(large_mat[i][j] == small_mat[i][j]);
            }
        }
    }
}

TEST_CASE("Addition operator+") 
{
    cout << endl << endl << endl << "~ Addition operator+:" << endl;

    SquareMat matOne = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat matTwo = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    SquareMat result = matOne + matTwo;

    CHECK(result.getSize() == matOne.getSize());
    CHECK(result[0][0] == doctest::Approx(1.0 + 10.0)); // Use double literals
    CHECK(result[1][2] == doctest::Approx(matOne[1][2] + matTwo[1][2])); // Use operator[]

    SquareMat zero_mat(matOne.getSize()); // Explicitly construct with size
    zeroMat(zero_mat);
    SquareMat sum_with_zero = matOne + zero_mat;
    CHECK(sum_with_zero.getSize() == matOne.getSize());
    CHECK(sum_with_zero[2][2] == doctest::Approx(matOne[2][2]));

    // Edge case: Adding matrices of different sizes
    SquareMat small_mat(2); // Explicitly construct with size
    CHECK_THROWS_AS(matOne + small_mat, invalid_argument);
}

TEST_CASE("Subtraction operator-") 
{
    cout << endl << endl << endl << "~ Subtraction operator-:" << endl;

    SquareMat matOne = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    SquareMat matTwo = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat result = matOne - matTwo;

    CHECK(result.getSize() == matOne.getSize());
    CHECK(result[0][0] == doctest::Approx(10.0 - 1.0)); // Use double literals
    CHECK(result[2][2] == doctest::Approx(matOne[2][2] - matTwo[2][2])); // Use operator[]

    // Edge case: Subtracting matrices of different sizes
    SquareMat small_mat(2); // Explicitly construct with size
    CHECK_THROWS_AS(matOne - small_mat, invalid_argument);
}

TEST_CASE("Unary minus operator-") 
{
    cout << endl << endl << endl << "~ Unary minus operator-:" << endl;

    SquareMat mat = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat neg = -mat;

    CHECK(neg.getSize() == mat.getSize());
    CHECK(neg[0][0] == doctest::Approx(-1.0)); // Use double literal
    CHECK(neg[2][2] == doctest::Approx(-mat[2][2])); // Use operator[]

    mat[0][0] = 0.0; // Use double literal
    SquareMat neg_zero = -mat;
    CHECK(neg_zero[0][0] == doctest::Approx(0.0)); // Use double literal
    CHECK_FALSE(signbit(neg_zero[0][0])); // Ensure -0.0 becomes +0.0
}

TEST_CASE("Matrix Multiplication operator* (SquareMat)") 
{
    cout << endl << endl << endl << "~ Matrix Multiplication operator* (SquareMat):" << endl;

    SquareMat matOne = buildMockMatrix("3\n1 2 0\n3 4 0\n0 0 0");
    SquareMat matTwo = buildMockMatrix("3\n5 6 0\n7 8 0\n0 0 0");
    SquareMat result = matOne * matTwo;

    int size = matOne.getSize();

    CHECK(result.getSize() == size);
    CHECK(result[0][0] == doctest::Approx(19.0)); // Use double literals
    CHECK(result[0][1] == doctest::Approx(22.0)); // Use double literals
    CHECK(result[0][2] == doctest::Approx(0.0)); // Use double literals

    CHECK(result[1][0] == doctest::Approx(43.0)); // Use double literals
    CHECK(result[1][1] == doctest::Approx(50.0)); // Use double literals
    CHECK(result[1][2] == doctest::Approx(0.0)); // Use double literals

    CHECK(result[2][0] == doctest::Approx(0.0)); // Use double literals
    CHECK(result[2][1] == doctest::Approx(0.0)); // Use double literals
    CHECK(result[2][2] == doctest::Approx(0.0)); // Use double literals

    // Multiplying matrices of different sizes throws
    SquareMat small_mat(2); // Explicitly construct with size
    CHECK_THROWS_AS(matOne * small_mat, invalid_argument);
}

TEST_CASE("Scalar Multiplication operator* (int scaler) and friend version") 
{
    cout << endl << endl << endl << "~ Scalar Multiplication operator* (int scaler) and friend version:" << endl;

    SquareMat a = buildMockMatrix("3\n2 3 4\n5 6 7\n8 9 10");
    int test_size = a.getSize();
    
    SquareMat result1 = a * 3;
    CHECK(result1.getSize() == test_size);
    CHECK(result1[0][0] == doctest::Approx(6.0)); // Use double literal
    CHECK(result1[1][1] == doctest::Approx(a[1][1] * 3.0)); // Use double literal

    SquareMat result2 = 4 * a; // Using friend function
    CHECK(result2.getSize() == test_size);
    CHECK(result2[0][0] == doctest::Approx(8.0)); // Use double literal
    CHECK(result2[1][1] == doctest::Approx(a[1][1] * 4.0)); // Use double literal
}

TEST_CASE("Element-wise Multiplication operator% (SquareMat)") 
{
    cout << endl << endl << endl << "~ Element-wise Multiplication operator% (SquareMat):" << endl;

    SquareMat a = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat b = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    int test_size = a.getSize();

    SquareMat result = a % b; // Element-wise multiplication

    CHECK(result.getSize() == test_size);
    CHECK(result[0][0] == doctest::Approx(1.0 * 10.0)); // Use double literals
    CHECK(result[1][1] == doctest::Approx(a[1][1] * b[1][1])); // Use operator[]
    CHECK(result[2][2] == doctest::Approx(a[2][2] * b[2][2])); // Use operator[]

    // Element-wise multiplication with different sizes throws
    SquareMat small_mat(2); // Explicitly construct with size
    CHECK_THROWS_AS(a % small_mat, invalid_argument);
}

TEST_CASE("Scalar Modulo operator% (int scaler)") 
{
    cout << endl << endl << endl << "~ Scalar Modulo operator% (int scaler):" << endl;

    SquareMat a = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    int test_size = a.getSize();
    
    SquareMat mod = a % 7;
    CHECK(mod.getSize() == test_size);
    CHECK(mod[0][0] == doctest::Approx(fmod(10.0, 7.0))); // Use double literals
    CHECK(mod[1][0] == doctest::Approx(fmod(a[1][0], 7.0))); // Use double literal

    SquareMat mod1 = a % 1;
    CHECK(mod1.getSize() == test_size);
    CHECK(mod1[2][2] == doctest::Approx(0.0)); // Use double literal

    CHECK_THROWS_AS(a % 0, invalid_argument); // Invalid mod by zero
}

TEST_CASE("Scalar Division operator/ (int scaler)") 
{
    cout << endl << endl << endl << "~ Scalar Division operator/ (int scaler):" << endl;

    SquareMat a = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    int test_size = a.getSize();

    SquareMat div = a / 2;
    CHECK(div.getSize() == test_size);
    CHECK(div[0][0] == doctest::Approx(5.0)); // Use double literal
    CHECK(div[1][1] == doctest::Approx(a[1][1] / 2.0)); // Use double literal

    CHECK_THROWS_AS(a / 0, invalid_argument); // Divide by 0
}

TEST_CASE("Power operator^") 
{
    cout << endl << endl << endl << "~ Power operator^:" << endl;

    SquareMat m = buildMockMatrix("3\n2 3 4\n5 6 7\n8 9 10");
    int test_size = m.getSize();
    
    SquareMat pow2 = m ^ 2;
    CHECK(pow2.getSize() == test_size);
    CHECK(pow2[0][0] == doctest::Approx(pow(2.0, 2.0))); // Use double literals
    CHECK(pow2[2][2] == doctest::Approx(pow(m[2][2], 2.0))); // Use double literal

    SquareMat pow0 = m ^ 0;
    CHECK(pow0.getSize() == test_size);
    for(int i=0; i<test_size; ++i) {
        for(int j=0; j<test_size; ++j) {
            CHECK(pow0[i][j] == doctest::Approx(1.0)); // Use double literal
        }
    }

    SquareMat pow1 = m ^ 1;
    CHECK(pow1.getSize() == test_size);
    for(int i=0; i<test_size; ++i) {
        for(int j = 0; j < test_size; ++j) {
            CHECK(pow1[i][j] == doctest::Approx(m[i][j])); // Power 1 => original
        }
    }

    SquareMat neg_pow_mat(test_size); // Explicitly construct with size
    fillMat(neg_pow_mat, 2.0); // Use double literal
    SquareMat neg_pow = neg_pow_mat ^ -1;
    CHECK(neg_pow.getSize() == test_size);
    CHECK(neg_pow[0][0] == doctest::Approx(1.0/2.0)); // Use double literals
    CHECK(neg_pow[1][1] == doctest::Approx(1.0/neg_pow_mat[1][1])); // Use operator[]

    SUBCASE("Power of zero base") {
        SquareMat zero_base_mat(test_size); // Explicitly construct with size
        zeroMat(zero_base_mat);
        zero_base_mat[0][0] = 0.0; // Use double literal

        SquareMat result_pos = zero_base_mat ^ 2;
        CHECK(result_pos[0][0] == doctest::Approx(0.0)); // Use double literal

        SquareMat result_zero_pow = zero_base_mat ^ 0;
        CHECK(result_zero_pow[0][0] == doctest::Approx(1.0)); // Use double literal

        SquareMat result_neg = zero_base_mat ^ -2;
        CHECK(isinf(result_neg[0][0]));
    }
}

TEST_CASE("Increment and Decrement operators ++ --") 
{
    cout << endl << endl << endl << "~ Increment and Decrement operators ++ --:" << endl;

    SquareMat a = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat b = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    int test_size = a.getSize();

    SquareMat before = a++;
    CHECK(before.getSize() == test_size);
    CHECK(a.getSize() == test_size);
    CHECK(before[0][0] == doctest::Approx(1.0)); // Postfix returns original value
    CHECK(a[0][0] == doctest::Approx(2.0)); // Original incremented

    SquareMat after = ++b;
    CHECK(after.getSize() == test_size);
    CHECK(b.getSize() == test_size);
    CHECK(after[0][0] == doctest::Approx(2.0)); // Prefix returns incremented value
    CHECK(b[0][0] == doctest::Approx(2.0)); // Original incremented

    SquareMat c(test_size); // Explicitly construct with size
    fillMat(c, 1.0); // Use double literal
    SquareMat c2 = c--;
    CHECK(c2.getSize() == test_size);
    CHECK(c.getSize() == test_size);
    CHECK(c2[0][0] == doctest::Approx(1.0)); // Postfix returns original value
    CHECK(c[0][0] == doctest::Approx(0.0)); // Decremented

    SquareMat d(test_size); // Explicitly construct with size
    fillMat(d, 1.0); // Use double literal
    --d;
    CHECK(d.getSize() == test_size);
    CHECK(d[0][0] == doctest::Approx(0.0)); // Prefix decrement
}

TEST_CASE("Compound operators += -= *= /= %=") 
{
    cout << endl << endl << endl << "~ Compound operators += -= *= /= %=:" << endl;

    SquareMat a = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat b = buildMockMatrix("3\n2 3 4\n5 6 7\n8 9 10");
    int test_size = a.getSize();
    
    a += b;
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(3.0)); // Add and check

    a -= b;
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(1.0)); // Subtract and revert

    SquareMat id(test_size); // Explicitly construct with size
    identityMat(id);
    a *= id; // Matrix multiplication
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(1.0)); // Identity multiplication

    a *= 2; // Scalar multiplication
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(2.0)); // Use double literal

    a /= 2; // Scalar division
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(1.0)); // Use double literal

    fillMat(a, 2.0); // Fill 'a' with new values for element-wise multiplication test
    fillMat(b, 3.0); // Fill 'b' with new values

    // Store original values before the compound assignment modifies 'a'
    double original_a_1_1 = a[1][1];
    double original_b_1_1 = b[1][1];

    a %= b; // Element-wise multiply
    CHECK(a.getSize() == test_size);
    CHECK(a[0][0] == doctest::Approx(2.0 * 3.0)); // Use double literals
    // Corrected CHECK: use original values for expected result
    CHECK(a[1][1] == doctest::Approx(original_a_1_1 * original_b_1_1));

    CHECK_THROWS_AS(a %= 0, invalid_argument); // Invalid scalar mod
}

TEST_CASE("Transpose operator~") 
{
    cout << endl << endl << endl << "~ Transpose operator~:" << endl;

    SquareMat m = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    int test_size = m.getSize();

    SquareMat transposed = ~m;

    CHECK(transposed.getSize() == test_size);
    CHECK(transposed[0][0] == doctest::Approx(1.0)); // Use double literal
    CHECK(transposed[0][1] == doctest::Approx(4.0)); // Use double literal
    CHECK(transposed[0][2] == doctest::Approx(7.0)); // Use double literal
    CHECK(transposed[1][0] == doctest::Approx(2.0)); // Use double literal
    CHECK(transposed[1][1] == doctest::Approx(5.0)); // Use double literal
    CHECK(transposed[1][2] == doctest::Approx(8.0)); // Use double literal
    CHECK(transposed[2][0] == doctest::Approx(3.0)); // Use double literal
    CHECK(transposed[2][1] == doctest::Approx(6.0)); // Use double literal
    CHECK(transposed[2][2] == doctest::Approx(9.0)); // Use double literal

    // Test transpose of a symmetric matrix
    SquareMat sym_mat(test_size); // Explicitly construct with size
    sym_mat[0][0] = 1.0; sym_mat[0][1] = 2.0; sym_mat[0][2] = 0.0; // Use double literals
    sym_mat[1][0] = 2.0; sym_mat[1][1] = 3.0; sym_mat[1][2] = 0.0; // Use double literals
    sym_mat[2][0] = 0.0; sym_mat[2][1] = 0.0; sym_mat[2][2] = 0.0; // Use double literals

    SquareMat transposed_sym = ~sym_mat;
    CHECK(transposed_sym.getSize() == test_size);
    for(int i = 0; i < test_size; ++i) {
        for(int j = 0; j < test_size; ++j) {
            CHECK(transposed_sym[i][j] == doctest::Approx(sym_mat[i][j]));
        }
    }
}

TEST_CASE("Comparison operators == != > < >= <=") 
{
    cout << endl << endl << endl << "~ Comparison operators == != > < >= <=:" << endl;

    SquareMat a = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat b = buildMockMatrix("3\n1 2 3\n4 5 6\n7 8 9");
    SquareMat c = buildMockMatrix("3\n2 3 4\n5 6 7\n8 9 10");
    int test_size = a.getSize();

    CHECK(a == b);
    CHECK_FALSE(a != b);

    CHECK(a != c);
    CHECK_FALSE(a == c);

    CHECK(c > a);
    CHECK_FALSE(a > c);

    CHECK(a < c);
    CHECK_FALSE(c < a);

    CHECK(a <= b);
    CHECK(a <= c);
    CHECK_FALSE(c <= a);

    CHECK(c >= a);
    CHECK(b >= a);
    CHECK_FALSE(a >= c);

    SUBCASE("Comparison with different sized matrices") {
        SquareMat small_mat(2); // Explicitly construct with size
        CHECK_FALSE(a == small_mat);
        CHECK(a != small_mat);
        CHECK_FALSE(a > small_mat);
        CHECK_FALSE(a < small_mat);
        CHECK_FALSE(a >= small_mat);
        CHECK_FALSE(a <= small_mat);
    }
}

TEST_CASE("Boolean Not operator! (Determinant)") 
{
    int test_size = 3;
    SquareMat id(test_size); // Explicitly construct with size
    identityMat(id);

    CHECK(!id == doctest::Approx(1.0)); // Use double literal

    SquareMat zero(test_size); // Explicitly construct with size
    zeroMat(zero);
    CHECK(!zero == doctest::Approx(0.0)); // Use double literal

    SquareMat singular_mat(test_size); // A singular matrix (determinant 0)
    singular_mat[0][0] = 1.0; singular_mat[0][1] = 2.0; singular_mat[0][2] = 3.0; // Use double literals
    singular_mat[1][0] = 2.0; singular_mat[1][1] = 4.0; singular_mat[1][2] = 6.0; // Use double literals
    singular_mat[2][0] = 0.0; singular_mat[2][1] = 0.0; singular_mat[2][2] = 0.0; // Use double literals
    CHECK(!singular_mat == doctest::Approx(0.0)); // Use double literal

    // Test a known 3x3 determinant
    SquareMat det_test_mat(test_size); // Explicitly construct with size
    det_test_mat[0][0] = 6.0; det_test_mat[0][1] = 1.0; det_test_mat[0][2] = 1.0; // Use double literals
    det_test_mat[1][0] = 4.0; det_test_mat[1][1] = -2.0; det_test_mat[1][2] = 5.0; // Use double literals
    det_test_mat[2][0] = 2.0; det_test_mat[2][1] = 8.0; det_test_mat[2][2] = 7.0; // Use double literals
    // Determinant calculation: -306
    CHECK(!det_test_mat == doctest::Approx(-306.0)); // Use double literal

    SUBCASE("Determinant of 1x1 matrix") {
        SquareMat m_1x1(1); // Explicitly construct with size
        m_1x1[0][0] = 5.0;
        CHECK(!m_1x1 == doctest::Approx(5.0));
    }

    SUBCASE("Determinant of 2x2 matrix") {
        SquareMat m_2x2(2); // Explicitly construct with size
        m_2x2[0][0] = 4.0; m_2x2[0][1] = 6.0;
        m_2x2[1][0] = 3.0; m_2x2[1][1] = 8.0;
        // Determinant = (4*8) - (6*3) = 32 - 18 = 14
        CHECK(!m_2x2 == doctest::Approx(14.0));
    }
}

TEST_CASE("Member access operators []") 
{
    cout << endl << endl << endl << "~ Member access operators []:" << endl;

    SquareMat m = buildMockMatrix("3\n10 11 12\n13 14 15\n16 17 18");
    int test_size = m.getSize();

    // Non-const access
    m[0][0] = 100.0; // Use double literal
    CHECK(m[0][0] == doctest::Approx(100.0)); // Use double literal

    // Const access
    const SquareMat& const_m = m;
    CHECK(const_m[0][0] == doctest::Approx(100.0)); // Use double literal
    CHECK(const_m[1][1] == doctest::Approx(10.0 + 1*const_m.getSize() + 1)); // Use double literal

    cout << endl
         << endl;
}

TEST_CASE("NaN and Inf edge behavior") 
{
    int test_size = 3;
    SquareMat a(test_size); // Explicitly construct with size
    nanMat(a); // Fill with NaNs
    SquareMat b = a + a; // Add NaNs
    CHECK(isnan(b[0][0])); // Check NaN propagation

    infMat(a); // Fill with Infs
    b = a * a; // Matrix multiplication with Infs
    CHECK(isinf(b[1][1])); // Check Inf remains

    CHECK_THROWS_AS(a / 0, invalid_argument); // Division by zero (scalar)
}

TEST_CASE("Output Stream operator<<") 
{
    int test_size = 3;
    SquareMat m(test_size); // Explicitly construct with size
    m[0][0] = 1.0; m[0][1] = 2.0; m[0][2] = 3.0;
    m[1][0] = 4.0; m[1][1] = 5.0; m[1][2] = 6.0;
    m[2][0] = 7.0; m[2][1] = 8.0; m[2][2] = 9.0;

    stringstream ss;
    ss << m;

    // Expected output based on SquareMat.cpp's `operator<<` (fixed, setprecision(2), no brackets)
    string expected_output = "1.00 2.00 3.00\n4.00 5.00 6.00\n7.00 8.00 9.00\n";
    CHECK(ss.str() == expected_output);

    // Test with negative numbers and different precision
    SquareMat neg_m(test_size); // Explicitly construct with size
    neg_m[0][0] = -1.234; neg_m[0][1] = 0.0; neg_m[0][2] = -5.67;
    neg_m[1][0] = 10.0; neg_m[1][1] = -0.001; neg_m[1][2] = 1.0;
    neg_m[2][0] = 0.5; neg_m[2][1] = 0.1; neg_m[2][2] = -99.99;

    stringstream ss_neg;
    ss_neg << neg_m;
    expected_output = "-1.23 0.00 -5.67\n10.00 -0.00 1.00\n0.50 0.10 -99.99\n";
    CHECK(ss_neg.str() == expected_output);

    SUBCASE("Output for 0x0 matrix") {
        SquareMat empty_mat(0); // Explicitly construct with size 0
        stringstream ss_empty;
        ss_empty << empty_mat;
        CHECK(ss_empty.str() == "[]"); // As defined in SquareMat.cpp for 0x0
    }
}