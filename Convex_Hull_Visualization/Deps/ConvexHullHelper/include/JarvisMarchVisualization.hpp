#ifndef JARVISMARCHVISUALIZATION_HPP
#define JARVISMARCHVISUALIZATION_HPP

#include <vector>
#include <algorithm>
#include <set>
#include <utility>

std::vector<std::pair<double, double> > getCurrentHullJarvis();
std::vector<std::pair<double, double> > getCurrentCheckJarvis();
std::vector<std::pair<double, double> > getCurrentHullLineJarvis();
std::pair<double, double> getCurrentHullPointJarvis();
void nextIterationJarvis();
void loadPointsJarvis(std::set<std::pair<double, double> > inputPoints);

#endif // !JARVISMARCHVISUALIZATION_HPP
