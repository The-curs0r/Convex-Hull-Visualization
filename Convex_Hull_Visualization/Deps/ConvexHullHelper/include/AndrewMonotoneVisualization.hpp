#ifndef ANDREWMONOTONEVISUALIZATION_HPP
#define ANDREWMONOTONEVISUALIZATION_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <utility>

std::vector<std::pair<double, double> > getCurrentHullAndrewMonotone();
bool doneAndrewMonotone();
void nextIterationAndrewMonotone();
void loadPointsAndrewMonotone(std::set<std::pair<double, double> > inputPoints);

#endif // !ANDREWMONOTONEVISUALIZATION_HPP