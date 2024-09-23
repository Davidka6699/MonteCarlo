/**
 * @file main.cpp
 * @brief Главный файл для выполнения интегрирования методом Монте-Карло с пользовательскими
 * функциями и пределами.
 */

#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "exprtk.hpp"  // Для работы с пользовательскими функциями
#include "gnuplot_i.hpp"
#include "integral_calculator.h"

// Примерные функции
double squareFunction(double x) { return x * x; }

double sinFunction(double x) { return sin(x); }

double expFunction(double x) { return exp(x); }

/**
 * @brief Выбор одномерной функции для интегрирования на основе ввода пользователя.
 *
 * @param chosenFuncIndex Ссылка на переменную для хранения индекса выбранной функции.
 * @param isCustomFunc Ссылка на булеву переменную для указания, выбрана ли пользовательская
 * функция.
 * @param expression Объект exprtk для разбора пользовательских функций.
 * @param x Переменная, используемая для пользовательской функции в exprtk.
 * @return Выбранная функция в виде std::function<double(double)>.
 */
std::function<double(double)> chooseFunction(int& chosenFuncIndex, bool& isCustomFunc,
                                             exprtk::expression<double>& expression, double& x) {
    std::cout << "Choose a function to integrate:\n";
    std::cout << "1: x^2\n";
    std::cout << "2: sin(x)\n";
    std::cout << "3: exp(x)\n";
    std::cout << "4: Custom function\n";  // Пользовательская функция
    std::cin >> chosenFuncIndex;

    if (chosenFuncIndex == 4) {
        isCustomFunc = true;
        std::string expression_string;
        std::cout << "Enter your custom function in terms of x (e.g., x^2 + 3*x): ";
        std::cin >> expression_string;
/////////////////
        exprtk::symbol_table<double> symbol_table;
        symbol_table.add_variable("x", x);
        symbol_table.add_constants();

        expression.register_symbol_table(symbol_table);

        exprtk::parser<double> parser;
        if (!parser.compile(expression_string, expression)) {
            std::cout << "Error: Invalid expression!" << std::endl;
            exit(1);
        }

        return [&expression, &x](double input) -> double {
            x = input;
            return expression.value();
        };////////////////
    }

    switch (chosenFuncIndex) {
        case 1:
            return squareFunction;
        case 2:
            return sinFunction;
        case 3:
            return expFunction;
        default:
            std::cout << "Invalid choice. Defaulting to x^2.\n";
            return squareFunction;
    }
}

/**
 * @brief Получает пределы интегрирования от пользователя.
 *
 * @param a Ссылка на переменную для хранения нижнего предела.
 * @param b Ссылка на переменную для хранения верхнего предела.
 */
void getIntegrationLimits(double& a, double& b) {
    std::cout << "Enter integration limits (a and b):\n";
    std::cin >> a >> b;
}

/**
 * @brief Получает от пользователя количество точек для интегрирования методом Монте-Карло.
 *
 * @return Количество точек для интегрирования.
 */
int getMonteCarloPoints() {
    int points;
    std::cout << "Enter number of points for Monte Carlo integration: ";
    std::cin >> points;
    return points;
}

/**
 * @brief Получает пределы интегрирования для каждого измерения в многомерном интеграле.
 *
 * @param dimensions Количество измерений.
 * @param limits Вектор пар, каждая пара содержит нижний и верхний пределы для каждого измерения.
 */
void getMultidimensionalLimits(int dimensions, std::vector<std::pair<double, double>>& limits) {
    for (int i = 0; i < dimensions; ++i) {
        double lower, upper;
        std::cout << "Enter lower and upper limits for dimension " << i + 1 << ": ";
        std::cin >> lower >> upper;
        limits.push_back(std::make_pair(lower, upper));
    }
}

/**
 * @brief Строит график ошибок для многомерного интегрирования методом Монте-Карло в зависимости от
 * количества точек.
 *
 * @param gp Объект Gnuplot для построения графика.
 * @param func Многомерная функция, которую нужно интегрировать.
 * @param limits Вектор пар, каждая пара содержит нижний и верхний пределы для каждого измерения.
 * @param dimensions Количество измерений.
 * @param maxPoints Максимальное количество точек для построения графика.
 * @param exactResult Точное значение многомерного интеграла для сравнения.
 */
