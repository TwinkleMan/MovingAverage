#include <fstream>
#include <iostream>
#include <vector>

constexpr int windowSize = 4;
const std::string logFileName = "output.csv";

template <typename T>
std::vector<T> movingAverage(std::vector<T>& data)
{
	std::vector<T> result;

	// The first windowSize - 1 elements won't produce input data - the window is not filled yet
	result.reserve(data.size() - windowSize + 1);
	float sum = 0.0f;

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

	return result;
}

template <typename T>
void log(std::vector<T>& data, std::string filename)
{
	std::ofstream out;
	out.open(filename, std::ios::app);

	if (out.is_open())
	{
		for (const auto& elem : data)
		{
			out << elem << ";";
		}
		out << std::endl;
		out.close();
	}
	else
		std::cout << "Couldn't open file!" << std::endl;
}

int main()
{
	std::ofstream file(logFileName, std::ios::trunc);
	file.close();

	srand(time(nullptr));

	std::vector<float> input;

	int testSize = 1000000;
	for (int i = 0; i < testSize; i++)
	{
		input.push_back(rand() % 500);
	}
	std::cout << "Logging initial array" << std::endl;
	log(input, logFileName);

	auto result = movingAverage(input);
	std::cout << "Logging processed array" << std::endl;
	log(result, logFileName);
}
