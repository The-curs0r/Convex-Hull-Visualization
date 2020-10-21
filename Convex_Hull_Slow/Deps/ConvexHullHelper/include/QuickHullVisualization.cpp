#include <set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <vector>
#include <stack>
#include <chrono>
#include "Point.hpp"
#include "QuickHull.hpp"
#include <iostream>

#define PI 3.141592

std::vector<Point>  inputVerticesQuick;
std::vector<Point>  currentHullQuick;

struct quickHullHelper
{
	std::vector<Point> inputPoints;
	Point leftEndpoint;
	Point rightEndpoint;
	Point farthestPoint;
	int stage;
};

std::stack<quickHullHelper>  quickStack;
std::stack<quickHullHelper>  quickStackRight;

int doneQuick = 0;
int doneQuickLeft = 0;

Point rightMostQuick;

bool doneLeft()
{
	return (bool)doneQuickLeft;
}

bool doneRight()
{
	return (bool)doneQuick;
}

std::pair<double, double> rightMost() {
	return std::make_pair(rightMostQuick.x, rightMostQuick.y);
}

std::vector<std::pair<double, double> > getCurrentHullQuickHull()
{
	std::vector<std::pair<double, double> > hull;
	for (auto point : currentHullQuick)
	{
		hull.push_back(std::make_pair(point.x, point.y));
	}
	return hull;
}

void loadPointsQuickHull(std::set<std::pair<double, double> > inputPoints)
{
	inputVerticesQuick.clear();
	doneQuick = 0;
	doneQuickLeft = 0;
	currentHullQuick.clear();
	while (!quickStack.empty())
		quickStack.pop();
	while (!quickStackRight.empty())
		quickStackRight.pop();
	for (auto point : inputPoints)
	{
		Point current;
		current.x = point.first;
		current.y = point.second;
		inputVerticesQuick.push_back(current);
	}
	//Check points > 2
	Point leftMostQuick = *std::min_element(inputVerticesQuick.begin(), inputVerticesQuick.end(), isToTheleft);
	rightMostQuick = *std::max_element(inputVerticesQuick.begin(), inputVerticesQuick.end(), isToTheleft);

	std::vector<Point> left;
	std::vector<Point> right;

	for (auto p : inputVerticesQuick) {
		antiClockwise(leftMostQuick, rightMostQuick, p) > 0 ? left.push_back(p) : right.push_back(p);
	}

	currentHullQuick.push_back(leftMostQuick);

	quickHullHelper current;
	current.inputPoints = left;
	current.leftEndpoint = leftMostQuick;
	current.rightEndpoint = rightMostQuick;
	current.stage = 0;
	quickStack.push(current);

	current.inputPoints = right;
	current.leftEndpoint = rightMostQuick;
	current.rightEndpoint = leftMostQuick;
	current.stage = 0;
	quickStackRight.push(current);


	return;
}

void nextIterationQuickHull()
{
	if ((quickStack.empty() && quickStackRight.empty()) || doneQuick) {
		doneQuick = 1;
		return;
	}
	if (quickStack.empty() || doneQuickLeft) {
		if (!doneQuickLeft) {
			currentHullQuick.push_back(rightMostQuick);
			doneQuickLeft = 1;
			return;
		}
		quickHullHelper current = quickStackRight.top();
		quickStackRight.pop();
		switch (current.stage)
		{
		case 0:
		{

			current.stage = 1;

			//currentHullQuick.push_back(current.leftEndpoint);
			if (current.inputPoints.size())
			{
				Point farthestPoint = current.inputPoints[getFarthest(current.leftEndpoint, current.rightEndpoint, current.inputPoints)];
				current.farthestPoint = farthestPoint;
				quickStackRight.push(current);

				quickHullHelper currentnew;
				for (auto p : current.inputPoints) {
					if (notEqual(farthestPoint, p) && notEqual(current.leftEndpoint, p) && antiClockwise(current.leftEndpoint, farthestPoint, p) > 0) {
						currentnew.inputPoints.push_back(p);
					}
				}
				currentnew.leftEndpoint = current.leftEndpoint;
				currentnew.rightEndpoint = farthestPoint;
				currentnew.stage = 0;

				quickStackRight.push(currentnew);
			}



			break;
		}
		case 1:
		{
			currentHullQuick.push_back(current.farthestPoint);
			current.stage = 2;
			quickStackRight.push(current);
			break;
		}
		case 2:
			//current.stage = 3;
			//quickStack.push(current);

			//currentHullQuick.push_back(current.leftEndpoint);
			if (current.inputPoints.size())
			{
				quickHullHelper currentnew;
				for (auto p : current.inputPoints) {
					if (antiClockwise(current.farthestPoint, current.rightEndpoint, p) > 0) {
						currentnew.inputPoints.push_back(p);
					}
				}
				currentnew.leftEndpoint = current.farthestPoint;
				currentnew.rightEndpoint = current.rightEndpoint;
				currentnew.stage = 0;
				quickStackRight.push(currentnew);
			}
			break;
		}
		return;
	}
	quickHullHelper current = quickStack.top();
	quickStack.pop();
	switch (current.stage)
	{
	case 0:
	{

		current.stage = 1;

		//currentHullQuick.push_back(current.leftEndpoint);
		if (current.inputPoints.size())
		{
			Point farthestPoint = current.inputPoints[getFarthest(current.leftEndpoint, current.rightEndpoint, current.inputPoints)];
			current.farthestPoint = farthestPoint;
			quickStack.push(current);

			quickHullHelper currentnew;
			for (auto p : current.inputPoints) {
				if (notEqual(farthestPoint, p) && notEqual(current.leftEndpoint, p) && antiClockwise(current.leftEndpoint, farthestPoint, p) > 0) {
					currentnew.inputPoints.push_back(p);
				}
			}
			currentnew.leftEndpoint = current.leftEndpoint;
			currentnew.rightEndpoint = farthestPoint;
			currentnew.stage = 0;

			quickStack.push(currentnew);
		}

		

		break;
	}
	case 1:
	{
		currentHullQuick.push_back(current.farthestPoint);
		current.stage = 2;
		quickStack.push(current);
		break;
	}
	case 2:
		//current.stage = 3;
		//quickStack.push(current);

		//currentHullQuick.push_back(current.leftEndpoint);
		if (current.inputPoints.size())
		{
			quickHullHelper currentnew;
			for (auto p : current.inputPoints) {
				if (antiClockwise(current.farthestPoint, current.rightEndpoint, p) > 0) {
					currentnew.inputPoints.push_back(p);
				}
			}
			currentnew.leftEndpoint = current.farthestPoint;
			currentnew.rightEndpoint = current.rightEndpoint;
			currentnew.stage = 0;
			quickStack.push(currentnew);
		}
		break;
	}
}