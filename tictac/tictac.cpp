#include <iostream>
#include <limits>
using namespace std;

char square[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void PrintMessage(string message, bool printTop = true, bool printBottom = true)
{
    if (printTop)
    {
        cout << "+-----------------------------+" << endl;
        cout << "|";
    }
    else
    {
        cout << "|";
    }
    bool front = true;
    for (int i = message.length(); i < 29; i++)
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
        cout << "+-----------------------------+" << endl;
    }
    else
    {
        cout << "|" << endl;
    }
}

int Checkwin()
{
    if (square[1] == square[2] && square[2] == square[3])
    {
        return 1;
    }
    else if (square[4] == square[5] && square[5] == square[6])
    {
        return 1;
    }
    else if (square[7] == square[8] && square[8] == square[9])
    {
        return 1;
    }
    else if (square[1] == square[4] && square[4] == square[7])
    {
        return 1;
    }
    else if (square[2] == square[5] && square[5] == square[8])
    {
        return 1;
    }
    else if (square[3] == square[6] && square[6] == square[9])
    {
        return 1;
    }
    else if (square[1] == square[5] && square[5] == square[9])
    {
        return 1;
    }
    else if (square[3] == square[5] && square[5] == square[7])
    {
        return 1;
    }
    else if (square[1] != '1' && square[2] != '2' && square[3] != '3' && square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7] != '7' && square[8] != '8' && square[9] != '9')
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void Board()
{
    cout << "+-----------------------------+" << endl;
    cout << "|         TIC TAC TOE         |" << endl;
    cout << "-------------------------------" << endl;
    cout << "|         |         |         |" << endl;
    cout << "|    " << square[1] << "    |    " << square[2] << "    |    " << square[3] << "    |" << endl;
    cout << "|---------|---------|---------|" << endl;
    cout << "|         |         |         |" << endl;
    cout << "|    " << square[4] << "    |    " << square[5] << "    |    " << square[6] << "    |" << endl;
    cout << "|---------+---------+---------+" << endl;
    cout << "|         |         |         |" << endl;
    cout << "|    " << square[7] << "    |    " << square[8] << "    |    " << square[9] << "    |" << endl;
}

void ResetBoard()
{
    for (int i = 1; i <= 9; i++)
    {
        square[i] = '0' + i; // Reset the board to initial state
    }
}

bool IsValidInput(int choice)
{
    // Check if the choice is a valid position (1-9) and the position is available
    return (choice >= 1 && choice <= 9 && square[choice] == '0' + choice);
}

int main()
{
    int player = 1, i, choice;
    char mark;
    char playAgain;

    do
    {
        // Reset the board before each round
        ResetBoard();

        do
        {
            system("cls");
            Board();
            player = (player % 2) ? 1 : 2;
            PrintMessage("Player " + to_string(player) + ", enter a number:__");
            while (true)
            {
                cin >> choice;

                // Check if the input is an integer and is a valid move
                if (cin.fail() || !IsValidInput(choice))
                {
                    cin.clear();                                         // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                    PrintMessage("Invalid input! Please enter a valid number (1-9) that is not already taken.");
                }
                else
                {
                    break; // Valid input, break the loop
                }
            }

            mark = (player == 1) ? 'X' : 'O';

            square[choice] = mark; // Place the mark on the board

            i = Checkwin(); // Check if there's a winner or a draw
            player++;
        } while (i == -1);

        system("cls");
        Board();

        if (i == 1)
        {
            PrintMessage("CONGZ! Player " + to_string(--player) + ", WINS!");
        }
        else
        {
            PrintMessage("It's a draw!");
        }

        // Handle invalid replay input
        while (true)
        {
            cout << "Do you want to play again? (Y/N): ";
            cin >> playAgain;
            if (playAgain == 'Y' || playAgain == 'y' || playAgain == 'N' || playAgain == 'n')
                break; // Valid input
            else
                cout << "Invalid choice! Please enter 'Y' for yes or 'N' for no." << endl;
        }

    } while (playAgain == 'Y' || playAgain == 'y');

    return 0;
}
