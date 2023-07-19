#include <fstream>
#include <iostream>
#include <vector>
#include "chrono"

int windowSize = 4;                 // Size of averaging window
long long performance = 0;          // Performance (counts/ms)
const int numberOfTests = 20;       // Number of tests for each window size
const int testDataSize = 1000000;   // Number of elements in test

template <typename T>
std::vector<T> movingAverage(const std::vector<T>& data)
{
    std::vector<T> result;

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
    // Stop clock
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
    performance += 1000000 / duration.count();

    return result;
}

// Log data to file
template <typename T>
void log(std::vector<T>& data, std::string filename)
{
    std::ofstream out;
    out.open(filename);

    if (out.is_open())
    {
        // Uncomment for full input data logging
//		for (const auto& elem : data)
//		{
//			out << elem << std::endl;
//		}

        // Log only first number to decrease testing time
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
    for (int i = 0; i < testDataSize; i++)
    {
        floatInput.push_back(rand());
        doubleInput.push_back(rand());
    }

    std::cout << "Testing for float values" << std::endl;
    for (; windowSize <= 128; windowSize *= 2) {
        // Run the processing 20 times to get
        std::vector<float> result;
        result.reserve(testDataSize);
        for (int i = 0; i < numberOfTests; ++i) {
            result = movingAverage(floatInput);
        }
        std::cout << "Logging processed array" << std::endl;
        log(result, "output.csv");

        performance /= numberOfTests;
        std::cout << "Result performance (window = " << windowSize << "): " << performance << " count/ms\n\n";
    }

    performance = 0;
    std::cout << "\n\nTesting for double values" << std::endl;
    for (windowSize = 4; windowSize <= 128; windowSize *= 2) {
        std::vector<double> result;
        result.reserve(testDataSize);
        for (int i = 0; i < numberOfTests; ++i) {
            result = movingAverage(doubleInput);
        }
        std::cout << "Logging processed array" << std::endl;
        log(result, "output.csv");

        performance /= numberOfTests;
        std::cout << "Result performance (window = " << windowSize << "): " << performance << " count/ms\n\n";
    }
}
