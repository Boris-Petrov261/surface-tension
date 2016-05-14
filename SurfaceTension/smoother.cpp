#include "smoother.h"
Point findExtr(vector<Point> & points)
{
    //vector<Point> extrs;
    for(auto& p : points)
        if(p.x == 1)
            return p;
    //return extrs.at(extrs.size() / 2);

}
int findExtrInd(vector<Point> & points)
{
    int br = 0;
    for(auto& p : points)
    {
        if(p.x == 1)
            break;
        br++;
    }
    return br;
}
void removeUnwantedPoints(vector<Point> & points)
{
    Point Extr = findExtr(points);
    cout << "is nice" << endl;
    // pre Extremum
    // we need only the points that are before the Extremum
    vector<Point>::iterator current = points.begin();
    /*while(!isExtr(current , Extr))
    {
        vector<Point>::iterator next = current + 1;
        while(!isExtr(next , Extr) && (*next).x <= (*current).x)
        {
            points.erase(next);
            next = current + 1;
        }
        current ++;
    }*/
    while(current != points.end())
    {
        vector<Point>::iterator next = current + 1;
        if(next == points.end())
            break;
        while((*next).x == (*current).x)
        {
            points.erase(next);
            next = current + 1;
            if(next == points.end())
                break;
        }
        if(next == points.end())
            break;
        current ++;
    }
}
void smoothing(vector<Point> & points)
{
    Point Extr = findExtr(points);
    vector<Point>::iterator current = points.begin();
    while(current != points.end())
    {
        vector<Point>::iterator next = current + 1;
        if(next == points.end())
            break;
        int numberOfPoints = distBetweenTwoIts(current , next) / 0.01;
        double xDist , yDist;
        xDist = (*next).x - (*current).x;
        yDist = (*next).y - (*current).y;
        for(int i = 1 ; i < numberOfPoints ; i ++)
        {
            Point tmp((*current).x + i * xDist/numberOfPoints , (*current).y + i * yDist/numberOfPoints);
            points.insert(current + i , tmp);
        }
        current = next;
    }

}
bool isExtr(vector<Point>::iterator & it , Point & p)
{
    return ((*it).x == p.x );
}
double distBetweenTwoIts(vector<Point>::iterator & current ,
                         vector<Point>::iterator & next)
{
    return sqrt(    pow(    (*current).x - (*next).x   , 2) + pow(  (*current).y - (*next).y   , 2)    );
}
