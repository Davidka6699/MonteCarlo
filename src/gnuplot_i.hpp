#ifndef GNUPLOT_I_HPP
#define GNUPLOT_I_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstdio>  // Для использования _popen и _pclose в Windows

/**
 * @class Gnuplot
 * @brief Класс для работы с утилитой Gnuplot из C++ кода.
 *
 * Класс предоставляет интерфейс для построения графиков с использованием утилиты Gnuplot.
 */
class Gnuplot {
public:
    /**
     * @brief Конструктор класса Gnuplot.
     *
     * Открывает процесс Gnuplot для взаимодействия с ним. Если Gnuplot недоступен,
     * генерируется исключение std::runtime_error.
     *
     * @throws std::runtime_error если Gnuplot не найден.
     */
    Gnuplot() {
        // Открываем процесс gnuplot
        gnuplotpipe = _popen("gnuplot -persist", "w");
        if (!gnuplotpipe) {
            throw std::runtime_error("Gnuplot not found!");
        }
    }

    /**
     * @brief Деструктор класса Gnuplot.
     *
     * Закрывает процесс Gnuplot, если он был открыт.
     */
    ~Gnuplot() {
        // Закрываем процесс gnuplot
        if (gnuplotpipe) {
            _pclose(gnuplotpipe);
        }
    }

    /**
     * @brief Отправляет команду в Gnuplot.
     *
     * Команда передается через pipe, который был открыт для процесса Gnuplot.
     *
     * @param command Строка, содержащая команду для выполнения в Gnuplot.
     */
    void sendCommand(const std::string& command) {
        fprintf(gnuplotpipe, "%s\n", command.c_str());
        fflush(gnuplotpipe);
    }

    /**
     * @brief Строит график на основе данных X и Y.
     *
     * Данные передаются в файл 'data.temp', который затем используется для построения графика.
     * График строится в режиме "lines".
     *
     * @param x Вектор значений для оси X.
     * @param y Вектор значений для оси Y.
     */
    void plotData(const std::vector<double>& x, const std::vector<double>& y) {
        std::ofstream tempFile("data.temp");
        for (size_t i = 0; i < x.size(); ++i) {
            tempFile << x[i] << " " << y[i] << std::endl;
        }
        tempFile.close();

        sendCommand("plot 'data.temp' with lines");
    }

private:
    FILE* gnuplotpipe; ///< Указатель на процесс Gnuplot для взаимодействия через pipe.
};

#endif // GNUPLOT_I_HPP
