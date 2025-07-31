//talgov44@gmail.com

#ifndef SQUARE_MAT_HPP
#define SQUARE_MAT_HPP

#include <iostream>
#include <stdexcept>
#include <cmath>
#include <iomanip>

namespace mat
{
    class SquareMat
    {
        private:
            int size; // The mat's size
            double** mat; // The mat - Pointer to pointers for dynamic 2D array

        public:
            // Constructors and Destructor
            SquareMat(); // Default constructor
            SquareMat(int sizeMat); // Constructor for initializing the mat by a given index
            SquareMat(const SquareMat& secondMat); // Copy constructor
            ~SquareMat(); // Destructor

            SquareMat& operator=(const SquareMat& secondMat); // Assignment operator

            int getSize() const // Getter for the size
            {
                return size;
            }

        private:
            //Helper functions:
            template <typename Func>
            void map(Func func); //Non-const

            template <typename Func>
            void map(Func func) const; //Const

            // Determinant
            void getCofactor(const double* const* src_mat_data, double** temp_mat_data, int p, int q, int n) const;
            double determinantOfMatrix(const double* const* mat_data, int n) const;

        public:
            double sumElements() const; // Elements sum

            // Operator declarations:
            SquareMat operator+(const SquareMat& secondMat) const;
            SquareMat operator-(const SquareMat& secondMat) const;
            SquareMat operator-() const; // Unary negation

            SquareMat operator*(const SquareMat& secondMat) const; // Matrix multiplication
            SquareMat operator*(int scaler) const; // Scalar multiplication

            SquareMat operator%(const SquareMat& secondMat) const; // Element-wise multiplication
            SquareMat operator%(int scaler) const; // Element-wise modulo

            SquareMat operator/(int scaler) const; // Element-wise division
            SquareMat operator^(int scaler) const; // Element-wise power

            SquareMat operator++(); // Prefix increment
            SquareMat operator++(int); // Postfix increment
            SquareMat operator--(); // Prefix decrement
            SquareMat operator--(int); // Postfix decrement

            SquareMat& operator+=(const SquareMat& secondMat);
            SquareMat& operator-=(const SquareMat& secondMat);
            SquareMat& operator*=(const SquareMat& secondMat); // Compound matrix multiplication
            SquareMat& operator*=(int scaler); // Compound scalar multiplication
            SquareMat& operator/=(int scaler);
            SquareMat& operator%=(const SquareMat& secondMat);
            SquareMat& operator%=(int scaler);

            SquareMat operator~() const; //Transpose

            bool operator==(const SquareMat& other) const;
            bool operator!=(const SquareMat& other) const;
            bool operator>(const SquareMat& other) const;
            bool operator<(const SquareMat& other) const;
            bool operator>=(const SquareMat& other) const;
            bool operator<=(const SquareMat& other) const;

            // Member access operators for double array:
            double* operator[](int index);
            const double* operator[](int index) const;

            // Friend function declarations:
            friend std::ostream& operator<<(std::ostream& os, const SquareMat& matrix);
            friend SquareMat operator*(int scaler, const SquareMat& matrix); // Scalar multiplication (scalar * matrix)
            friend double operator!(const SquareMat& matrix); //Determinant (unary!)
    };
}

#endif