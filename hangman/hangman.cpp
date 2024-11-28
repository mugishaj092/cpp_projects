#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;

void PrintMessage(string message, bool printTop = true, bool printBottom = true)
{
    if (printTop)
    {
        cout << "+---------------------------------+" << endl;
        cout << "|";
    }
    else
    {
        cout << "|";
    }
    bool front = true;
    for (int i = message.length(); i < 33; i++)
    {
        if (front)
        {
            message = " " + message;
        }
        else
        {
            message = message + " ";
        }
        front = !front;
    }
    cout << message.c_str();
    if (printBottom)
    {
        cout << "|" << endl;
        cout << "+---------------------------------+" << endl;
    }
    else
    {
        cout << "|" << endl;
    }
}

void DrawHangman(int guesCount = 0)
{
    if (guesCount >= 1)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);
    if (guesCount >= 2)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);
    if (guesCount >= 3)
        PrintMessage("0", false, false);
    else
        PrintMessage("", false, false);
    if (guesCount == 4)
        PrintMessage("/  ", false, false);
    if (guesCount == 5)
        PrintMessage("/| ", false, false);
    if (guesCount >= 6)
        PrintMessage("/|\\", false, false);
    else
        PrintMessage("", false, false);
    if (guesCount >= 7)
        PrintMessage("|", false, false);
    else
        PrintMessage("", false, false);
    if (guesCount == 8)
        PrintMessage("/  ", false, false);
    if (guesCount >= 9)
        PrintMessage("/ \\", false, false);
}

void PrintLetters(string input, char from, char to)
{
    string s;
    for (char i = from; i <= to; i++)
    {
        if (input.find(i) == string::npos)
        {
            s += i;
            s += " ";
        }
        else
            s += "  ";
    }
    PrintMessage(s, false, false);
}

void PrintAvailableLetters(string taken)
{
    PrintMessage("AVAILABLE LETTER");
    PrintLetters(taken, 'A', 'M');
    PrintLetters(taken, 'N', 'Z');
}

bool PrintWordAndCheckWinner(string word, string guessed)
{
    bool won = true;
    string s;
    for (int i = 0; i < word.length(); i++)
    {
        if (guessed.find(word[i]) == string::npos)
        {
            won = false;
            s += "_ ";
        }
        else
            s += word[i];
        s += " ";
    }
    PrintMessage(s, false, true);
    return won;
}

string LoadRondomWords()
{
    int lineCount = 0;
    string word;
    vector<string> v;
    ifstream reader("words.txt");
    if (reader.is_open())
    {
        while (getline(reader, word))
        {
            v.push_back(word);
        }
        int randomLine = rand() % v.size();
        word = v.at(randomLine);
    }
    return word;
}

int TriesLeft(string word, string guessed)
{
    int error = 0;
    for (int i = 0; i <= guessed.length(); i++)
    {
        if (word.find(guessed[i]) == string::npos)
        {
            error++;
        }
    }
    return error;
}

int main()
{
    srand(time(0));
    char playAgain;

    do
    {
        string wordToGuess = LoadRondomWords();
        string gueses;
        int tries = 0;
        bool win = false;

        do
        {
            system("cls");
            PrintMessage("HANG MAN IS COOL");
            DrawHangman(10 - tries);
            PrintAvailableLetters(gueses);
            PrintMessage("Guessed Word");
            win = PrintWordAndCheckWinner(wordToGuess, gueses);
            if (win)
                break;

            char x;
            cout << "> ";
            cin >> x;

            if (gueses.find(x) == string::npos)
            {
                gueses += x;
            }
            tries = TriesLeft(wordToGuess, gueses);
        } while (tries < 10);

        if (win)
        {
            PrintMessage("YOU WON!");
        }
        else
            PrintMessage("Game Over");

        cout << "Do you want to play again? (Y/N): ";
        cin >> playAgain;
    } while (playAgain == 'Y' || playAgain == 'y');

    system("pause");
    return 0;
}
