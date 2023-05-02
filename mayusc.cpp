#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
    string input;
    while (getline(cin, input))
    {
        transform(input.begin(), input.end(), input.begin(), ::toupper);
        cout << input << endl;
    }
    return 0;
}
