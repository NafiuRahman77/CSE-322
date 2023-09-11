#include <random>
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <cmath>

using namespace std;

// Function to append padding character to the data string
void appendPadding(string &s, int m, char paddingChar)
{
    while (true)
    {
        if (s.length() % m == 0)
            break;
        else
            s += "~";
    }
}

// Function to create and store the data block
vector<string> createDataBlock(const string &s, int m)
{
    int numBlocks = s.length() / m;
    vector<string> dataBlock(numBlocks);

    for (int i = 0; i < numBlocks; i++)
    {
        string block;
        for (int j = 0; j < m; j++)
        {
            char c = s[i * m + j];
            block += bitset<8>(c).to_string(); // Store ASCII code in binary (8 bits)
        }
        dataBlock[i] = block;
    }

    return dataBlock;
}

// Function to print the data block
void printDataBlock(const vector<string> &dataBlock)
{
    for (string row : dataBlock)
    {
        std::cout << row << endl;
    }
}

// Function to calculate the number of parity bits needed for a given data length
int calculateParityBits(int dataLength)
{
    int i = 0;
    while (pow(2, i) < dataLength + i + 1)
    {
        i++;
    }
    return i;
}

// Function to insert parity bits into the binary string
string insertParityBits(const string &data)
{
    int dataLength = data.length();
    int parityBitCount = calculateParityBits(dataLength);

    string res; // Resulting string

    int maxLength = parityBitCount + dataLength;

    for (int i = 0, j = 0; i < maxLength; i++)
    {
        if ((i & (i + 1)) == 0)
        {
            res += '0';
        }
        else
        {
            res += data[j % dataLength];
            j++;
        }
    }

    for (int i = 0; i < parityBitCount; i++)
    {
        int p = pow(2, i);
        int x = 0;
        for (int j = 0; j < res.size(); j++)
        {
            if (p & (j + 1))
            {
                char c = res[j];
                if (c == '1')
                {
                    x = (x == 0) ? 1 : 0;
                }
            }
        }
        res[p - 1] = static_cast<char>(x + '0');
    }

    return res;
}

string removeParityBits(const string &data)
{

    int dataLength = data.length();

    int parityBitCount = calculateParityBits(dataLength);
    int error = 0;
    for (int i = 0; i < parityBitCount; i++)
    {
        int p = pow(2, i);
        int x = 0;
        for (int j = 0; j < data.size(); j++)
        {
            if (p & (j + 1))
            {
                char c = data[j];
                if (c == '1')
                {
                    x = (x == 0) ? 1 : 0;
                }
            }
        }
        if (x == 1)
        {
            error += p;
        }
    }

    string s = data;
    if (error > 0 && error <= data.size())
    {
        s[error - 1] = (data[error - 1] == '0') ? '1' : '0';
    }

    std::string result;

    for (int i = 0; i < s.length(); i++)
    {

        if (((i + 1) & i) != 0)
        {
            result += s[i];
        }
    }

    return result;
}
vector<string> HammingCodeDataBlock(const vector<string> &dataBlock)
{

    vector<string> res;

    int i = 0;
    for (int i = 0; i < dataBlock.size(); i++)
    {
        // cout << "debug " << i << endl;
        res.push_back(insertParityBits(dataBlock[i]));
    }
    return res;
}

vector<string> HammingDecodeDataBlock(const vector<string> &dataBlock)
{

    vector<string> res;
    int i = 0;

    for (int i = 0; i < dataBlock.size(); i++)
    {
        res.push_back(removeParityBits(dataBlock[i]));
    }
    return res;
}

void printWithCheckBits(const vector<string> &hammingEncodedBlock)
{
    for (int j = 0; j < hammingEncodedBlock.size(); j++)
    {
        string hammingCode = hammingEncodedBlock[j];
        for (int i = 0; i < hammingCode.length(); i++)
        {
            if ((i & (i + 1)) == 0)
            {
                // If i is one less than a power of two, print in green.
                std::cout << "\033[32m" << hammingCode[i] << "\033[0m";
            }
            else
            {
                // Otherwise, print normally.
                std::cout << hammingCode[i];
            }
        }
        std::cout << endl;
    }
}

string serializeColumnMajor(const std::vector<std::string> &dataBlock)
{

    // Find the maximum length of strings in the vector
    size_t maxStringLength = 0;
    for (const std::string &str : dataBlock)
    {
        maxStringLength = std::max(maxStringLength, str.length());
    }

    // Create a result string with the appropriate size
    std::string result;
    result.reserve(maxStringLength * dataBlock.size());

    // Iterate through each column and concatenate characters
    for (size_t col = 0; col < maxStringLength; col++)
    {
        for (const std::string &str : dataBlock)
        {
            if (col < str.length())
            {
                result.push_back(str[col]);
            }
            else
            {
                // If the string in this column is shorter, add a placeholder (e.g., space)
                result.push_back(' ');
            }
        }
    }

    return result;
}

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

void printWithCyanRemainder(const string &dividend, const string &rem)
{
    std::cout << dividend;

    // Change text color to cyan
    std::cout << "\x1b[36m" << rem << "\x1b[0m"; // Set text color to cyan and reset color

    std::cout << endl;
}

string corruptData(const string &input, double errorProbability)
{

    string receivedFrame = input;
    for (int i = 0; i < receivedFrame.size(); i++)
    {
        double randValue = (double)(rand()) / RAND_MAX;
        if (randValue < errorProbability)
        {
            receivedFrame[i] = (receivedFrame[i] == '0') ? '1' : '0';
        }
    }
    return receivedFrame;
}

