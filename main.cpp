#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
//sources- claude has helped with functions 

// Function to trim leading and trailing whitespace
string trim(const string& str) {
    size_t start = 0;
    size_t end = str.length();
    
    while (start < end && isspace(str[start])) start++;
    while (end > start && isspace(str[end - 1])) end--;
    
    return str.substr(start, end - start);
}

// Function to validate if a string is a valid double number
bool isValidDouble(const string& s) {
    if (s.empty()) return false;
    
    string str = trim(s);
    if (str.empty()) return false;
    
    size_t i = 0;
    
    // Check for optional sign
    if (str[i] == '+' || str[i] == '-') {
        i++;
    }
    
    // Must have at least one digit after sign (if any)
    if (i >= str.length()) return false;
    
    bool hasDigitBeforeDecimal = false;
    bool hasDecimal = false;
    bool hasDigitAfterDecimal = false;
    
    // Parse digits before decimal point
    while (i < str.length() && isdigit(str[i])) {
        hasDigitBeforeDecimal = true;
        i++;
    }
    
    // Check for decimal point
    if (i < str.length() && str[i] == '.') {
        hasDecimal = true;
        i++;
        
        // Parse digits after decimal point
        while (i < str.length() && isdigit(str[i])) {
            hasDigitAfterDecimal = true;
            i++;
        }
    }
    
    // Must have consumed entire string
    if (i != str.length()) return false;
    
    // Must have at least one digit
    if (!hasDigitBeforeDecimal && !hasDigitAfterDecimal) return false;
    
    // If decimal point exists, must have digits on both sides
    if (hasDecimal && (!hasDigitBeforeDecimal || !hasDigitAfterDecimal)) {
        return false;
    }
    
    return true;
}

// Function to remove leading zeros from integer part
string removeLeadingZeros(const string& s) {
    size_t i = 0;
    while (i < s.length() && s[i] == '0') {
        i++;
    }
    if (i == s.length()) return "0";
    return s.substr(i);
}

// Function to add two positive numbers (without signs)
string addPositive(const string& a, const string& b) {
    string num1 = a, num2 = b;
    
    // Find decimal points
    size_t dot1 = num1.find('.');
    size_t dot2 = num2.find('.');
    
    string int1, frac1, int2, frac2;
    
    if (dot1 != string::npos) {
        int1 = num1.substr(0, dot1);
        frac1 = num1.substr(dot1 + 1);
    } else {
        int1 = num1;
        frac1 = "";
    }
    
    if (dot2 != string::npos) {
        int2 = num2.substr(0, dot2);
        frac2 = num2.substr(dot2 + 1);
    } else {
        int2 = num2;
        frac2 = "";
    }
    
    // Pad fractional parts to same length
    while (frac1.length() < frac2.length()) frac1 += '0';
    while (frac2.length() < frac1.length()) frac2 += '0';
    
    // Add fractional parts
    string fracResult = "";
    int carry = 0;
    
    for (int i = frac1.length() - 1; i >= 0; i--) {
        int digit1 = frac1[i] - '0';
        int digit2 = frac2[i] - '0';
        int sum = digit1 + digit2 + carry;
        fracResult = char('0' + (sum % 10)) + fracResult;
        carry = sum / 10;
    }
    
    // Add integer parts
    string intResult = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    
    while (i >= 0 || j >= 0 || carry > 0) {
        int digit1 = (i >= 0) ? (int1[i] - '0') : 0;
        int digit2 = (j >= 0) ? (int2[j] - '0') : 0;
        int sum = digit1 + digit2 + carry;
        intResult = char('0' + (sum % 10)) + intResult;
        carry = sum / 10;
        i--;
        j--;
    }
    
    // Remove leading zeros from integer part
    intResult = removeLeadingZeros(intResult);
    
    // Build result
    if (fracResult.empty()) {
        return intResult;
    } else {
        // Remove trailing zeros from fractional part
        while (!fracResult.empty() && fracResult.back() == '0') {
            fracResult.pop_back();
        }
        if (fracResult.empty()) {
            return intResult;
        }
        return intResult + "." + fracResult;
    }
}

// Compare absolute values: returns -1 if a < b, 0 if equal, 1 if a > b
int compareAbs(const string& a, const string& b) {
    string num1 = a, num2 = b;
    
    size_t dot1 = num1.find('.');
    size_t dot2 = num2.find('.');
    
    string int1, frac1, int2, frac2;
    
    if (dot1 != string::npos) {
        int1 = num1.substr(0, dot1);
        frac1 = num1.substr(dot1 + 1);
    } else {
        int1 = num1;
        frac1 = "";
    }
    
    if (dot2 != string::npos) {
        int2 = num2.substr(0, dot2);
        frac2 = num2.substr(dot2 + 1);
    } else {
        int2 = num2;
        frac2 = "";
    }
    
    int1 = removeLeadingZeros(int1);
    int2 = removeLeadingZeros(int2);
    
    if (int1.length() != int2.length()) {
        return (int1.length() > int2.length()) ? 1 : -1;
    }
    
    if (int1 != int2) {
        return (int1 > int2) ? 1 : -1;
    }
    
    // Pad fractional parts
    while (frac1.length() < frac2.length()) frac1 += '0';
    while (frac2.length() < frac1.length()) frac2 += '0';
    
    if (frac1 == frac2) return 0;
    return (frac1 > frac2) ? 1 : -1;
}

