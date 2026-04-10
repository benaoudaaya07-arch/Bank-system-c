# Bank Management System in C

## Author
Aya Benaouda  
First-Year Computer Science — 2025/2026

## Description
This program simulates a simple banking system using the C programming language.  

Users can:
- Create new accounts  
- Deposit money  
- Withdraw money  
- Transfer money between accounts  
- View account information  
- Close accounts  

The program stores account data in arrays and provides a menu-driven interface for easy interaction. It also includes PIN verification and basic error handling to ensure security and correctness.

## Features
- Maximum of 50 accounts  
- Unique account ID for each user  
- PIN verification for account security  
- Error handling for invalid ID, insufficient balance, and other invalid operations

  ## How to Compile
 
 gcc -Wall -Wextra -std=c11 main.c -o bank.exe

## How to Run

./bank      # On Linux or Mac
bank.exe    # On Windows

## Test Cases
The file `test_cases.txt` contains multiple test scenarios (minimum 5), showcasing different functionalities of the system such as account creation, deposits, withdrawals, transfers, and error handling.

## File Structure
- main.c → Contains the entire program
- README.md → Project description
- test_cases.txt → Sample inputs and outputs
