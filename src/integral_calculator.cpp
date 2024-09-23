/**
 * @file integral_calculator.cpp
 * @brief Файл с функциями для выполнения интегрирования методом Монте-Карло, включая многомерное
 * интегрирование.
 */

#include "integral_calculator.h"

#include <cmath>
#include <cstdlib>  // Для использования rand()
#include <functional>
#include <stdexcept>  // Для std::invalid_argument
#include <vector>

/**
 * @brief Выполняет интегрирование методом Монте-Карло для одномерной функции на интервале [a, b].
 *
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param points Количество случайных точек для выборки.
 * @param func Функция для интегрирования. Должна принимать double и возвращать double.
 * @return Оценка значения интеграла.
 * @throws std::invalid_argument если количество точек меньше или равно нулю.
 */
double monteCarloIntegral(double a, double b, int points,
                          const std::function<double(double)>& func) {
    if (points <= 0) {
        throw std::invalid_argument("Количество точек должно быть положительным.");
    }

    double sum = 0.0;
    for (int i = 0; i < points; ++i) {
        double x = a + static_cast<double>(rand()) / RAND_MAX * (b - a);
        sum += func(x);
    }
    return (b - a) * sum / points;
}

/**
 * @brief Выполняет многомерное интегрирование методом Монте-Карло.
 *
 * @param dimensions Количество измерений функции.
 * @param limits Вектор пар, каждая пара содержит нижний и верхний пределы для каждого измерения.
 * @param points Количество случайных точек для выборки.
 * @param func Многомерная функция для интегрирования. Должна принимать вектор значений и возвращать
 * double.
 * @return Оценка значения многомерного интеграла.
 */
double monteCarloMultidimensional(int dimensions,
                                  const std::vector<std::pair<double, double>>& limits, int points,
                                  std::function<double(const std::vector<double>&)> func) {
    std::vector<double> randomPoint(dimensions);
    double volume = 1.0;
    for (const auto& limit : limits) {
        volume *= (limit.second - limit.first);
    }

    double sum = 0.0;
    for (int i = 0; i < points; ++i) {
        for (int d = 0; d < dimensions; ++d) {
            randomPoint[d] = limits[d].first + static_cast<double>(rand()) / RAND_MAX *
                                                   (limits[d].second - limits[d].first);
        }
        sum += func(randomPoint);
    }

    return volume * sum / points;
}
