#include <iostream>
#include <windows.h>

using namespace std;

const string RECOVERY_KEY = "recover";

void unhideFile(const string &filePath)
{
    SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL);
    cout << "File has been successfully unhidden.\n";
}

int main()
{
    const string filePath = "test.txt";
    string userKey;

    cout << "Enter the recovery key to unhide the file: ";
    cin >> userKey;

    if (userKey == RECOVERY_KEY)
    {
        unhideFile(filePath);
    }
    else
    {
        cout << "Incorrect recovery key. Access denied.\n";
    }

    return 0;
}
