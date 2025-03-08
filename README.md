# Library Management System

This project implements a Library Management System in C++ using Object-Oriented Programming (OOP) principles. It supports multiple user roles such as Students, Faculty, and Librarians and uses file-based persistence for books and user data.

## Overview

The system allows:
- **Students** to borrow up to 3 books at a time for a simulated period of 15 days (1 minute = 1 simulated day). If a book is returned late, a fine of 10 rupees is applied per simulated day overdue.
- **Faculty** to borrow up to 5 books for a simulated period of 30 days. Faculty do not incur fines but receive a warning if a book is returned after 60 simulated days.
- **Librarians** to manage the library by adding/removing books and users. Librarians do not borrow or return books.

Each user logs in with a unique ID and password. The system simulates time for testing purposes, with 60 seconds representing one day.

## Features

- **Role-Based Access Control:**
  - **Students/Faculty:** Borrow/return books, view account details, and pay fines.
  - **Librarians:** Manage the library, including adding or removing books and users.
- **Password Authentication:** Each user has a password used during login.
- **Data Persistence:**  
  Books and users are stored in `books.txt` and `users.txt` files, respectively.
- **Simulated Time:**  
  For testing purposes, the system simulates the passage of days in minutes (1 minute = 1 simulated day).

## File Structure

- **main.cpp:** Contains the complete C++ source code.
  
- **books.txt:** Stores book records. Each line is in the format:  
  `ISBN|Title|Author|Publisher|Year|Status`
  
- **users.txt:** Stores user records. Each line is in the format:  
  `UserID|Name|Email|Role|Password|Fine|BorrowedRecords`
  
  *BorrowedRecords* are in the format `ISBN:borrowtimestamp` (multiple records are separated by commas).
  
- **Makefile:** Contains build instructions.
  
- **README.md:** This file.

