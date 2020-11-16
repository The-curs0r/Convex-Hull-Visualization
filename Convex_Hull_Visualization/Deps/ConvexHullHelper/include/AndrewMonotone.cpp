#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>
#include "Point.hpp"
#include "AndrewMonotone.hpp"
//For orientation and second point function
#include "GrahamScan.hpp"

bool customSortByCoordinate(const Point& v1, const Point& v2)
{
	return v1.x == v2.x ? v1.y < v2.y : v1.x < v2.x;
}

void andrewMonotone(const std::set<std::pair<double, double> > vertices, std::vector<std::pair<double, double> >& convexHullVertices) 
{
	if (vertices.size() <= 2)
	{
		for (auto point : vertices)
		{
			convexHullVertices.push_back(point);
		}
	}

	std::vector<Point> inputPoints;
	for (auto point : vertices)
	{
		Point current;
		current.x = point.first;
		current.y = point.second;
		inputPoints.push_back(current);
	}
	std::sort(inputPoints.begin(), inputPoints.end(), customSortByCoordinate);
	std::stack<Point> upperHull;
	std::stack<Point> lowerHull;

	for (auto point : inputPoints)
	{
		while (lowerHull.size() >= 2 && orientation(secondPoint(lowerHull), lowerHull.top(), point))
			lowerHull.pop();
		lowerHull.push(point);
	}
	std::reverse(inputPoints.begin(), inputPoints.end());
	for (auto point : inputPoints)
	{
		while (upperHull.size() >= 2 && orientation(secondPoint(upperHull), upperHull.top(), point))
			upperHull.pop();
		upperHull.push(point);
	}
	lowerHull.pop();
	upperHull.pop();
	while (!upperHull.empty())
	{
		Point current = upperHull.top();
		convexHullVertices.push_back(std::make_pair(current.x, current.y));
		upperHull.pop();
	}
	while (!lowerHull.empty())
	{
		Point current = lowerHull.top();
		convexHullVertices.push_back(std::make_pair(current.x, current.y));
		lowerHull.pop();
	}
	return;
}