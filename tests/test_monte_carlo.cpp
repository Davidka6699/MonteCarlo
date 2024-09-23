#define BOOST_TEST_MODULE MonteCarloTests
#include <boost/test/included/unit_test.hpp>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <vector>

#include "functions.h"
#include "integral_calculator.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Тестирование функции x^2
BOOST_AUTO_TEST_CASE(test_square_function) {
    double result = monteCarloIntegral(0.0, 1.0, 10000, [](double x) { return x * x; });
    double exact = 1.0 / 3.0;
    BOOST_CHECK_SMALL(result - exact, 0.01);  // Разница должна быть маленькой
}

// Тестирование функции sin(x)
BOOST_AUTO_TEST_CASE(test_sin_function) {
    double result = monteCarloIntegral(0.0, M_PI, 10000, [](double x) { return sin(x); });
    double exact = 2.0;
    BOOST_CHECK_SMALL(result - exact, 0.01);
}

// Тестирование функции exp(x)
BOOST_AUTO_TEST_CASE(test_exp_function) {
    double result = monteCarloIntegral(0.0, 1.0, 10000, [](double x) { return exp(x); });
    double exact = exp(1) - 1;
    BOOST_CHECK_SMALL(result - exact, 0.01);
}

// Негативный тест: равные пределы интегрирования
BOOST_AUTO_TEST_CASE(test_equal_limits) {
    double result = monteCarloIntegral(1.0, 1.0, 10000, [](double x) { return x * x; });
    BOOST_CHECK_EQUAL(result, 0.0);
}

// Негативный тест: некорректное количество точек
BOOST_AUTO_TEST_CASE(test_invalid_points) {
    BOOST_CHECK_THROW(monteCarloIntegral(0.0, 1.0, -1000, [](double x) { return x * x; }),
                      std::invalid_argument);
}

// Тестирование многомерного интеграла
BOOST_AUTO_TEST_CASE(test_multidimensional_cube) {
    std::vector<std::pair<double, double>> limits = {
        {0.0, 1.0}, {0.0, 1.0}, {0.0, 1.0}};  // 3-мерный куб
    double result = monteCarloMultidimensional(3, limits, 10000,
                                               [](const std::vector<double>& x) { return 1.0; });
    double exact = 1.0;  // Площадь куба
    BOOST_CHECK_SMALL(result - exact, 0.01);
}

// Тестирование многомерного интеграла с функцией
BOOST_AUTO_TEST_CASE(test_multidimensional_function) {
    std::vector<std::pair<double, double>> limits = {{0.0, 1.0}, {0.0, 1.0}};  // 2-мерный куб
    double result = monteCarloMultidimensional(
        2, limits, 10000, [](const std::vector<double>& x) { return x[0] * x[1]; });
    double exact = 1.0 / 4.0;  // Интеграл для x*y на [0,1]x[0,1]
    BOOST_CHECK_SMALL(result - exact, 0.01);
}

// Негативный тест: слишком малое количество точек
BOOST_AUTO_TEST_CASE(test_too_few_points) {
    double result = monteCarloIntegral(0.0, 1.0, 1, [](double x) { return x * x; });
    BOOST_CHECK_MESSAGE(fabs(result - 1.0 / 3.0) > 0.1,
                        "Expected large error due to insufficient points.");
}

// Негативный тест: функция всегда возвращает ноль
BOOST_AUTO_TEST_CASE(test_zero_function) {
    double result = monteCarloIntegral(0.0, 1.0, 10000, [](double x) { return 0.0; });
    BOOST_CHECK_EQUAL(result, 0.0);
}

// Негативный тест: интеграл функции с острыми пиками
BOOST_AUTO_TEST_CASE(test_peak_function) {
    double result =
        monteCarloIntegral(0.0, 1.0, 10000, [](double x) { return (x < 0.5) ? 1000.0 : 0.0; });
    BOOST_CHECK_MESSAGE(result > 500, "Result should be large due to sharp peak.");
}

// Негативный тест: многомерная функция с острыми пиками
BOOST_AUTO_TEST_CASE(test_multidimensional_peak_function) {
    std::vector<std::pair<double, double>> limits = {{0.0, 1.0}, {0.0, 1.0}};
    double result = monteCarloMultidimensional(2, limits, 10000, [](const std::vector<double>& x) {
        return (x[0] < 0.5 && x[1] < 0.5) ? 1000.0 : 0.0;
    });
    BOOST_CHECK_MESSAGE(result > 100, "Expected large result due to peak.");
}
