#include "matrices_lib.h"


double vectorProduct(vector<double> v1, vector<double> v2)
{
	if (v1.size() != v2.size())
	{
		cerr << "Error: vectors have different lengths, cannot perform operation\n";
		return 0;
	}

	double sum = 0;
	vector<double>::iterator it1 = v1.begin();
	vector<double>::iterator it2 = v2.begin();
	for  (; it1 != v1.end() ;)
	{
		sum += (*it1)*(*it2);
		it1++; it2++;
	}
	return sum;
}

vector<double> multiplyVector(vector<double> v, double scalar)
{
	vector<double> result = v;
	for (auto& i : result)
		i *= scalar;
	return result;
}

double vectorMagnitude(vector<double> v)
{
	double sum = 0;
	for (auto i : v)
		sum += i*i;
	return sqrt(sum);
}

vector<double> subtractVectors(vector<double> v1, vector<double> v2)
{
	if (v1.size() != v2.size())
	{
		cerr << "Error: vectors have different lengths, cannot perform operation\n";
		return vector<double>();
	}

	vector<double> result;
	vector<double>::iterator it1 = v1.begin();
	vector<double>::iterator it2 = v2.begin();
	for (; it1 != v1.end();)
	{
		result.push_back((*it1) - (*it2));
		it1++; it2++;
	}
	return result;
}

//here the vector $toProj is projected onto the vector $projOnto
vector<double> calcProjection(vector<double> projOnto, vector<double> toProj)
{
	return multiplyVector(projOnto, vectorProduct(projOnto, toProj) / vectorProduct(projOnto, projOnto));
}

Matrix transposeMatrix(Matrix A)
{
	Matrix T;

	for (int i = 0; i < A[0].size(); i++)
	{
		vector<double> toAdd;
		for (int j = 0; j < A.size(); j++)
		{
			toAdd.push_back(A[j][i]);
		}
		T.push_back(toAdd);
	}
	return T;
}

void printMatrix(Matrix A)
{
	for (auto i : A)
	{
		for (auto j : i)
			cout << j << " ";
		cout << endl;
	}
}

Matrix multiplyMatrices(Matrix A, Matrix B)
{
	Matrix C;
	A = transposeMatrix(A);

	for (int i = 0; i < A.size(); i++)
	{
		vector<double> toAdd;
		for (int j = 0; j < B.size(); j++)
			toAdd.push_back(vectorProduct(A[i], B[j]));
		C.push_back(toAdd);
	}

	return C;
}

//here A is given as a vector(collection) of its columns rather than its rows
pair<Matrix, Matrix> QRDecomposition(Matrix A)
{
	Matrix Q, R;

	int m = A[0].size();
	int n = A.size();

	for (int i = 0; i < n; i++)
	{
		vector<double> toAdd = A[i];

		for (int j = 0; j < i; j++)
		{
			//cout << "i: " << i << "j: " << j << endl;
			toAdd = subtractVectors(toAdd, calcProjection(Q[j], A[i]));
		}
		//orthonormalize
		toAdd = multiplyVector(toAdd, 1/vectorMagnitude(toAdd));
		Q.push_back(toAdd);
	}

	R = transposeMatrix(multiplyMatrices(transposeMatrix(Q), A));

	return pair<Matrix, Matrix>(Q,R);
}

vector<double> polynomializeGraph(vector<Point>& pts, int order)
{
	int n = pts.size();

	//coefficients of the polynomial that approximates the set of points
	vector<double> coefficients;

	// A*coefficients = resultColumn
	Matrix A;
	vector<double> resultColumn;

	for (int i = 0; i <= order; i++)
	{
		vector<double> column;
		for (int j = 0; j <= order; j++)
			column.push_back(0);
		A.push_back(column);
	}

	A[0][0] = n;

	//fill A
	for (int i = 1; i <= order; i++)
	{
		double sum = 0;
		for (int k = 0; k < n; k++)
			sum += pow(pts[k].x, i);
		int k = i;
		for (int j = 0; j <= i; k--, j++)
			A[k][j] = sum;
	}
	for (int j = 1; j <= order; j++)
	{
		double sum = 0;
		for (int k = 0; k < n; k++)
			sum += pow(pts[k].x, j + order);
		int k = j;
		for (int i = order; k <= order; i--, k++)
			A[k][i] = sum;
	}

	//fill resultColumn
	for (int i = 0; i <= order; i++)
	{
		double sum = 0;
		for (int j = 0; j < n; j++)
			sum += pts[j].y*pow(pts[j].x, i);
		resultColumn.push_back(sum);
	}

	printMatrix(A);

	pair<Matrix, Matrix> qr = QRDecomposition(A);
	Matrix Q = qr.first;
	Matrix R = qr.second;

	Matrix Y;
	Y.push_back(resultColumn);
	printMatrix(Q);
	Y = transposeMatrix(multiplyMatrices(transposeMatrix(Q), Y));
	printMatrix(R);
	cout << "(0,1): " << R[0][1] << "(0,2): " << R[0][2] << endl;
	cout << "(1,1): " << R[1][1] << "(1,2): " << R[1][2] << endl;

	coefficients.push_back(Y[0][order] / R[order][order]);
	printMatrix(Y);

	for (int i = order; i >= 1; i--)
	{
		for (int j = i - 1; j >= 0; j--)
		{
			Y[0][j] -= R[i][j] * coefficients[order-i];
		}
		coefficients.push_back(Y[0][i-1] / R[i-1][i-1]);
	}

	reverse(coefficients.begin(), coefficients.end());

	/*cout << "Q:\n";
	printMatrix(Q);
	cout << "R:\n";
	printMatrix(transposeMatrix(R));*/

	return coefficients;
}

//assuming a < b
//step_size  -  space between two points on the X-axis
//coefficients are ai i=0,1,...n in the polynomial formula given by a0 + a1*x + a2*x^2 + ... + an*x^n
vector<Point> generatePolynomial(vector<double> coefficients, double step_size, double a, double b)
{
	vector<Point> graph;
	int interval_size = abs(b - a);
	for (double i = a; i <= b; i += step_size)
	{
		double y = 0;
		for (int j = 0; j < coefficients.size(); j++)
			y += coefficients[j] * pow(i, j);

		graph.push_back(Point(i, y));
	}

	return graph;
}
