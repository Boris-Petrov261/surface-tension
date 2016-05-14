#ifndef SMOOTHER_H_
#define SMOOTHER_H_
#include "st_library.h"
Point findExtr(vector<Point> &);
int findExtrInd(vector<Point> &);
void removeUnwantedPoints(vector<Point> &);
void smoothing(vector<Point> &);
bool isExtr(vector<Point>::iterator & , Point &);
double distBetweenTwoIts(vector<Point>::iterator & , vector<Point>::iterator &);
#endif // SMOOTHER_H_