void printCorruptedData(const string &actualData, const string &corruptedData)
{
    for (int i = 0; i < corruptedData.length(); i++)
    {
        char bit = corruptedData[i];
        if (bit != actualData[i])
        {
            std::cout << "\x1b[31m" << bit << "\x1b[0m"; // Set text color to red for erroneous bits
        }
        else
        {
            std::cout << bit; // Print correct bits without color
        }
    }
    std::cout << endl;
}

bool verifyReceivedData(const string &receivedData, const string &generatorPolynomial)
{
    int genLength = generatorPolynomial.length();
    string rem = computeCRC(receivedData, generatorPolynomial);

    for (char bit : rem)
    {
        if (bit == '1')
        {
            return false;
        }
    }

    return true;
}

std::vector<std::string> splitString(const std::string &inputString, int length)
{
    std::vector<std::string> result(length);
    int inputLength = inputString.length();

    int idx = 0;
    // int chunkSize = inputLength / length;
    for (int i = 0; i < inputLength; i++)
    {
        result[idx] += inputString[i];
        idx = (idx + 1) % length;
    }

    return result;
}

void printColoredDifference(const std::vector<std::string> &checkBitAddedDataBlock,
                            const std::vector<std::string> &receivedDataBlock)
{
    if (checkBitAddedDataBlock.size() != receivedDataBlock.size())
    {
        std::cerr << "Input vectors must have the same size." << std::endl;
        return;
    }

    for (size_t i = 0; i < checkBitAddedDataBlock.size(); ++i)
    {
        const std::string &checkStr = checkBitAddedDataBlock[i];
        const std::string &receivedStr = receivedDataBlock[i];

        if (checkStr.length() != receivedStr.length())
        {
            std::cerr << "Strings in the vectors must have the same length." << std::endl;
            return;
        }

        for (size_t j = 0; j < checkStr.length(); ++j)
        {
            if (checkStr[j] != receivedStr[j])
            {
                std::cout << "\x1B[31m" << receivedStr[j] << "\x1B[0m";
            }
            else
            {
                std::cout << receivedStr[j];
            }
        }

        std::cout << std::endl;
    }
}

string generateOutputFrame(const vector<string> &removedCheckBits)
{
    string result;

    for (int k = 0; k < removedCheckBits.size(); k++)
    {
        for (size_t i = 0; i < removedCheckBits[k].length(); i += 8)
        {
            std::string substring = removedCheckBits[k].substr(i, 8);
            char character = static_cast<char>(stoi(substring, nullptr, 2));
            result += character;
        }
    }
    return result;
}

int main()
{
    srand(1);
    string dataString = "Computer Networks";
    int m = 4;
    char paddingChar = '~';
    double probability = 0.04;
    string generatorPolynomial = "1010111";

    cout << "Enter the data string: " << endl;
    getline(cin, dataString);
    cout << "Enter m (number of bytes in a row): " << endl;
    cin >> m;
    cin.ignore();
    cout << "Enter generator polynomial: " << endl;
    getline(cin, generatorPolynomial);
    cout << "Enter probability: " << endl;
    cin >> probability;

    // append padding
    appendPadding(dataString, m, paddingChar);
    std::cout << "data string after padding: " << dataString << endl;
    std::cout << endl;

    // create and store the data block
    vector<string> dataBlock = createDataBlock(dataString, m);

    // print the data block
    std::cout << "data block (ascii code of m characters per row):" << endl;
    printDataBlock(dataBlock);
    std::cout << endl;

    // print the data block after adding check bits
    std::cout << "data block after adding check bits:" << endl;
    vector<string> checkBitAddedDataBlock = HammingCodeDataBlock(dataBlock);
    printWithCheckBits(checkBitAddedDataBlock);
    int checkBitAddedDataBlockSize = checkBitAddedDataBlock[0].size();
    std::cout << endl;

    // print the data block after serializing in column major order
    string serialized = serializeColumnMajor(checkBitAddedDataBlock);
    std::cout << "data bits after column-wise serialization:" << endl;
    std::cout << serialized << endl;
    std::cout << endl;

    // prints datablock with added check sum
    std::cout << "data bits after appending CRC checksum (sent frame):" << endl;
    int l_key = generatorPolynomial.length();
    string appended_data = (serialized + std::string(l_key - 1, '0'));
    printWithCyanRemainder(serialized, computeCRC(appended_data, generatorPolynomial));
    std::cout << endl;

    // print corrupted data

    string checkSum = computeCRC(appended_data, generatorPolynomial);
    string serializedwithRem = serialized + computeCRC(appended_data, generatorPolynomial);
    std::cout << "received frame :" << endl;
    string receivedData = corruptData(serializedwithRem, probability);
    printCorruptedData(serializedwithRem, receivedData);
    std::cout << endl;

    std::cout << "result of CRC checksum matching : ";
    if (verifyReceivedData(receivedData, generatorPolynomial))
    {
        std::cout << "no error detected" << endl;
    }
    else
    {
        std::cout << "error detected" << endl;
    }

    std::cout << endl;

    // print after removing checksum bits, in data block format again
    std::cout << "data bits after removing CRC checksum bits: " << endl;
    string removedCheckSum = receivedData.substr(0, receivedData.length() - checkSum.length());
    vector<string> receivedDataBlock = splitString(removedCheckSum, checkBitAddedDataBlock.size());
    printColoredDifference(checkBitAddedDataBlock, receivedDataBlock);
    std::cout << endl;

    // Remove check bits
    std::cout << "data bits after removing check bits :" << endl;
    vector<string> removedCheckBits = HammingDecodeDataBlock(receivedDataBlock);
    printDataBlock(removedCheckBits);
    cout << endl;
    std::string result = generateOutputFrame(removedCheckBits);

    // Output the frame
    std::cout << "output frame: " << result << std::endl;

    return 0;
}
