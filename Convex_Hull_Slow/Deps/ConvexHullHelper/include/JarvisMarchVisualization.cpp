#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <chrono>
#include "Point.hpp"
#include <iostream>

#define PI 3.141592 ///<Value of PI i.e. 3.141592

std::vector<Point> inputVertices;
std::vector<Point> currentHull;
Point currentPoint;
int nextPointIndex = 0;
int nextCheckedPointIndex = -1;
int done = 0;
Point leftMostPoint;
Point currentCheckedPoint;

std::vector<std::pair<double, double> > getCurrentHullJarvis() 
{
	std::vector<std::pair<double, double> > hull;
	for (auto point : currentHull)
	{
		hull.push_back(std::make_pair(point.x, point.y));
	}
	return hull;
}
std::vector<std::pair<double, double> > getCurrentCheckJarvis() 
{
	std::vector<std::pair<double, double> > check;
	if (inputVertices.size() >= 3 && !done)
	{
		check.push_back(std::make_pair(currentPoint.x, currentPoint.y));
		check.push_back(std::make_pair(inputVertices[nextPointIndex].x, inputVertices[nextPointIndex].y));
	}
	return check;
}
std::vector<std::pair<double, double> > getCurrentHullLineJarvis()
{
	std::vector<std::pair<double, double> > check;
	if (inputVertices.size() >= 3 && !done)
	{
		check.push_back(std::make_pair(currentPoint.x, currentPoint.y));
		check.push_back(std::make_pair(currentCheckedPoint.x, currentCheckedPoint.y));
	}
	return check;
}
std::pair<double, double> getCurrentHullPointJarvis()
{
	if (!done)
		return std::make_pair(currentPoint.x, currentPoint.y);
	else
		return std::make_pair(NULL, NULL);
}

void loadPointsJarvis(std::set<std::pair<double, double> > inputPoints) 
{
	inputVertices.clear();
	currentHull.clear();
	nextPointIndex = 0;
	nextCheckedPointIndex = -1;
	done = 0;
	for (auto point : inputPoints)
	{
		Point current;
		current.x = point.first;
		current.y = point.second;
		inputVertices.push_back(current);
	}
	int minindex = 0;
	for (int i = 0;i < inputVertices.size();i++)
	{
		if (inputVertices[i].x < inputVertices[minindex].x)
		{
			minindex = i;
		}
	}
	std::swap(inputVertices[0], inputVertices[minindex]);
	currentHull.push_back(inputVertices[0]);
	leftMostPoint = inputVertices[0];
	currentPoint = leftMostPoint;
	if (inputVertices.size() >= 2)
	{
		currentCheckedPoint = inputVertices[1];
		nextCheckedPointIndex = 1;
	}
	if(inputVertices.size()>=3)
		nextPointIndex = 2;
}

void nextIterationJarvis()
{
	if (!done)
	{
		Point checked = currentCheckedPoint - currentPoint;
		Point checking = inputVertices[nextPointIndex];
		Point toChecking = checking - currentPoint;
		double crossProduct = checked.cross(toChecking);
		if (crossProduct < 0)
		{
			currentCheckedPoint = checking;
			nextCheckedPointIndex = nextPointIndex;
		}
		nextPointIndex = nextPointIndex + 1;
		if (nextPointIndex == inputVertices.size()) {
			if (currentCheckedPoint == leftMostPoint) {
				done = 1;
			}
			currentHull.push_back(currentCheckedPoint);
			currentPoint = currentCheckedPoint;
			nextPointIndex = 0;
			inputVertices.erase(inputVertices.begin() + nextCheckedPointIndex);
			currentCheckedPoint = leftMostPoint;
		}
	}
}