#include "st_library.h"
#include "matrices_lib.h"
#include "edgeDetectionPendantDrop.h"
#include "edgeDetectionRotatedDrop.h"
const double ERROR = 0.003;
const double STEP = 0.009; //used for precision in deepest descent algorithm
const double TAU = 0.01;

double dist(Point first, Point second)
{
    return sqrt((second.x - first.x)*(second.x - first.x) + (second.y - first.y)*(second.y - first.y));
}
double dist(vector<Point> experimentalPts, Point p)
{
	//min_X is the smallest point in the experimental points for which min_X.x >= p.x
	//max_X is the largest point in the experimental points for which max_X.x <= p.x
	Point min_X, max_X;
	min_X = max_X = experimentalPts.front().x;
	for (auto point : experimentalPts)
	{
		if (point.x > p.x)
		{
			if (abs(min_X.x - p.x) > abs(point.x - p.x))
				min_X = point;
		}
		else if (abs(max_X.x - p.x) > abs(point.x - p.x))
			max_X = point;
	}

	double distance = 0;
	if (min_X.x != max_X.x)
	{
		Point& p1 = max_X;
		Point& p2 = min_X;

		distance = abs((p2.x - p1.x)*(p1.y - p.y) - (p1.x - p.x)*(p2.y - p1.y)) / sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
	}
	else
	{
		distance = sqrt((min_X.x - p.x)*(min_X.x - p.x) + (min_X.y - p.y)*(min_X.y - p.y));
	}

	return distance;
}

double errorFunct(vector<Point> experimentalPts, vector<Point> theoreticalPts)
{
	double result = 0;
	for (auto p : theoreticalPts)
	{
		double di = dist(experimentalPts, p);
		result += di*di;
	}

	return result;
}

//create the theoretical model of a drop based on given values (b,c)
vector<Point> createTheoreticalModel(Vector2D params, drop_type dropType, Vector2D dimLimits)
{
	vector<Point> result;
	double b = params.x,
		c = params.y;

	Point zero;
	zero.x = 0;
	zero.y = 0;
	zero.fi = 0;
	result.push_back(zero);

	Point first;
	first.x = TAU*cos(0);
	first.y = TAU*sin(0);
	first.fi = TAU*(2 * b);
	result.push_back(first);
    Point newPoint;
    int counter = 0;
	while(newPoint.y < dimLimits.y && counter < 600 && newPoint.x >= 0)
	{
		Point tmp = result.back();
		newPoint.x = tmp.x + TAU * cos(tmp.fi);
		newPoint.y = tmp.y + TAU * sin(tmp.fi);

		if(dropType == drop_type::PENDANT)
            newPoint.fi = tmp.fi + TAU * (2 * b + c * tmp.y - sin(tmp.fi) / tmp.x);
        else //ROTATED case
            newPoint.fi = tmp.fi + TAU * (2 * b + c * tmp.x * tmp.x - sin(tmp.fi) / tmp.x);

		result.push_back(newPoint);
		counter++;
	}

	return result;
}

double getMaximumXValue(vector<Point> pts)
{
    double currentMax = -10000;

    for(auto p : pts)
    {
        if(p.x > currentMax)
            currentMax = p.x;
    }

    //based on experience this is a more appropriate value
    currentMax = currentMax - 0.09;

    return currentMax;
}

double getMaximumYValue(vector<Point> pts)
{
    double currentMax = -10000;

    for(auto p : pts)
    {
        if(p.y > currentMax)
            currentMax = p.y;
    }


    return currentMax;
}

//from the experimental model we derive some initial values b,c
Vector2D findInitialValues(vector<Point> experimentalPoints, drop_type dropType)
{
    //we are looking for the solution X = (b,c)
    //given by the equation AX = B
    Matrix A, X, B;

    //if point is rotated only use the points up until the first inflexion point
    if(dropType == drop_type::ROTATED)
    {
        double inflexMax = getMaximumXValue(experimentalPoints);
        for(auto it = experimentalPoints.begin(); it != experimentalPoints.end(); ++it)
        {
            if((*it).x > inflexMax)
            {
                experimentalPoints.erase(it, experimentalPoints.end());
                break;
            }
        }
    }

    //add first column vector of A
    vector<double> toAdd;
    for(int i = 0; i < experimentalPoints.size()-1; i++)
        toAdd.push_back(2*TAU);
    A.push_back(toAdd);

    //add second column vector of A
    toAdd = vector<double>();
    if(dropType == drop_type::PENDANT)
    {
        for(auto p : experimentalPoints)
            toAdd.push_back(TAU*p.y);
    }
    else //ROTATED case
    {
        for(auto p : experimentalPoints)
            toAdd.push_back(TAU*p.x*p.x);
    }
    toAdd.pop_back();
    A.push_back(toAdd);

    //this is B - the value vector
    vector<double> valueVector;
    auto it1 = experimentalPoints.begin();
    auto it2 = it1; it2++;
    for(; it2!= experimentalPoints.end(); ++it1, ++it2)
    {
        valueVector.push_back((*it2).fi - (*it1).fi + TAU*sin((*it1).fi)/((*it1).x));
    }
    B.push_back(valueVector);

    //  cout << A[0].size() << " " << A[1].size() << " " << B[0].size() << endl;

    pair<Matrix, Matrix> qr = QRDecomposition(A);

    Matrix Q = qr.first;
    Matrix R = qr.second;

    printMatrix(multiplyMatrices(transposeMatrix(Q),B));
    printMatrix(R);

    Matrix rightSide = multiplyMatrices(transposeMatrix(Q),B);

    double c = rightSide[1][0]/R[1][1];
    double b = (rightSide[0][0] - R[1][0]*c)/R[0][0];

    cout << "b: " << b << " c: " << c << endl;

    //shit niggà what are you doing
    if(dropType == drop_type::ROTATED)
        c += 0.074*c;

    return Vector2D(b,c);


}