// Subtract two positive numbers (a - b), assumes a >= b
string subtractPositive(const string& a, const string& b) {
    string num1 = a, num2 = b;
    
    size_t dot1 = num1.find('.');
    size_t dot2 = num2.find('.');
    
    string int1, frac1, int2, frac2;
    
    if (dot1 != string::npos) {
        int1 = num1.substr(0, dot1);
        frac1 = num1.substr(dot1 + 1);
    } else {
        int1 = num1;
        frac1 = "";
    }
    
    if (dot2 != string::npos) {
        int2 = num2.substr(0, dot2);
        frac2 = num2.substr(dot2 + 1);
    } else {
        int2 = num2;
        frac2 = "";
    }
    
    // Pad fractional parts
    while (frac1.length() < frac2.length()) frac1 += '0';
    while (frac2.length() < frac1.length()) frac2 += '0';
    
    // Subtract fractional parts
    string fracResult = "";
    int borrow = 0;
    
    for (int i = frac1.length() - 1; i >= 0; i--) {
        int digit1 = frac1[i] - '0';
        int digit2 = frac2[i] - '0';
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        fracResult = char('0' + diff) + fracResult;
    }
    
    // Subtract integer parts
    string intResult = "";
    int i = int1.length() - 1;
    int j = int2.length() - 1;
    
    while (i >= 0 || j >= 0) {
        int digit1 = (i >= 0) ? (int1[i] - '0') : 0;
        int digit2 = (j >= 0) ? (int2[j] - '0') : 0;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        intResult = char('0' + diff) + intResult;
        i--;
        j--;
    }
    
    // Remove leading zeros
    intResult = removeLeadingZeros(intResult);
    
    // Build result
    if (fracResult.empty()) {
        return intResult;
    } else {
        while (!fracResult.empty() && fracResult.back() == '0') {
            fracResult.pop_back();
        }
        if (fracResult.empty()) {
            return intResult;
        }
        return intResult + "." + fracResult;
    }
}

// Main addition function handling signs
string addNumbers(const string& a, const string& b) {
    string num1 = trim(a);
    string num2 = trim(b);
    
    bool neg1 = false, neg2 = false;
    size_t start1 = 0, start2 = 0;
    
    if (num1[0] == '-') {
        neg1 = true;
        start1 = 1;
    } else if (num1[0] == '+') {
        start1 = 1;
    }
    
    if (num2[0] == '-') {
        neg2 = true;
        start2 = 1;
    } else if (num2[0] == '+') {
        start2 = 1;
    }
    
    string abs1 = num1.substr(start1);
    string abs2 = num2.substr(start2);
    
    // Remove leading zeros from integer part
    size_t dot1 = abs1.find('.');
    if (dot1 != string::npos) {
        string intPart = removeLeadingZeros(abs1.substr(0, dot1));
        abs1 = intPart + abs1.substr(dot1);
    } else {
        abs1 = removeLeadingZeros(abs1);
    }
    
    size_t dot2 = abs2.find('.');
    if (dot2 != string::npos) {
        string intPart = removeLeadingZeros(abs2.substr(0, dot2));
        abs2 = intPart + abs2.substr(dot2);
    } else {
        abs2 = removeLeadingZeros(abs2);
    }
    
    string result;
    
    if (!neg1 && !neg2) {
        // Both positive
        result = addPositive(abs1, abs2);
    } else if (neg1 && neg2) {
        // Both negative
        result = "-" + addPositive(abs1, abs2);
    } else {
        // Different signs
        int cmp = compareAbs(abs1, abs2);
        if (cmp == 0) {
            result = "0";
        } else if (cmp > 0) {
            result = subtractPositive(abs1, abs2);
            if (neg1) result = "-" + result;
        } else {
            result = subtractPositive(abs2, abs1);
            if (neg2) result = "-" + result;
        }
    }
    
    return result;
}

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return 1;
    }
    
    string line;
    int lineNum = 0;
    
    while (getline(infile, line)) {
        lineNum++;
        
        istringstream iss(line);
        string num1, num2;
        
        if (!(iss >> num1 >> num2)) {
            cout << "Line " << lineNum << ": Invalid format" << endl;
            continue;
        }
        
        bool valid1 = isValidDouble(num1);
        bool valid2 = isValidDouble(num2);
        
        if (!valid1 || !valid2) {
            cout << "Line " << lineNum << ": ";
            if (!valid1) cout << num1 << " is invalid";
            if (!valid1 && !valid2) cout << ", ";
            if (!valid2) cout << num2 << " is invalid";
            cout << endl;
        } else {
            string result = addNumbers(num1, num2);
            cout << "Line " << lineNum << ": " << num1 << " + " << num2 << " = " << result << endl;
        }
    }
    
    infile.close();
    return 0;
}