void plotMultidimensionalErrors(Gnuplot& gp,
                                const std::function<double(const std::vector<double>&)>& func,
                                const std::vector<std::pair<double, double>>& limits,
                                int dimensions, int maxPoints, double exactResult) {
    std::vector<double> pointsVec, errors;

    for (int points = 100; points <= maxPoints; points += maxPoints / 20) {
        pointsVec.push_back(points);
        double approx = monteCarloMultidimensional(dimensions, limits, points, func);
        errors.push_back(fabs(approx - exactResult));
    }

    if (pointsVec.back() != maxPoints) {
        pointsVec.push_back(maxPoints);
        double approx = monteCarloMultidimensional(dimensions, limits, maxPoints, func);
        errors.push_back(fabs(approx - exactResult));
    }

    gp.sendCommand("set title 'Multidimensional Monte Carlo Integration Error vs. Points'");
    gp.plotData(pointsVec, errors);
}

/**
 * @brief Строит график ошибки интегрирования методом Монте-Карло в зависимости от количества точек.
 *
 * @param gp Объект Gnuplot для построения графика.
 * @param func Функция, которую нужно интегрировать.
 * @param a Нижний предел интегрирования.
 * @param b Верхний предел интегрирования.
 * @param maxPoints Максимальное количество точек для построения графика.
 * @param exactResult Точное значение интеграла для сравнения.
 */
void plotErrors(Gnuplot& gp, const std::function<double(double)>& func, double a, double b,
                int maxPoints, double exactResult) {
    std::vector<double> pointsVec, errors;

    for (int points = 100; points <= maxPoints; points += maxPoints / 20) {
        pointsVec.push_back(points);
        double approx = monteCarloIntegral(a, b, points, func);
        errors.push_back(fabs(approx - exactResult));
    }

    if (pointsVec.back() != maxPoints) {
        pointsVec.push_back(maxPoints);
        double approx = monteCarloIntegral(a, b, maxPoints, func);
        errors.push_back(fabs(approx - exactResult));
    }

    gp.sendCommand("set title 'Monte Carlo Integration Error vs. Points'");
    gp.plotData(pointsVec, errors);
}

/**
 * @brief Основная функция для выполнения интегрирования методом Монте-Карло.
 * Получает от пользователя метод интегрирования (одномерное или многомерное) и выполняет
 * соответствующее интегрирование.
 *
 * @return int Возвращает 0 при успешном завершении.
 */
int main() {
    std::cout << "Choose integration method:\n";
    std::cout << "1: Standard Monte Carlo Integration\n";
    std::cout << "2: Multidimensional Monte Carlo Integration\n";
    int methodChoice;
    std::cin >> methodChoice;

    if (methodChoice == 1) {
        int chosenFuncIndex;
        bool isCustomFunc = false;
        double x;
        exprtk::expression<double> expression;

        std::function<double(double)> chosenFunc =
            chooseFunction(chosenFuncIndex, isCustomFunc, expression, x);

        double a, b;
        getIntegrationLimits(a, b);

        int points = getMonteCarloPoints();

        double result = monteCarloIntegral(a, b, points, chosenFunc);
        std::cout << "The integral result is: " << result << std::endl;

        Gnuplot gp;
        plotErrors(gp, chosenFunc, a, b, points, result);

    } else if (methodChoice == 2) {
        std::cout << "Enter the number of dimensions for the integration: ";
        int dimensions;
        std::cin >> dimensions;

        std::vector<std::pair<double, double>> limits;
        getMultidimensionalLimits(dimensions, limits);

        int points = getMonteCarloPoints();

        auto multidimensionalFunc = [](const std::vector<double>& point) -> double {
            return sin(point[0]);  // Пример многомерной функции
        };

        double result =
            monteCarloMultidimensional(dimensions, limits, points, multidimensionalFunc);
        std::cout << "The multidimensional integral result is: " << result << std::endl;

        Gnuplot gp;
        plotMultidimensionalErrors(gp, multidimensionalFunc, limits, dimensions, points, result);
    } else {
        std::cout << "Invalid method choice!" << std::endl;
    }

    return 0;
}
