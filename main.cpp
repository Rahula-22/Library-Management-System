#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;

/*Library Management System
  Developed by: Rahul Ahirwar*/

// BookStatus: This funcition is created for meaningful names like Available, Borrowed, and Reserved to Prevent invalid values.
enum BookStatus { Available, Borrowed, Reserved };

// BorrowedBook Record
struct BorrowedBook {
    string isbn;
    time_t borrowtimestamp;
};

// Helper Function to Clear Input Buffer (Prevents issues with getline after cin >>)
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// statusToText converts enum status to a string.
string statusToText(BookStatus stat) {
    if (stat == Available)
        return "Available";
    else if (stat == Borrowed)
        return "Borrowed";
    else
        return "Reserved";
}

// textToStatus converts a string to a corresponding BookStatus enum.
BookStatus textToStatus(const string &str) {
    if (str == "Available")
        return Available;
    else if (str == "Borrowed")
        return Borrowed;
    else
        return Reserved;
}

// Book Class 
class Book {
    private:
    // Data attributes
    string title;
    string author;
    string publisher;
    int year;
    string ISBN;
    BookStatus status;
    public:
    Book() {}
    Book(string i, string t, string a, string p, int y, BookStatus status = Available)
        : ISBN(i), title(t), author(a), publisher(p), year(y), status(status) {}

    // Display book details
    void display() const {
        cout << "ISBN: " << ISBN << "\nTitle: " << title << "\nAuthor: " << author << "\nPublisher: " << publisher 
        << "\nYear: " << year << "\nStatus: " << statusToText(status) << "\n";
    }
    // Member functions
    string getISBN() const { return ISBN; }

    string getTitle() const { return title; }

    string getAuthor() const { return author; }

    string getPublisher() const { return publisher; }

    int getYear() const { return year; }

    BookStatus getStatus() const { return status; }

    void setStatus(BookStatus st) { status = st; }
};


// Account Class: Manages Borrowed Books and Fines
class Account {
    private:
    vector<BorrowedBook> borrowed;
    double fine;
    public:
    Account() : fine(0.0) {}
    // Add a book borrow record with the current time.
    void addbookrecord(const string &isbn, time_t ts) {
        BorrowedBook book;
        book.borrowtimestamp = ts;
        book.isbn = isbn;
        borrowed.push_back(book);
    }
    // Remove the record for a returned book.
    void removebookrecord(const string &isbn) {
        for (auto x = borrowed.begin(); x != borrowed.end(); ++x) {
            if (x->isbn == isbn) {
                borrowed.erase(x);
                break;
            }
        }
    }
    int countborrowed() const { return borrowed.size(); }

    vector<BorrowedBook> getborrowrecords() const { return borrowed; }

    double getfine() const { return fine; }

    void addfine(double amt) { fine += amt; }

    void clearFine() { fine = 0; }

    // borrow timestamp for a specific book.
    time_t getborrowtime(const string &isbn) const {
        for (auto x : borrowed) {
            if (x.isbn == isbn)
                return x.borrowtimestamp;
        }
        return 0;
    }
};


// User Class and Derived Classes (Student, Faculty, Librarian)
class User {
    protected:
    int userID;
    string name;
    string email;
    string password;
    Account account;
    public:
    User() {}
    virtual ~User() {}

    User(int id, string n, string e, string p)
        : userID(id), name(n), email(e), password(p) {}

    // Pure virtual functions
    virtual void borrowBook(Book &book) = 0;
    virtual void returnBook(Book &book) = 0;

    void display() const {
        cout << "User ID: " << userID << "\nName: " << name << "\nEmail: " << email << "\n";
    }

    // member functions
    int getID() const { return userID; }

    string getName() const { return name; }

    string getEmail() const { return email; }

    Account& getAccount() { return account; }

    string getPassword() const { return password; }

    void setPassword(const string &pwd) { password = pwd; }

    bool verifyPassword(const string &pwd) const { return password == pwd; }
};

