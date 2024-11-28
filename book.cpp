#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Enum for Genre
enum Genre
{
    FICTION,
    NON_FICTION,
    PERIODICAL,
    BIOGRAPHY,
    CHILDREN
};

// Function to convert genre to string
string genreToString(Genre genre)
{
    switch (genre)
    {
    case FICTION:
        return "Fiction";
    case NON_FICTION:
        return "Non-Fiction";
    case PERIODICAL:
        return "Periodical";
    case BIOGRAPHY:
        return "Biography";
    case CHILDREN:
        return "Children";
    default:
        return "Unknown";
    }
}

// Class for Book
class Book
{
private:
    string ISBN;
    string title;
    string author;
    string copyrightDate;
    bool checkedOut;
    Genre genre;

public:
    Book(string isbn, string t, string a, string date, Genre g)
        : ISBN(isbn), title(t), author(a), copyrightDate(date), checkedOut(false), genre(g) {}

    string getISBN() const { return ISBN; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    Genre getGenre() const { return genre; }
    bool isCheckedOut() const { return checkedOut; }

    void checkOut() { checkedOut = true; }
    void checkIn() { checkedOut = false; }

    bool operator==(const Book &other) const { return ISBN == other.ISBN; }

    friend ostream &operator<<(ostream &os, const Book &book)
    {
        os << "Title: " << book.title << "\n"
           << "Author: " << book.author << "\n"
           << "ISBN: " << book.ISBN << "\n"
           << "Genre: " << genreToString(book.genre) << "\n";
        return os;
    }
};

// Class for Patron
class Patron
{
private:
    string userName;
    string cardNumber;
    int owedFees;

public:
    Patron(string name, string cardNum)
        : userName(name), cardNumber(cardNum), owedFees(0) {}

    string getUserName() const { return userName; }
    string getCardNumber() const { return cardNumber; }
    int getOwedFees() const { return owedFees; }
    bool owesFees() const { return owedFees > 0; }

    void setFees(int fees) { owedFees = fees; }
};

// Class for Transaction
class Transaction
{
private:
    Book book;
    Patron patron;
    string activity; // "check out" or "check in"
    string date;

public:
    Transaction(Book b, Patron p, string act, string d)
        : book(b), patron(p), activity(act), date(d) {}

    void displayTransaction() const
    {
        cout << "Transaction: " << activity << "\n"
             << "Book: " << book.getTitle() << "\n"
             << "Patron: " << patron.getUserName() << "\n"
             << "Date: " << date << "\n";
    }
};

// Class for Library
class Library
{
private:
    vector<Book> books;
    vector<Patron> patrons;
    vector<Transaction> transactions;

public:
    void addBook(Book book)
    {
        for (const auto &b : books)
        {
            if (b.getISBN() == book.getISBN())
            {
                cout << "Error: A book with this ISBN already exists.\n";
                return;
            }
        }
        books.push_back(book);
        cout << "Book added successfully.\n";
    }

    void addPatron(Patron patron) { patrons.push_back(patron); }

    void checkOutBook(string isbn, string cardNum)
    {
        Book *bookPtr = nullptr;
        for (auto &book : books)
        {
            if (book.getISBN() == isbn)
            {
                bookPtr = &book;
                break;
            }
        }

        Patron *patronPtr = nullptr;
        for (auto &patron : patrons)
        {
            if (patron.getCardNumber() == cardNum)
            {
                patronPtr = &patron;
                break;
            }
        }

        if (!bookPtr)
        {
            cout << "Error: Book with ISBN " << isbn << " not found.\n";
            return;
        }
        if (!patronPtr)
        {
            cout << "Error: Patron with card number " << cardNum << " not found.\n";
            return;
        }
        if (patronPtr->owesFees())
        {
            cout << "Error: Patron " << patronPtr->getUserName() << " owes fees.\n";
            return;
        }
        if (bookPtr->isCheckedOut())
        {
            cout << "Error: Book is already checked out.\n";
            return;
        }

        bookPtr->checkOut();
        transactions.emplace_back(*bookPtr, *patronPtr, "check out", "YYYY-MM-DD");
        cout << "Book checked out successfully.\n";
    }

    void checkInBook(string isbn)
    {
        Book *bookPtr = nullptr;
        for (auto &book : books)
        {
            if (book.getISBN() == isbn)
            {
                bookPtr = &book;
                break;
            }
        }

        if (!bookPtr)
        {
            cout << "Error: Book with ISBN " << isbn << " not found.\n";
            return;
        }
        if (!bookPtr->isCheckedOut())
        {
            cout << "Error: Book is not checked out.\n";
            return;
        }

        bookPtr->checkIn();
        transactions.emplace_back(*bookPtr, Patron("", ""), "check in", "YYYY-MM-DD");
        cout << "Book checked in successfully.\n";
    }

    void displayTransactions() const
    {
        if (transactions.empty())
        {
            cout << "No transactions to display.\n";
        }
        else
        {
            cout << "Transactions:\n";
            for (const auto &transaction : transactions)
            {
                transaction.displayTransaction();
                cout << endl;
            }
        }
    }

    void displayPatronsOwingFees() const
    {
        cout << "Patrons owing fees:\n";
        bool found = false; // Flag to check if any patron owes fees
        for (const auto &patron : patrons)
        {
            if (patron.owesFees())
            {
                cout << patron.getUserName() << " owes " << patron.getOwedFees() << " fees.\n";
                found = true;
            }
        }
        if (!found)
        {
            cout << "No patrons owe fees.\n";
        }
    }
};

// Dashboard function
void dashboard(Library &library)
{
    int choice;
    while (true)
    {
        cout << "\nLibrary Dashboard\n";
        cout << "1. Add a Book\n";
        cout << "2. Add a Patron\n";
        cout << "3. Check Out a Book\n";
        cout << "4. Check In a Book\n";
        cout << "5. Display Patrons Who Owe Fees\n";
        cout << "6. Display Transactions\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 7)
            break;

        string isbn, title, author, cardNum, date;
        int genre;
        switch (choice)
        {
        case 1:
            cout << "Enter ISBN: ";
            cin >> isbn;
            cout << "Enter Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
            cout << "Enter Copyright Date as YYYY-MM-DD: ";
            cin >> date;
            cout << "Enter Genre (0: Fiction, 1: Non-Fiction, 2: Periodical, 3: Biography, 4: Children): ";
            cin >> genre;
            if (genre <= 0 || genre >= 4)
            {
                cout << "Error: Unknown genre choice.\n";
            }
            else
            {
                library.addBook(Book(isbn, title, author, date, static_cast<Genre>(genre)));
            }
            break;

        case 2:
            cout << "Enter User Name: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Card Number: ";
            cin >> cardNum;
            library.addPatron(Patron(title, cardNum));
            cout << "Patron added successfully.\n";
            break;

        case 3:
            cout << "Enter ISBN of the book to check out: ";
            cin >> isbn;
            cout << "Enter Patron's Card Number: ";
            cin >> cardNum;
            library.checkOutBook(isbn, cardNum);
            break;

        case 4:
            cout << "Enter ISBN of the book to check in: ";
            cin >> isbn;
            library.checkInBook(isbn);
            break;

        case 5:
            library.displayPatronsOwingFees();
            break;

        case 6:
            library.displayTransactions();
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

// Main function
int main()
{
    Library library;
    dashboard(library);
    return 0;
}