#include <iostream>


void displayProgressBar(float progress, int barWidth = 50)
{
	std::cout << "[";
	int pos = static_cast<int>(progress * barWidth);
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos)
			std::cout << "=";
		else if (i == pos)
			std::cout << ">";
		else
			std::cout << " ";
	}
	std::cout << "] " << static_cast<int>(progress * 100.0) << "%\r";
	std::cout.flush();
}

