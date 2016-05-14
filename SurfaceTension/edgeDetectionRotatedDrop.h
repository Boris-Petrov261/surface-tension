#ifndef EDGEDETECTIONROTATEDDROP_H_
#define EDGEDETECTIONROTATEDDROP_H_
#include<string>
#include<iostream>
#include<vector>
#include "st_library.h"
vector<Point> readRotatedDrop(string);
int findXBegR(bitmap_image image);
int findXEndR(bitmap_image image);
int findYBegR(bitmap_image image);
int findYEndR(bitmap_image image);


#endif // EDGEDETECTIONROTATEDDROP_H_
