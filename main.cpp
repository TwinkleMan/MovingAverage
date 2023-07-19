#include <fstream>
#include <iostream>
#include <vector>
#include "chrono"

int windowSize = 4;
long long resTime = 0;

template <typename T>
std::vector<T> movingAverage(std::vector<T>& data)
{
    std::vector<T> result;
    std::chrono::high_resolution_clock clock;

    // The first windowSize - 1 elements won't produce input data - the window is not filled yet
    result.reserve(data.size() - windowSize + 1);
    float sum = 0.0f;

    // Start clock
    auto beg = std::chrono::high_resolution_clock::now();

    // Count first window average
    for (int i = 0; i < windowSize; i++)
    {
        sum += data[i];
    }
    result.push_back(sum / windowSize);

    // Then we can count the next window average by subtracting the first window number and adding a new one
    for (int i = windowSize; i < data.size(); i++)
    {
        sum = sum - data[i - windowSize] + data[i];
        result.push_back(sum / windowSize);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
    resTime += 1000000 / duration.count();

    return result;
}

template <typename T>
void log(std::vector<T>& data, std::string filename)
{
    std::ofstream out;
    out.open(filename);

    if (out.is_open())
    {
//		for (const auto& elem : data)
//		{
//			out << elem << std::endl;
//		}
        out << data[0] << std::endl;
        out.close();
    }
    else
        std::cout << "Couldn't open file!" << std::endl;
}

int main()
{
    srand(time(nullptr));

    std::vector<float> floatInput;
    std::vector<double> doubleInput;
    int testSize = 1000000;
    for (int i = 0; i < testSize; i++)
    {
        floatInput.push_back(rand());
        doubleInput.push_back(rand());
    }

    std::cout << "Testing for float values" << std::endl;
    for (; windowSize <= 128; windowSize *= 2) {
        auto result = movingAverage(floatInput);
        for (int i = 0; i < 19; ++i) {
            result = movingAverage(floatInput);
        }
        std::cout << "Logging processed array" << std::endl;
        log(result, "output.csv");

        resTime /= 20;
        std::cout << "Result performance (window = " << windowSize << "): " << resTime << " count/ms\n\n";
    }

    resTime = 0;
    std::cout << "\n\nTesting for double values" << std::endl;
    for (windowSize = 4; windowSize <= 128; windowSize *= 2) {
        auto result = movingAverage(doubleInput);
        for (int i = 0; i < 19; ++i) {
            result = movingAverage(doubleInput);
        }
        std::cout << "Logging processed array" << std::endl;
        log(result, "output.csv");

        resTime /= 20;
        std::cout << "Result performance (window = " << windowSize << "): " << resTime << " count/ms\n\n";
    }
}