// Student class: derived from User
class Student : public User {
    public:
    Student(int id, string n, string e, string p)
        : User(id, n, e, p) {}

    void borrowBook(Book &book) override {
        if (account.countborrowed() >= 3) {
            cout << "Borrow limit reached on your account, max 3 books are allowed.\n";
            return;
        }
        if (account.getfine() > 0) {
            cout << "Please clear remaining fines before borrowing.\n";
            return;
        }
        if (book.getStatus() != Available) {
            cout << "This book is not available right now.\n";
            return;
        }
        book.setStatus(Borrowed);
        time_t currentTime = time(nullptr);

        account.addbookrecord(book.getISBN(), currentTime);

        cout << "Congratulations! Book borrowed Succesfully with ISBN: " << book.getISBN() << "\n";
    }

    void returnBook(Book &book) override {
        time_t borrowTime = account.getborrowtime(book.getISBN());
        time_t currentTime = time(nullptr);

        // For simulation: 1 min = 1 day. So, you can wait for greater than 15 mins and get the fine amount.
        double simulatedDays = difftime(currentTime, borrowTime) / 60;
        if (simulatedDays > 15) { 
            int overdueDays = simulatedDays - 15;
            double fineDue = overdueDays * 10;
            account.addfine(fineDue);
            cout << "Late return! Overdue by " << overdueDays 
                 << " days. Fine imposed of : " << fineDue << " rupees.\n";
        }
        book.setStatus(Available);
        account.removebookrecord(book.getISBN());
        cout << "Book returned successfully with ISBN: " << book.getISBN() << "\n";
    }
};

// Faculty class: derived from User
class Faculty : public User {
    public:
    Faculty(int id, string n, string e, string p)
        : User(id, n, e, p) {}

    void borrowBook(Book &book) override {
        if (account.countborrowed() >= 5) {
            cout << "Borrow limit reached on your account, max 5 books are allowed.\n";
            return;
        }
        if (book.getStatus() != Available) {
            cout << "Book is currently unavailable.\n";
            return;
        }
        book.setStatus(Borrowed);
        time_t currentTime = time(nullptr);

        account.addbookrecord(book.getISBN(), currentTime);
        cout << "Congratulations! Book borrowed Succesfully with ISBN: " << book.getISBN() << "\n";
    }

    void returnBook(Book &book) override {
        time_t borrowTime = account.getborrowtime(book.getISBN());
        time_t currentTime = time(nullptr);

        double simulatedDays = difftime(currentTime, borrowTime) / 60;
        if (simulatedDays > 60) {
            cout << "Book overdue by more than 60 days. Please address this soon.\n";
        }
        book.setStatus(Available);
        account.removebookrecord(book.getISBN());
        cout << "Book returned successfully with ISBN: " << book.getISBN() << "\n";
    }
};

// Librarian class: manages users and books.
class Librarian : public User {
    public:

    Librarian(int id, string n, string e, string p)
        : User(id, n, e, p) {}

    void borrowBook(Book &book) override {
        cout << "Action not allowed.\n";
    }
    void returnBook(Book &book) override {
        cout << "Action not allowed.\n";
    }

    // Methods to add or remove books.
    void addnewbook(vector<Book> &books, const Book &newBook) {
        books.push_back(newBook);
        cout << "New book added with ISBN: " << newBook.getISBN() << "\n";
    }
    void removeexistingbook(vector<Book> &books, const string &isbn) {
        for (auto x = books.begin(); x != books.end(); ++x) {
            if (x->getISBN() == isbn) {
                books.erase(x);
                cout << "Book removed  with ISBN: " << isbn << "\n";
                return;
            }
        }
        cout << "No book found with ISBN: " << isbn << "\n";
    }
};

// Library Class: Handles File I/O
class Library {
    public:
    vector<Book> books;
    vector<User*> users; 

    Library() {}
    ~Library() {
        for (auto userPtr : users)
            delete userPtr;
    }

