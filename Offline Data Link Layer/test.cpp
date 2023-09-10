#include <bits/stdc++.h>
using namespace std;

// Returns XOR of 'a' and 'b'
// (both of same length)
string xor1(string a, string b)
{

	// Initialize result
	string result = "";

	int n = b.length();

	// Traverse all bits, if bits are
	// same, then XOR is 0, else 1
	for (int i = 1; i < n; i++) {
		if (a[i] == b[i])
			result += "0";
		else
			result += "1";
	}
	return result;
}

// Performs Modulo-2 division
string mod2div(string dividend, string divisor)
{

	// Number of bits to be XORed at a time.
	int pick = divisor.length();

	// Slicing the dividend to appropriate
	// length for particular step
	string tmp = dividend.substr(0, pick);

	int n = dividend.length();

	while (pick < n) {
		if (tmp[0] == '1')

			// Replace the dividend by the result
			// of XOR and pull 1 bit down
			tmp = xor1(divisor, tmp) + dividend[pick];
		else

			// If leftmost bit is '0'.
			// If the leftmost bit of the dividend (or the
			// part used in each step) is 0, the step cannot
			// use the regular divisor; we need to use an
			// all-0s divisor.
			tmp = xor1(std::string(pick, '0'), tmp)
				+ dividend[pick];

		// Increment pick to move further
		pick += 1;
	}

	// For the last n bits, we have to carry it out
	// normally as increased value of pick will cause
	// Index Out of Bounds.
	if (tmp[0] == '1')
		tmp = xor1(divisor, tmp);
	else
		tmp = xor1(std::string(pick, '0'), tmp);

	return tmp;
}

// Function used at the sender side to encode
// data by appending remainder of modular division
// at the end of data.
void encodeData(string data, string key)
{
	int l_key = key.length();

	// Appends n-1 zeroes at end of data
	string appended_data
		= "10011011101111";

	string remainder = mod2div(appended_data, key);

	// Append remainder in the original data
	string codeword = data + remainder;
	cout << "Remainder : " << remainder << "\n";
	cout << "Encoded Data (Data + Remainder) :" << codeword
		<< "\n";
}
// checking if the message received by receiver is correct
// or not. If the remainder is all 0 then it is correct,
// else wrong.
void receiver(string data, string key)
{
	string currxor
		= mod2div(data.substr(0, key.size()), key);
	int curr = key.size();
	while (curr != data.size()) {
		if (currxor.size() != key.size()) {
			currxor.push_back(data[curr++]);
		}
		else {
			currxor = mod2div(currxor, key);
		}
	}
	if (currxor.size() == key.size()) {
		currxor = mod2div(currxor, key);
	}
	if (currxor.find('1') != string::npos) {
		cout << "there is some error in data" << endl;
	}
	else {
		cout << "correct message received" << endl;
	}
}
// Driver code
int main()
{
	string data = "10011011101111";
	string key = "101";
	cout << "Sender side..." << endl;
	encodeData(data, key);

	cout << "\nReceiver side..." << endl;
	receiver(data+mod2div(data+std::string(key.size() - 1, '0'),key), key);

	return 0;
}

// This code is contributed by MuskanKalra1 , Mayank Sharma
