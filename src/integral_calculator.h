#ifndef INTEGRAL_CALCULATOR_H
#define INTEGRAL_CALCULATOR_H

#include <functional>
#include <vector>

double monteCarloIntegral(double a, double b, int points,
                          const std::function<double(double)>& func);

double monteCarloMultidimensional(int dimensions,
                                  const std::vector<std::pair<double, double>>& limits, int points,
                                  std::function<double(const std::vector<double>&)> func);

#endif  // INTEGRAL_CALCULATOR_H
