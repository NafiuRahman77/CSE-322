#include <iostream>
#include <string>
using namespace std;
// Function to perform modulo-2 division and return the remainder

std::string computeCRC(const std::string &dividend, const std::string &divisor)
{
	std::string remainder = dividend;
	int divisorLength = divisor.length();
	int dividendLength = dividend.length();
	int i = 0;

	while (i < dividendLength - (divisorLength - 1))
	{
		if (remainder[i] == '1')
		{
			int j = 0;
			string tempXORResult;  // Accumulate XOR results in a temporary string

            while (j < divisorLength) {
                int intDivisor = divisor[j] - '0';
                int intRemainder = remainder[i + j] - '0';
                int xorResult = intDivisor ^ intRemainder;
                tempXORResult += to_string(xorResult);  // Store XOR result as a string
                j++;
            }

            for (int k = 0; k < tempXORResult.length(); k++) {
                remainder[i + k] = tempXORResult[k];
            }
		}
		i++;
	}

	return remainder.substr(dividendLength - (divisorLength - 1));
}

int main()
{
	std::string dividend = "0100100001000001011011011011110101110110000000101010011110110000011111110110101101001101111111110111100100000011111111110001110001111010001111100000001111101101111111110100011001010101100010000000"; // Your dividend
	std::string divisor = "1010111";																																 // Your divisor (generator polynomial)

	// Calculate the CRC remainder (checksum)
	std::string remainder = computeCRC(dividend, divisor);

	std::cout << "CRC Remainder (Checksum): " << remainder << std::endl;

	return 0;
}
