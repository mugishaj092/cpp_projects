#include <iostream>
#include <fstream>
#include <unordered_map>
#include <windows.h>
#include <thread>
#include <chrono>
#include <sstream>

using namespace std;

const string FILE_PATH = "test.txt";
const string LOG_FILE = "access_log.txt";
const string MASTER_KEY = "admin123";
const string DB_FILE = "db.txt";

unordered_map<string, string> userKeys;

void loadUserCredentials()
{
    ifstream file(DB_FILE);
    if (!file.is_open())
    {
        cout << "Error: Could not open the credentials file.\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string username, key;
        ss >> username >> key;
        userKeys[username] = key;
    }
    file.close();
}

void saveUserCredentials()
{
    ofstream file(DB_FILE, ios::trunc);
    if (!file.is_open())
    {
        cout << "Error: Could not open the credentials file for saving.\n";
        return;
    }

    for (const auto &entry : userKeys)
    {
        file << entry.first << " " << entry.second << endl;
    }
    file.close();
}

void logAccessAttempt(const string &username, bool success)
{
    ofstream log(LOG_FILE, ios::app);
    if (log.is_open())
    {
        log << "User: " << username
            << " | Access: " << (success ? "Granted" : "Denied ")
            << " | Time: " << time(nullptr) << endl;
        log.close();
    }
}

void hideFile(const string &filePath)
{
    SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_HIDDEN);
    cout << "The file has been hidden due to unauthorized access attempts.\n";
}

void unhideFile(const string &filePath)
{
    SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL);
    cout << "The file is now unhidden.\n";
}

void displayFileContent()
{
    ifstream file(FILE_PATH);
    if (!file.is_open())
    {
        cout << "Error: Could not open the file.\n";
        return;
    }

    cout << "\n--- File Content ---\n";
    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    cout << "---------------------\n";
    file.close();
}
void editFileContent()
{
    cout << "\nDo you want to:\n";
    cout << "1. Append to the file\n";
    cout << "2. Overwrite the file\n";
    cout << "Enter your choice (1 or 2): ";
    int choice;
    cin >> choice;
    cin.ignore(); // Clear the input buffer

    if (choice == 1)
    {
        // Append to the file
        ifstream file(FILE_PATH);
        if (file.is_open())
        {
            cout << "\n--- Current File Content ---\n";
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
            }
            cout << "-----------------------------\n";
            file.close();
        }
        else
        {
            cout << "Error: Could not open the file.\n";
            return;
        }

        ofstream fileOut(FILE_PATH, ios::app);
        if (!fileOut.is_open())
        {
            cout << "Error: Could not open the file for editing.\n";
            return;
        }

        cout << "Enter text to append to the file (type END to stop):\n";
        string newLine;
        while (true)
        {
            getline(cin, newLine);
            if (newLine == "END")
                break;
            fileOut << newLine << endl;
        }
        fileOut.close();
        cout << "Changes appended.\n";
    }
    else if (choice == 2)
    {
        // Overwrite the file
        ofstream fileOut(FILE_PATH, ios::trunc);
        if (!fileOut.is_open())
        {
            cout << "Error: Could not open the file for overwriting.\n";
            return;
        }

        cout << "Enter new content for the file (type END to stop):\n";
        string newLine;
        while (true)
        {
            getline(cin, newLine);
            if (newLine == "END")
                break;
            fileOut << newLine << endl;
        }
        fileOut.close();
        cout << "File content overwritten.\n";
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

void adminActions()
{
    int choice;
    cout << "\nAdmin actions:\n";
    cout << "1. Add new user\n2. Delete user\n3. Change user key\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    string username, key;
    switch (choice)
    {
    case 1:
        cout << "Enter new username: ";
        cin >> username;
        cout << "Enter secret key: ";
        cin >> key;
        userKeys[username] = key;
        saveUserCredentials();
        cout << "New user added.\n";
        break;
    case 2:
        cout << "Enter username to delete: ";
        cin >> username;
        if (userKeys.find(username) != userKeys.end())
        {
            userKeys.erase(username);
            saveUserCredentials();
            cout << "User deleted.\n";
        }
        else
        {
            cout << "User not found.\n";
        }
        break;
    case 3:
        cout << "Enter username to change key: ";
        cin >> username;
        if (userKeys.find(username) != userKeys.end())
        {
            cout << "Enter new secret key: ";
            cin >> key;
            userKeys[username] = key;
            saveUserCredentials();
            cout << "Key changed.\n";
        }
        else
        {
            cout << "User not found.\n";
        }
        break;
    default:
        cout << "Invalid choice.\n";
    }
}

// Main Program
int main()
{
    loadUserCredentials();

    int attempts = 0;
    string username, key;

    cout << "Enter your username: ";
    cin >> username;

    if (userKeys.find(username) == userKeys.end() && username != "admin")
    {
        cout << "Unknown user.\n";
        return 0;
    }

    while (attempts < 2)
    {
        cout << "Enter your secret key: ";
        cin >> key;

        if (username == "admin" && key == MASTER_KEY)
        {
            cout << "Admin access granted.\n";
            adminActions();
            return 0;
        }

        if (userKeys[username] == key)
        {
            cout << "Access granted! Choose an option:\n";
            cout << "1. View file\n2. Edit file\nEnter choice: ";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                displayFileContent();
            }
            else if (choice == 2)
            {
                editFileContent();
            }
            else
            {
                cout << "Invalid choice.\n";
            }

            logAccessAttempt(username, true);
            return 0;
        }
        else
        {
            cout << "Incorrect key. Try again.\n";
            attempts++;
            logAccessAttempt(username, false);
            if (attempts < 2)
            {
                this_thread::sleep_for(chrono::seconds(3));
            }
        }
    }
    hideFile(FILE_PATH);
    return 0;
}
