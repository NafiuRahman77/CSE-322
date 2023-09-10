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
        cout << row << endl;
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

vector<string> HammingCodeDataBlock(const vector<string> &dataBlock)
{

    vector<string> res(dataBlock.size());
    int i = 0;
    for (string s : dataBlock)
    {
        res[i] = insertParityBits(s);
        i++;
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
                cout << "\033[32m" << hammingCode[i] << "\033[0m";
            }
            else
            {
                // Otherwise, print normally.
                cout << hammingCode[i];
            }
        }
        cout << endl;
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
    string remainder = dividend;
    int divisorLength = divisor.length();

    int i = 0;

    for (int i = 0; i < dividend.length() - (divisorLength - 1); i++)
    {
        if (remainder[i] == '1')
        {

            for (int j = 0; j < divisorLength; j++)
            {
                int intDivisor = divisor[j] - '0';
                int intRemainder = remainder[i + j] - '0';
                int xorResult = intDivisor ^ intRemainder;

                remainder[i + j] = static_cast<char>(xorResult + '0');
            }
        }
    }
    return remainder.substr(dividend.length() - (divisorLength - 1));
}

void printWithCyanRemainder(const string &dividend, const string &rem)
{
    cout << dividend;

    // Change text color to cyan
    cout << "\x1b[36m" << rem << "\x1b[0m"; // Set text color to cyan and reset color

    cout << endl;
}

string corruptData(const string &input, double errorProbability)
{
    srand(1);
    string receivedFrame = input;
    for (char &bit : receivedFrame)
    {
        double randValue = static_cast<double>(rand()) / RAND_MAX;
        if (randValue < errorProbability)
        {
            bit = (bit == '0') ? '1' : '0';
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
            cout << "\x1b[31m" << bit << "\x1b[0m"; // Set text color to red for erroneous bits
        }
        else
        {
            cout << bit; // Print correct bits without color
        }
    }
    cout << endl;
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

int main()
{
    string dataString = "Hamming Code";
    int m = 2;
    char paddingChar = '~';
    double probability = 0.05;
    string generatorPolynomial = "10101";

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

    cout << "Updated data string with padding: " << dataString << endl;

    // create and store the data block
    vector<string> dataBlock = createDataBlock(dataString, m);

    // print the data block
    cout << "Data block:" << endl;
    printDataBlock(dataBlock);

    cout << endl;

    // print the data block after adding check bits
    cout << "Data block after adding check bits:" << endl;
    vector<string> checkBitAddedDataBlock = HammingCodeDataBlock(dataBlock);
    printWithCheckBits(checkBitAddedDataBlock);
    int checkBitAddedDataBlockSize = checkBitAddedDataBlock[0].size();

    cout << endl;

    // print the data block after serializing in column major order
    string serialized = serializeColumnMajor(checkBitAddedDataBlock);
    cout << "Serializing data block:" << endl;
    cout << serialized << endl;

    cout << endl;

    // prints datablock with added check sum
    cout << "After adding checksum, data block:" << endl;

    int l_key = generatorPolynomial.length();

    // Appends n-1 zeroes at end of data
    string appended_data = (serialized + std::string(l_key - 1, '0'));

    printWithCyanRemainder(serialized, computeCRC(appended_data, generatorPolynomial));

    cout << endl;

    string checkSum = computeCRC(appended_data, generatorPolynomial);

    string serializedwithRem = serialized + computeCRC(appended_data, generatorPolynomial);

    // print corrupted data

    cout << "Corrupted Data :" << endl;
    string receivedData = corruptData(serializedwithRem, probability);

    printCorruptedData(serializedwithRem, receivedData);

    cout << endl;

    if (verifyReceivedData(receivedData, generatorPolynomial))
    {
        cout << "All ok" << endl;
    }
    else
    {
        cout << "error" << endl;
    }

    cout << endl;

    string removedCheckSum = receivedData.substr(0, receivedData.length() - checkSum.length());

    cout << "Removed Checksum Bits: " << endl;
    cout << removedCheckSum << endl;

    vector<string> receivedDataBlock = splitString(removedCheckSum, checkBitAddedDataBlock.size());
    cout << endl;

    printColoredDifference(checkBitAddedDataBlock, receivedDataBlock);

    return 0;
}
