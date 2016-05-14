#include <cstdlib>
#include <math.h>
#include <GL/glfw.h>
#include "okglib.h"
#include "st_library.h"
#include "matrices_lib.h"
#include "edgeDetectionPendantDrop.h"
#include "edgeDetectionRotatedDrop.h"
#include "smoother.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<windows.h>
#define M_PI 3.141592
#define SCALE 1.0f
using namespace std;

void readFromFile(vector<Point> &ls , string FILENAME)
{
    ifstream iFile;
    iFile.open(FILENAME);
    while(!iFile.eof())
    {
        Point p;
        iFile >> p.x;
        iFile >> p.y;
        ls.push_back(p);
    }
}
void drawLine(Point p1 , Point p2)
{

    glBegin(GL_POINTS);
        glVertex3d(p1.x , 0 , p1.y);
        glVertex3d(p2.x , 0 , p2.y);
    glEnd();
}
void drawvector(vector<Point> &ls)
{
    vector<Point>::iterator it = ls.begin();
    for(int i = 0 ; i < ls.size() - 1 ; i++) {
        vector<Point>::iterator it2 = it;
        it2++;
        drawLine(*it , *it2);
        it++;
    }
}
void drawFunction(vector<Point> &ls)
{
    glPointSize(1);
    //float SCALE = 4.0f;
    glPushMatrix();
        glScaled(SCALE,SCALE,SCALE);
        drawvector(ls);
    glPopMatrix();
  /*  glPushMatrix();
        glRotatef(180,0,0,1);
        glScaled(SCALE,SCALE,SCALE);
        drawvector(ls);
    glPopMatrix();
    glLineWidth(1);*/

}
int main()
{

    init();

    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );
    string EXP_path , DER_path;
    EXP_path = "res\\text\\experimentalDrops\\";
    DER_path = "res\\text\\derivedDrops\\";
    vector<Point> ls;
    readFromFile(ls , EXP_path + "EXP_PEN_(0.5424)_(2.9).txt");
    vector<Point> ls2;
    readFromFile(ls2 , EXP_path + "EXP_PEN_(0.635)_(2.4).txt");
    vector<Point> ls3;
    readFromFile(ls3 , EXP_path + "EXP_PEN_(0.4844)_(2.4).txt");
    vector<Point> ls4;
    readFromFile(ls4 , EXP_path + "EXP_ROT_(0.6647)_(1.873).txt");
    vector<Point> ls5;
    readFromFile(ls5 , EXP_path + "EXP_ROT_(0.5192)_(4.214).txt");

    vector<Point> experimentalPoints;
    experimentalPoints = readRotatedDrop("EXP_ROTATED_2.bmp");
    removeUnwantedPoints(experimentalPoints);
    smoothing(experimentalPoints);

    vector<double> coefficientsXPolynomial, coefficientsYPolynomial;
    vector<Point> curveXCoords, curveYCoords;

    vector<Point> xAxisData, yAxisData;
    double currentCurveLength = 0;
    Point prev = experimentalPoints[0];
    for(auto it = experimentalPoints.begin()+1; it != experimentalPoints.end(); ++it)
    {
        xAxisData.push_back(Point(currentCurveLength, (*it).x));
        yAxisData.push_back(Point(currentCurveLength, (*it).y));
        currentCurveLength += dist(prev, (*it));
        cout << currentCurveLength << endl;
        prev = (*it);
    }

    coefficientsXPolynomial = polynomializeGraph(xAxisData, 5);
    coefficientsYPolynomial = polynomializeGraph(yAxisData, 5);
    curveXCoords = generatePolynomial(coefficientsXPolynomial, 0.01, 0, currentCurveLength);
    curveYCoords = generatePolynomial(coefficientsYPolynomial, 0.01, 0, currentCurveLength);

    cout << "CoeffX: " << coefficientsXPolynomial[0]<<endl;
    cout << "CoeffY: " << coefficientsYPolynomial[0]<<endl;

    vector<Point> smoothGraph;
    double coeffX0 = coefficientsXPolynomial[0], coeffY0 = coefficientsYPolynomial[0];
    for(int i = 0; i< curveXCoords.size(); i++)
        smoothGraph.push_back(Point(curveXCoords[i].y - coeffX0, curveYCoords[i].y - coeffY0));

    {
        auto it = smoothGraph.begin();
        Point prev = (*it);
        it++;

        for (; it != smoothGraph.end(); ++it)
        {
            (*it).fi = atan(((*it).y - prev.y) / (((*it).x) - prev.x));
            prev = (*it);
        }
    }
    smoothGraph.erase(smoothGraph.begin());
    //Vector2D identifiedParams = identifyParams(EXP_path + "EXP_ROT_(0.6647)_(1.873).txt", "asd.txt", drop_type::ROTATED);
    vector<Point> test1 = createTheoreticalModel(findInitialValues(smoothGraph, drop_type::ROTATED), drop_type::ROTATED, Vector2D(0,4));


    while( running() )
    {
        glClear( GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT );
        glClearColor(1, 1, 1,1.0f);
        glColor3f(0,0,0);
        glPushMatrix();
        glTranslatef(0.0f,0.0f, -3.0f);
        drawOxyz();
        //First Pendant
        //glColor3f(1,0,0);drawFunction(ls);
        //Second Pendant
        //glColor3f(0,1,0);drawFunction(ls2);
        //Third Pendant
        //glColor3f(0,0,1);drawFunction(ls3);
        //First Rotated
        //glColor3f(1,0,0);drawFunction(ls4);
        //Second Rotated
        //glColor3f(1,0,0);drawFunction(ls5);

        glColor3f(0,1,0);drawFunction(experimentalPoints);
        glColor3f(1,0,0);drawFunction(smoothGraph);
        glColor3f(0,0,1);drawFunction(test1);
        glfwSwapBuffers();
        glPopMatrix();
    }

    finit();
    return 0;
}
