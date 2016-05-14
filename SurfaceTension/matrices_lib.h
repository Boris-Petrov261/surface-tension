#ifndef MATRICES_LIB_H_INCLUDED
#define MATRICES_LIB_H_INCLUDED

#include <iostream>
#include <vector>
#include <cmath>
#include "st_library.h"
using namespace std;
using Matrix = vector<vector<double>>;


double vectorProduct(vector<double> v1, vector<double> v2);
vector<double> multiplyVector(vector<double> v, double scalar);

double vectorMagnitude(vector<double> v);
vector<double> subtractVectors(vector<double> v1, vector<double> v2);
vector<double> calcProjection(vector<double> projOnto, vector<double> toProj);
Matrix transposeMatrix(Matrix A);
void printMatrix(Matrix A);
Matrix multiplyMatrices(Matrix A, Matrix B);
pair<Matrix, Matrix> QRDecomposition(Matrix A);
vector<double> polynomializeGraph(vector<Point>& pts, int order );
vector<Point> generatePolynomial(vector<double> coefficients, double step_size, double a, double b);

#endif // MATRICES_LIB_H_INCLUDED