Vector2D calcGradient(vector<Point> experimentalPts, Vector2D params, drop_type dropType, Vector2D dimLimits)
{
	double xCoord = (errorFunct(experimentalPts, createTheoreticalModel(Vector2D(params.x + 0.001, params.y), dropType, dimLimits)) - errorFunct(experimentalPts, createTheoreticalModel(params, dropType, dimLimits))) / 0.001;
	double yCoord = (errorFunct(experimentalPts, createTheoreticalModel(Vector2D(params.x, params.y + 0.001), dropType, dimLimits)) - errorFunct(experimentalPts, createTheoreticalModel(params, dropType, dimLimits))) / 0.001;
	cout << "Gradient: " << xCoord << " " << yCoord << endl;

	Vector2D gradient(xCoord, yCoord);
	if(gradient.magnitude()>1)
        return gradient.normalizedVector();

    return gradient;
}

//at this point we have some initial values b,c (params)
//we try to find more precise values to better fit the experimental model
Vector2D preciseError(Vector2D params, vector<Point> experimentalPts, drop_type dropType, Vector2D dimLimits)
{
	cout << "Testing for vals: " << params.x << " " << params.y << endl;
	vector<Point> theoreticalPts = createTheoreticalModel(params, dropType, dimLimits);
	cout << "Error is: " << errorFunct(experimentalPts, theoreticalPts) << endl;

	if (errorFunct(experimentalPts, theoreticalPts) < ERROR)
	{
		cout << "Last vals: " << params.x << params.y << endl;
		Vector2D result(params.x, params.y);
		return result;
	}
	else
	{
		params = params - STEP*calcGradient(experimentalPts, params, dropType, dimLimits);
		return preciseError(params, experimentalPts, dropType, dimLimits);
	}

}

Vector2D identifyParams(string fileInput, string fileOutput, drop_type dropType)
{
    ifstream coords(fileInput);

	vector<Point> experimentalPoints;

  /*  if(dropType == drop_type::PENDANT)
        experimentalPoints = readPendantDrop(fileInput);
    else
        experimentalPoints = readRotatedDrop(fileInput);*/

	experimentalPoints.push_back(Point(0,0,0));

	//add points to list
	while (!coords.eof())
	{
		Point p;
		coords >> p;
		experimentalPoints.push_back(p);
	}

	auto it = experimentalPoints.begin();
	Point prev = (*it);
	it++;

	for (; it != experimentalPoints.end(); ++it)
	{
		(*it).fi = atan(((*it).y - prev.y) / (((*it).x) - prev.x));
		prev = (*it);
	}

     ofstream outputPoints(fileOutput);
    for(auto p : experimentalPoints)
        outputPoints << p.x << " " << p.y << " " << p.fi<< endl;
	experimentalPoints.erase(experimentalPoints.begin()); // delete (0,0,0) point

	Vector2D dimLimits(0, getMaximumYValue(experimentalPoints));

    Vector2D initialGuess = findInitialValues(experimentalPoints, dropType);
    Vector2D realParams = preciseError(initialGuess, experimentalPoints, dropType, dimLimits);


    cout << "Derived parameters: " << "\nb: " << realParams.x << "\nc: " << realParams.y;



    return realParams;
}

void drawToBMP(vector<Point> & points , string FILENAME)
{

}

void inverseGraph(vector<Point> & points , Point & reversePoint)
{
    reversePoint = points[0];
    for(auto & p : points)
    {
        p.x = abs(reversePoint.x - p.x);
        p.y = abs(reversePoint.y - p.y);
    }
    for(auto & p : points)
        swap(p.x , p.y);
}
void reverseInverseGraph(vector<Point> & points , Point & reversePoint)
{
    for(auto & p : points)
        swap(p.x , p.y);
    for(auto & p : points)
    {
        p.x = (reversePoint.x - p.x);
        p.y = (reversePoint.y + p.y);
    }

}

