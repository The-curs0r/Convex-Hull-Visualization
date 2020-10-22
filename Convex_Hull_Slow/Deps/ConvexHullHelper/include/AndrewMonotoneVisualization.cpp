#include <vector>
#include <set>
#include <stack>
#include <iostream>
#include <algorithm>
#include <map>
#include "Point.hpp"
#include "AndrewMonotoneVisualization.hpp"
//For orientation and second point function
#include "GrahamScan.hpp"
#include "AndrewMonotone.hpp"

std::vector<Point> inputVerticesAndrew;
std::vector<Point> inputVerticesAndrewCpy;
std::stack<Point> upperHullAndrewMonotone;
std::stack<Point> lowerHullAndrewMonotone;
int doneAndrew = 0;
int doneUpperAndrew = 0;

std::vector<std::pair<double, double> > getCurrentHullAndrewMonotone() 
{
	std::stack<Point> upperHullAndrewMonotoneCpy = upperHullAndrewMonotone;
	std::stack<Point> lowerHullAndrewMonotoneCpy = lowerHullAndrewMonotone;
	std::vector<std::pair<double, double> > hull;
	while (!lowerHullAndrewMonotoneCpy.empty())
	{
		Point current = lowerHullAndrewMonotoneCpy.top();
		hull.push_back(std::make_pair(current.x, current.y));
		lowerHullAndrewMonotoneCpy.pop();
	}
	while (!upperHullAndrewMonotoneCpy.empty())
	{
		Point current = upperHullAndrewMonotoneCpy.top();
		hull.push_back(std::make_pair(current.x, current.y));
		upperHullAndrewMonotoneCpy.pop();
	}
	return hull;
}

bool doneAndrewMonotone() {
	return (bool)doneAndrew;
}

void loadPointsAndrewMonotone(std::set<std::pair<double, double> > inputPoints) 
{
	inputVerticesAndrew.clear();
	inputVerticesAndrewCpy.clear();
	while (!upperHullAndrewMonotone.empty())
		upperHullAndrewMonotone.pop();
	while (!lowerHullAndrewMonotone.empty())
		lowerHullAndrewMonotone.pop();
	doneAndrew = 0;
	
	for (auto point : inputPoints)
	{
		Point current;
		current.x = point.first;
		current.y = point.second;
		inputVerticesAndrew.push_back(current);
	}
	std::sort(inputVerticesAndrew.begin(), inputVerticesAndrew.end(), customSortByCoordinate);
	inputVerticesAndrewCpy = inputVerticesAndrew;
	std::reverse(inputVerticesAndrewCpy.begin(), inputVerticesAndrewCpy.end());
}
void nextIterationAndrewMonotone() 
{
	if (inputVerticesAndrew.empty() && inputVerticesAndrewCpy.empty() || doneAndrew)
	{
		if (!doneAndrew)
			lowerHullAndrewMonotone.pop();
		doneAndrew = 1;
		return;
	}
	if (inputVerticesAndrew.size())
	{
		while (upperHullAndrewMonotone.size() >= 2 && !orientation(secondPoint(upperHullAndrewMonotone), upperHullAndrewMonotone.top(), inputVerticesAndrew[0]))
			upperHullAndrewMonotone.pop();
		upperHullAndrewMonotone.push(inputVerticesAndrew[0]);
		inputVerticesAndrew.erase(inputVerticesAndrew.begin());
		return;
	}
	if (inputVerticesAndrewCpy.size()) 
	{
		if (!doneUpperAndrew)
			upperHullAndrewMonotone.pop();
		while (lowerHullAndrewMonotone.size() >= 2 && !orientation(secondPoint(lowerHullAndrewMonotone), lowerHullAndrewMonotone.top(), inputVerticesAndrewCpy[0]))
			lowerHullAndrewMonotone.pop();
		lowerHullAndrewMonotone.push(inputVerticesAndrewCpy[0]);
		inputVerticesAndrewCpy.erase(inputVerticesAndrewCpy.begin());
		doneUpperAndrew = 1;
		return;
	}
	
}

