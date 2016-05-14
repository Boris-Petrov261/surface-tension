#include "edgeDetectionPendantDrop.h"
#include <fstream>
#include <cmath>
#define TRESHHOLD 120
using namespace std;
vector<Point> readPendantDrop(string FILENAME_INPUT)
{
    vector <Point> coords;
    unsigned char red , green , blue;
    string EXP_DIRECTORY = "res\\pics\\EXP_PROFILES\\";
    string DER_DIRECTORY = "res\\pics\\DER_PROFILES\\";
    bitmap_image image(EXP_DIRECTORY + FILENAME_INPUT);
    // xBeg , xEnd , yEnd
    int xBeg , xEnd , yEnd;
    //find horizontal and vertical extrema
    xBeg = findXBegP(image);
    xEnd = findXEndP(image);
    yEnd = findYEndP(image);
    //find coordinates
    for(int h = 1 ; h < yEnd ; h ++)
    {
        for(int w = xEnd ; w > (xEnd + xBeg) / 2 ; w --)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= TRESHHOLD && (int)green <= TRESHHOLD && (int)blue <= TRESHHOLD)
            {
                image.set_pixel(w,h,255,0,0);
                Point tmp(w , h);//creates a temporary point to be inserted in the vector
                coords.push_back(tmp);//temporary point is inserted in the vector
                break;
            }
        }
    }
    image.save_image(DER_DIRECTORY + FILENAME_INPUT);
    //remove unwanted coordinates
    Point tmpPoint = coords.at(0);
    for(auto& it : coords)
    {
        if(abs(tmpPoint.x - it.x) <= 30)
            coords.erase(coords.begin());
        else
            break;
    }

    //reverses the vector
    reverseVector(coords);
    //resizes the coordinates
    for(auto& it : coords)
    {
        it.x = (it.x - ((xEnd + xBeg)/2))*(1.0)/((xEnd - xBeg)/2);
        it.y = (yEnd - it.y)*(1.0)/((xEnd - xBeg)/2);
    }
    coords.insert(coords.begin() , Point(0,0));
    return coords;
}
int findXBegP(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int w = 1 ; w < image.width() ; w ++)
        for(int h = 1 ; h < image.height() ; h ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= TRESHHOLD && (int)green <= TRESHHOLD && (int)blue <= TRESHHOLD)
                return w;
        }
}
int findXEndP(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int w = image.width() - 1 ; w > 0  ; w --)
        for(int h = 1 ; h < image.height() ; h ++)
        {
            image.get_pixel(w , h , red , green , blue);
            if((int)red <= TRESHHOLD && (int)green <= TRESHHOLD && (int)blue <= TRESHHOLD)
                return w;
        }
}
int findYEndP(bitmap_image image)
{
    unsigned char red , green , blue;
    for(int h = image.height() - 1 ; h > 0 ; h --)
    {
        for(int w = 1 ; w < image.width() ; w ++) {
            image.get_pixel(w,h,red,green,blue);
            //image.set_pixel(w,h,0,0,255);
            if((int)red <= TRESHHOLD && (int)green <= TRESHHOLD && (int)blue <= TRESHHOLD)
                return h;
        }
    }
}
void reverseVector(vector<Point> & coords)
{
    vector<Point> tmpVector;
    for(vector<Point>::iterator it = coords.begin() ; it != coords.end() ; it ++)
    {
        if(it == (coords.begin()))
            tmpVector.push_back(*it);
        else
            tmpVector.insert(tmpVector.begin() , *it);
    }
    coords = tmpVector;
}
