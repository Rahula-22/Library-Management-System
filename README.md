# Library Management System

This project implements a Library Management System in C++ using Object-Oriented Programming (OOP) principles. It supports multiple user roles such as Students, Faculty, and Librarians and uses file-based persistence for books and user data.

## Overview

The system allows:
- **Students** to borrow up to 3 books at a time for a simulated period of 15 days (1 minute = 1 simulated day). If a book is returned late, a fine of 10 rupees is applied per simulated day overdue.
- **Faculty** to borrow up to 5 books for a simulated period of 30 days. Faculty do not incur fines but receive a warning if a book is returned after 60 simulated days.
- **Librarians** to manage the library by adding/removing books and users. Librarians do not borrow or return books.

## Features

- **Role-Based Access Control:**
  - **Students/Faculty:** Borrow/return books, view account details, and pay fines.
  - **Librarians:** Manage the library, including adding or removing books and users.
- **Password Authentication:** Each user has a password used during login.
- **Data Persistence:**  
  Books and users are stored in books.txt and users.txt files, respectively.
- **Simulated Time:**  
  For testing purposes, the system simulates the passage of days in minutes (1 minute = 1 simulated day).

## File Structure

- **main.cpp:** Contains the complete C++ source code.
  
- **books.txt:** Stores book records. It contains the records for each book (ISBN, title, author, publisher, year, and status). Each line is in the format:  
  `ISBN|Title|Author|Publisher|Year|Status`
  
- **users.txt:** Stores user records. It contains user information (User ID, name, email, role, password, and borrowing details). Each line is in the format:  
  `UserID|Name|Email|Role|Password|Fine|BorrowedRecords`
  
  *BorrowedRecords* are in the format `ISBN:borrowtimestamp` (multiple records are separated by commas).
  
- **README.md:** This file.

## 1. How to Run the Program

### Prerequisites
- A C++ compiler supporting C++11 or later
- A terminal or command prompt

### Build and Run Instructions

1. **Open a terminal** and navigate to your project folder or you can directly open the file in the termial (using option menu).
2. **Compile the program**:
   g++ -std=c++11 -o main main.cpp
3. **Run the executable**:
   ./main

## 2. Data Initialization and Persistence
### How Data is Loaded
- On startup, the program checks for books.txt and users.txt:
If found, it reads and reconstructs the book and user objects.
If not found, You have to upload both files in the same directry as the main file.

## 3. Default Credentials
- For testing purposes, the system comes preloaded with default login credentials:

**Student Accounts**: For example, User ID: 101 with Password: pass101 (Other student IDs might be available as well).

**Faculty Accounts**: For example, User ID: 201 with Password: pass201 (Additional faculty IDs may be included).

**Librarian (Admin) Account**: User ID: 301 with Password: pass301

## 4. Code Structure and Workflow
**Key Components**
**Book Class**:
- Manages details about books (including status) and handles serialization for file I/O.

**User Class Hierarchy** (Student, Faculty, Librarian):
- Provides authentication and role-specific borrowing/returning logic. Each user type implements unique policies regarding borrowing limits and fine calculations.

**Account Class**: 
- Manages record of the books borrowed and fines imposed.It includes addbookrecord, removebookrecord, and fine realted functions.

**Library Class**:
- Aggregates books and users, and implements functions for adding, removing, issuing, and returning books. It also handles loading and saving data to books.txt and users.txt.

### Program Flow
- Startup: The program loads data from books.txt and users.txt.

- User Authentication: Users choose their role, enter their User ID and password, and then the system validates the credentials.

- Operations: Based on the logged-in user's role, the program displays a specific menu (for borrowing/returning books or managing the library).

*Data Persistence*: All changes are saved back to the files when the user exit the program.

## 5. Simulated Time for Fine Calculation
- To facilitate testing without waiting for days, the system simulates time by treating 1 minute as 1 day. For example, if a student has 15 days to return a book, it will be 15 minutes in this simulation. Fines and due date checks are based on this simulated time.

# Conclusion
This Library Management System is designed using modern C++ techniques, demonstrating object-oriented programming concepts like inheritance and polymorphism, as well as practical file I/O for data persistence. Enjoy managing your library and feel free to extend the system further as needed.

*Happy Coding!*
