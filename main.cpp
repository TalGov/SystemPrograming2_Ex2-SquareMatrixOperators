//talgov44@gmail.com

#include "SquareMat.hpp"

#include <iostream>

using namespace mat;
using namespace std;

int main()
{
    // Setting the matrixes (with the default constructor):
    cout << "---Mat One" << endl;
    SquareMat matOne = SquareMat();
    cout << endl << "---Mat Two:" << endl;
    SquareMat matTwo = SquareMat();
    SquareMat resultMat(matOne.getSize());

    resultMat = matOne + matTwo;
    cout << endl << endl << "matOne + matTwo:" << endl << resultMat << endl;

    resultMat = matOne - matTwo;
    cout << "matOne - matTwo:" << endl << resultMat << endl;

    resultMat = -matOne;
    cout << "-matOne:" << endl << resultMat << endl;

    resultMat = matOne * matTwo;
    cout << "matOne * matTwo:" << endl << resultMat << endl;

    resultMat = matOne * 2;
    cout << "matOne * 2:" << endl << resultMat << endl;

    resultMat = matOne % matTwo;
    cout << "matOne % matTwo:" << endl << resultMat << endl;

    resultMat = matOne % 2;
    cout << "matOne % 2:" << endl << resultMat << endl;

    resultMat = matOne / 2;
    cout << "matOne / 2:" << endl << resultMat << endl;

    resultMat = matOne ^ 2;
    cout << "matOne ^ 2:" << endl << resultMat << endl;

    matOne++;
    cout << "matOne++:" << endl << matOne << endl;

    matOne--;
    cout << "matOne--:" << endl << matOne << endl;

    resultMat = ~matOne;
    cout << "~matOne:" << endl << resultMat << endl;

    cout << "matOne == matTwo: " << (matOne == matTwo) << endl;
    cout << "matOne < matTwo: " << (matOne < matTwo) << endl << endl;

    matOne += matTwo;
    cout << "matOne += matTwo:" << endl << matOne << endl;

    matOne -= matTwo;
    cout << "matOne -= matTwo:" << endl << matOne << endl;

    cout << "!matOne: " << !matOne << endl;

    return 0;
}