    void loadBooks(const string &filename = "books.txt") {
        ifstream infile(filename);
        if (!infile.is_open()){
            cout << "Books file not found. Starting with an empty library.\n";
            return;
        }

        string line;
        while(getline(infile, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            string isbn, title, author, publisher, yearStr, statusStr;
            getline(ss, isbn, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, publisher, '|');
            getline(ss, yearStr, '|');
            getline(ss, statusStr, '|');
            int year = stoi(yearStr);
            BookStatus st = textToStatus(statusStr);
            books.push_back(Book(isbn, title, author, publisher, year, st));
        }
        infile.close();
    }

    void saveBooks(const string &filename = "books.txt") {
        ofstream outfile(filename);
        for (auto &book : books) {
            outfile << book.getISBN() << "|" << book.getTitle() << "|" << book.getAuthor() << "|"
                    << book.getPublisher() << "|" << book.getYear() << "|" << statusToText(book.getStatus()) << "\n";
        }
        outfile.close();
    }

    void loadUsers(const string &filename = "users.txt") {
        ifstream infile(filename);
        if (!infile.is_open()){
            cout << "Users file not found. Starting with an empty user list.\n";
            return;
        }
        string line;
        while(getline(infile, line)) {
            if(line.empty()) continue;
            stringstream ss(line);
            string idStr, name, email, role, pwd, fineStr, borrowedRecords;
            getline(ss, idStr, '|');
            getline(ss, name, '|');
            getline(ss, email, '|');
            getline(ss, role, '|');
            getline(ss, pwd, '|');
            getline(ss, fineStr, '|');
            getline(ss, borrowedRecords, '|');

            int id = stoi(idStr);
            double fine = stod(fineStr);
            User *userPtr = nullptr;
            if(role == "Student")
                userPtr = new Student(id, name, email, pwd);
            else if(role == "Faculty")
                userPtr = new Faculty(id, name, email, pwd);
            else if(role == "Librarian")
                userPtr = new Librarian(id, name, email, pwd);

            if (userPtr) {
                if (fine > 0)
                    userPtr->getAccount().addfine(fine);
                // borrowed book records - ISBN:timestamp
                stringstream ssRecords(borrowedRecords);
                string record;
                while(getline(ssRecords, record, ',')) {
                    if (!record.empty()){
                        size_t pos = record.find(':');
                        if(pos != string::npos) {
                            string isbn = record.substr(0, pos);
                            string tsStr = record.substr(pos + 1);
                            time_t ts = stol(tsStr);
                            userPtr->getAccount().addbookrecord(isbn, ts);
                        } else {
                            userPtr->getAccount().addbookrecord(record, 0);
                        }
                    }
                }
                users.push_back(userPtr);
            }
        }
        infile.close();
    }

    void saveUsers(const string &filename = "users.txt") {
        ofstream outfile(filename);
        for (auto user : users) {
            string roleText;
            if (dynamic_cast<Student*>(user))
                roleText = "Student";
            else if (dynamic_cast<Faculty*>(user))
                roleText = "Faculty";
            else if (dynamic_cast<Librarian*>(user))
                roleText = "Librarian";
            else
                roleText = "Unknown";
            
            outfile << user->getID() << "|" << user->getName() << "|" << user->getEmail() 
                    << "|" << roleText << "|" << user->getPassword() << "|" 
                    << user->getAccount().getfine() << "|";
            vector<BorrowedBook> recs = user->getAccount().getborrowrecords();
            for (size_t i = 0; i < recs.size(); i++){
                outfile << recs[i].isbn << ":" << recs[i].borrowtimestamp;
                if(i != recs.size() - 1)
                    outfile << ",";
            }
            outfile << "\n";
        }
        outfile.close();
    }

    Book* findBook(const string &isbn) {
        for (auto &book : books) {
            if (book.getISBN() == isbn)
                return &book;
        }
        return nullptr;
    }

    User* findUser(int id) {
        for (auto user : users) {
            if (user->getID() == id)
                return user;
        }
        return nullptr;
    }

    void showallBooks() {
        for (auto &book : books) {
            book.display();
            cout << "-----------------------\n";
        }
    }

    void showallUsers() {
        for (auto user : users) {
            user->display();
            cout << "Fine: " << user->getAccount().getfine() << "\n";
            vector<BorrowedBook> recs = user->getAccount().getborrowrecords();
            cout << "Borrowed Books: ";
            for (auto &rec : recs)
                cout << rec.isbn << " ";
            cout << "\n------------------------\n";
        }
    }
    void addUser() {
        int newUserID;
        string newName, newEmail, newRole, newP;

        cout << "Enter new User ID: ";
        cin >> newUserID;
        clearInputBuffer();
        cout << "Enter Name: ";
        getline(cin, newName);
        cout << "Enter Email: ";
        getline(cin, newEmail);
        cout << "Enter Role (Student/Faculty/Librarian): ";
        getline(cin, newRole);
        cout << "Enter Password: ";
        getline(cin, newP);

        User* newUser = nullptr;
        if (newRole == "Student")
            newUser = new Student(newUserID, newName, newEmail, newP);
        else if (newRole == "Faculty")
            newUser = new Faculty(newUserID, newName, newEmail, newP);
        else if (newRole == "Librarian")
            newUser = new Librarian(newUserID, newName, newEmail, newP);
        else {
            cout << "Invalid role entered.\n";
            return;
        }

        users.push_back(newUser);
        cout << "User added successfully!\n";
    }

    void removeUser(int currentUserID) {
        int remUserID;
        cout << "Enter the User ID to remove: ";
        cin >> remUserID;
        clearInputBuffer();

        bool removed = false;
        for (auto it = users.begin(); it != users.end(); ++it) {
            if ((*it)->getID() == remUserID) {
                if ((*it)->getID() == currentUserID) {
                    cout << "You cannot remove yourself while logged in.\n";
                    removed = true;
                    break;
                }
                delete *it;
                users.erase(it);
                cout << "User removed successfully.\n";
                removed = true;
                break;
            }
        }

        if (!removed)
            cout << "User with the specified ID was not found.\n";
    }
};


// Main Function
int main() {
    Library librarySystem; // Library System Object
    
    // Load data from files (Assuming we have books.txt and users.txt in the same directory)
    librarySystem.loadBooks();
    librarySystem.loadUsers();

    cout<<"\nWelcome to the Library Management System!"<<endl;
    int role;
    cout << "\nPlease select your role:\n1. Librarian\n2. Student/Faculty\nEnter your choice: ";
    cin >> role;
    clearInputBuffer();

    int userID;
    if (role == 1) {
        cout << "Enter your Librarian User ID: ";
        cin >> userID;
        clearInputBuffer();
    } else if (role == 2) {
        cout << "Enter your User ID: ";
        cin >> userID;
        clearInputBuffer();
    } else {
        cout << "Invalid role selection. Exiting the System...\n";
        return 0;
    }

    User* currentUser = librarySystem.findUser(userID);
    if (!currentUser) {
        cout << "User not found. Please check your User ID or Contact the librarian\n";
        return 0;
    }
    
    // Password and verification.
    string Password;
    cout << "Enter your password: ";
    getline(cin, Password);
    if (!currentUser->verifyPassword(Password)) {
        cout << "Incorrect password provided. Exiting the System...\n";
        return 0;
    }
    
    // Ensure the role selection matches the user's type.
    if (role == 1 && !dynamic_cast<Librarian*>(currentUser)) {
        cout << "The provided User ID does not belong to a Librarian. Exiting the System...\n";
        return 0;
    }
    if (role == 2 && dynamic_cast<Librarian*>(currentUser)) {
        cout << "The provided User ID belongs to a Librarian. Please log in under the Librarian role. Exiting the System...\n";
        return 0;
    }
    
    cout << "Welcome, " << currentUser->getName() << "!\n";

    bool logout = false;
    while (!logout) {
        if (dynamic_cast<Librarian*>(currentUser)) {
            cout << "\n----- Librarian Menu -----\n";
            cout << "1. Add Book\n2. Remove Book\n3. Add User\n4. Remove User\n5. List All Books\n6. List All Users\n7. Logout and Save\nChoice: ";
            int opt;
            cin >> opt;
            clearInputBuffer();
            Librarian* librarianPtr = dynamic_cast<Librarian*>(currentUser);
            switch (opt) {
                case 1: {
                    string isbn, title, author, publisher;
                    int year;
                    cout << "Enter ISBN: ";
                    getline(cin, isbn);
                    cout << "Enter Title: ";
                    getline(cin, title);
                    cout << "Enter Author: ";
                    getline(cin, author);
                    cout << "Enter Publisher: ";
                    getline(cin, publisher);
                    cout << "Enter Year: ";
                    cin >> year;
                    clearInputBuffer();
                    Book newBook(isbn, title, author, publisher, year);
                    librarianPtr->addnewbook(librarySystem.books, newBook);
                    break;
                }
                case 2: {
                    string isbn;
                    cout << "Enter ISBN of the book to remove: ";
                    getline(cin, isbn);
                    librarianPtr->removeexistingbook(librarySystem.books, isbn);
                    break;
                }
                case 3: {
                    librarySystem.addUser();
                    break;
                }
                case 4: {
                    librarySystem.removeUser(currentUser->getID());
                    break;
                }
                case 5: {
                    librarySystem.showallBooks();
                    break;
                }
                case 6: {
                    librarySystem.showallUsers();
                    break;
                }
                case 7: {
                    logout = true;
                    break;
                }
                default:
                    cout << "Invalid option. Please try again.\n";
            }
        } else {  // Menu for Students/Faculty
            cout << "\n----- User Menu -----\n";
            cout << "1. Borrow Book\n2. Return Book\n3. View Account Details\n4. Pay Fine\n5. List All Books\n6. Logout\nChoice: ";
            int opt;
            cin >> opt;
            clearInputBuffer();
            switch (opt) {
                case 1: {
                    string isbn;
                    cout << "Enter ISBN of the book to borrow: ";
                    getline(cin, isbn);
                    Book* bookPtr = librarySystem.findBook(isbn);
                    if (bookPtr)
                        currentUser->borrowBook(*bookPtr);
                    else
                        cout << "No book found with the provided ISBN.\n";
                    break;
                }
                case 2: {
                    string isbn;
                    cout << "Enter ISBN of the book to return: ";
                    getline(cin, isbn);
                    Book* bookPtr = librarySystem.findBook(isbn);
                    if (bookPtr)
                        currentUser->returnBook(*bookPtr);
                    else
                        cout << "No book found with the provided ISBN.\n";
                    break;
                }
                case 3: {
                    currentUser->display();
                    cout << "Current Fine: " << currentUser->getAccount().getfine() << "\n";
                    vector<BorrowedBook> records = currentUser->getAccount().getborrowrecords();
                    cout << "Borrowed Books: ";
                    for (auto &rec : records)
                        cout << rec.isbn << " ";
                    cout << "\n";
                    break;
                }
                case 4: {
                    cout << "Paying off fine. Fine before payment: " << currentUser->getAccount().getfine() << "\n";
                    currentUser->getAccount().clearFine();
                    cout << "All fines cleared.\n";
                    break;
                }
                case 5: {
                    librarySystem.showallBooks();
                    break;
                }
                case 6: {
                    logout = true;
                    break;
                }
                default:
                    cout << "Invalid option. Please try again.\n";
            }
        }
    }

    // Save all changes to files before exiting.
    librarySystem.saveBooks();
    librarySystem.saveUsers();
    cout << "Thankyou! Exiting the System...\n";
    return 0;
}
