/*

 * Class: CMSC226 CRN

 * Instructor: Dr. Grinberg

 * Project 9

 * Description: This project is designed to be a cashier, with a lot of different features including inventory management

 * Due Date: 11/17/2019

 * I pledge that I have completed the programming assignment independently.

   I have not copied the code from a student or any source.

   I have not given my code to any student.

   Print your Name here: __James Carter Garrett________

 */

#include <stdlib.h>
#include "mainmenu.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;

double SoldBook::taxRate = 1.06;
double SoldBook::total = 0;
//ISBN, Title, Author, Publisher, Date Added, Quantity-On-Hand, Wholesale Cost, Retail Price
fstream dataFile;

BookData newBook[20];
InventoryBook secondaryInfo[20];

int main() {
	int userInput = 0, counter = 0, iTemp = 0;
	double dTemp;
	string fileStr;

	//Copies the file information over into newBook and secondaryInfo

	try {
		openFileR("InventoryDatabase.txt");

		getline(dataFile, fileStr, '~');
		while (!dataFile.eof()) {
			//ISBN
			getline(dataFile, fileStr, ',');
			strUpper(&fileStr);
			newBook[counter].setISBN(&fileStr, counter);

			//Title
			getline(dataFile, fileStr, ',');
			strUpper(&fileStr);
			newBook[counter].setTitle(&fileStr, counter);

			//Author
			getline(dataFile, fileStr, ',');
			strUpper(&fileStr);
			newBook[counter].setAuthor(&fileStr, counter);

			//Publisher
			getline(dataFile, fileStr, ',');
			strUpper(&fileStr);
			newBook[counter].setPub(&fileStr, counter);

			//Date Added
			getline(dataFile, fileStr, ',');
			strUpper(&fileStr);
			secondaryInfo[counter].setDateAdded(&fileStr, counter);

			//Quantity - On - Hand
			getline(dataFile, fileStr, ',');
			iTemp = atoi(fileStr.c_str());
			secondaryInfo[counter].setQty(iTemp, counter);

			//Wholesale
			getline(dataFile, fileStr, ',');
			dTemp = atof(fileStr.c_str());
			secondaryInfo[counter].setWholesale(dTemp, counter);

			//Retail
			getline(dataFile, fileStr);
			dTemp = atof(fileStr.c_str());
			secondaryInfo[counter].setRetail(dTemp, counter);

			//Sets to the next line?
			getline(dataFile, fileStr, '~');
			counter++;
		}
		dataFile.close();
	}
	catch (string error) {
		cout << "\nERROR: FILE COULD NOT BE FOUND. EXITING NOW..." << endl;
		exit(1);
	}
	
	while (userInput != 4) {
		cout << "Serendipity Booksellers" << endl;
		cout << "\tMain Menu" << endl;
		cout << "\n1. Cashier Module" << endl;
		cout << "2. Inventory Database Module" << endl;
		cout << "3. Report Module" << endl;
		cout << "4. Exit" << endl;

		cout << "\nEnter Your Choice: ";
		cin >> userInput;
		cin.ignore();

		switch (userInput) {
		case 1:
			cashier();
			break;
		case 2:
			invMenu();
			break;
		case 3:
			reports();
			break;
		case 4:
			cout << "You selected item 4." << endl;
			exit(0);
		default:
			cout << "ERROR: INVALID INPUT ~ Please enter a number between 1 and 4" << endl;
		}
	}

	return 0;
}

int cashier() {
	const double SALES_TAX = 1.06;
	const int ITEM_LIMIT = 20;

	string date, cashierISBN, cashierTitle;
	double differentTitles;

	char userChoice;
	int bookQuantity, counter = 0, bookPosition, temp;
	double bookUnitPrice, merchTotal = 0, individualBookTotal, merchTax, total = 0;
	bool stopRunning = false, isbnValid = false;

	cout << "\nSerendipity Book Sellers" << endl;
	cout << " Cashier Module" << endl;

	cout << "\nEnter the Date (MM/DD/YY): ";
	getline(cin, date);

	cout << "\nEnter the amount of titles the customer is purchasing: ";
	cin >> differentTitles;
	cin.ignore();

	if (differentTitles > 0) {
		SoldBook* seperateTitles;
		try {
			seperateTitles = new SoldBook[differentTitles];
		}
		catch (bad_alloc e) {
			cout << "\nSYSTEM FAILED TO ALLOCATE ENOUGH MEMORY. ENDING PROGRAM...";
			exit(2);
		}

		do {
			cout << "\nEnter the quantity of books being purchased: ";
			cin >> bookQuantity;
			cin.ignore();
			seperateTitles[counter].setQtySold(bookQuantity);

			while (isbnValid == false) {
				cout << "\nEnter the book's ISBN number: ";
				getline(cin, cashierISBN);
				seperateTitles[counter].setSoldISBN(cashierISBN);


				for (int i = 0; i < 20; i++) {
					if (seperateTitles[counter].getSoldISBN() == newBook[i].getISBN()) {
						isbnValid = true;
						bookPosition = i;
						break;
					}
				}

				if (isbnValid == true) {
					break;
				}

				else {
					cout << "The program cannot locate the ISBN number in our library. Would you like to re-enter the number? (Y or N): ";
					cin.get(userChoice);
					cin.ignore();

					if (userChoice == 'y' || userChoice == 'Y') {
						continue;
					}

					else if (userChoice == 'n' || userChoice == 'N') {
						return 0;
					}

					else {
						cout << "\nINVALID INPUT. RETURNING TO MAIN MENU..." << endl;
						return 0;
					}
				}
			}
			isbnValid = false;

			if (seperateTitles[counter].getQtySold() <= secondaryInfo[bookPosition].getQty()) {
				seperateTitles[counter].setSoldTitle(newBook[bookPosition].getTitle());
				bookUnitPrice = secondaryInfo[bookPosition].getRetail();
				seperateTitles[counter].setUnitPrices(bookUnitPrice);

				//Removes books from stock
				temp = secondaryInfo[bookPosition].getQty();
				temp -= seperateTitles[counter].getQtySold();
				secondaryInfo[bookPosition].setQty(temp, bookPosition);

				individualBookTotal = (seperateTitles[counter].getQtySold() * seperateTitles[counter].getUnitPrices());
				merchTax = individualBookTotal * seperateTitles[counter].getTaxRate();

				seperateTitles[counter].setTax(merchTax - individualBookTotal);
				seperateTitles[counter].setSubtotal((seperateTitles[counter].getUnitPrices() * seperateTitles[counter].getQtySold()) +
					seperateTitles[counter].getTax());

				seperateTitles[counter].setTotal(seperateTitles[counter].getTotal() + seperateTitles[counter].getSubtotal());

				counter++;
			}

			else {
				cout << "There are not enough copies in stock to fulfill this order. Returning to the main menu..." << endl;
				return 0;
			}

		}

		while (stopRunning == false && counter != differentTitles);

		cout << "\n\nSerendipity Book Sellers" << endl;
		cout << "Date: " << date << endl;
		cout << "Qty\tISBN\t\tTitle\t\tPrice\tTax\tSubtotals" << endl;
		cout << "___________________________________________________________" << endl;

		for (int i = 0; i < counter; i++) {
			cout << seperateTitles[i].getQtySold() << " " << seperateTitles[i].getSoldISBN() << " " << seperateTitles[i].getSoldTitle() <<
				"\t$" << setprecision(2) << fixed << seperateTitles[i].getUnitPrices() << "\t$" << seperateTitles[i].getTax() << "\t$" <<
				seperateTitles[i].getSubtotal() << endl;
		}

		cout << "\n\tTotal" << "\t\t\t\t\t\t$" << seperateTitles->getTotal() << endl;

		cout << "Thank You for Shopping at Serendipity!" << endl;

		//Write changes to file
		try {
			openFileP("InventoryDatabase.txt");
				for (int i = 0; i < 20; i++) {
					dataFile << i + 1 << "~" <<
						newBook[i].getISBN() << "," <<
						newBook[i].getTitle() << "," <<
						newBook[i].getAuthor() << "," <<
						newBook[i].getPub() << "," <<
						secondaryInfo[i].getDateAdded() << "," <<
						secondaryInfo[i].getQty() << "," <<
						secondaryInfo[i].getWholesale() << "," <<
						secondaryInfo[i].getRetail() << endl;
				}
			dataFile.close();
		}

		catch (string error) {
			cout << "\nERROR: FILE COULD NOT BE FOUND. EXITING NOW..." << endl;
			exit(1);
		}

		//Destructor
		delete[] seperateTitles;

		return 0;
	}
	
	else {
		cout << "\nGoing back to menu..." << endl;
		return 0;
	}
}

int invMenu() {
	int userChoice = 0;

	while (userChoice != 5) {

		cout << "\nSerendipity Booksellers" << endl;
		cout << "  Inventory Database" << endl;
		cout << "\n1. Look Up a Book" << endl;
		cout << "2. Add a Book" << endl;
		cout << "3. Edit a Book's record" << endl;
		cout << "4. Delete a Book" << endl;
		cout << "5. Return to the Main Menu" << endl;

		cout << "\nEnter Your Choice: ";
		cin >> userChoice;
		cin.ignore();

		switch (userChoice) {
		case 1:
			lookUpBook();
			break;
		case 2:
			addBook();
			break;
		case 3:
			editBook();
			break;
		case 4:
			deleteBook();
			break;
		case 5:
			cout << "You selected item 5." << endl << endl;
			break;
		default:
			cout << "ERROR: INVALID INPUT ~ Please enter a number between 1 and 5." << endl;
		}
	}

	return 0;
}

int bookInfo(char isbn[14], char title[51], char author[31], char publisher[31], char date[11], int qty, double wholesale, double retail) {

	cout << "\t\tSerendipity Booksellers" << endl;
	cout << "\t\t  Book Information" << endl;
	cout << "\nISBN: " << isbn << endl;
	cout << "Title: " << title << endl;
	cout << "Author: " << author << endl;
	cout << "Publisher: " << publisher << endl;
	cout << "Date Added: " << date << endl;
	cout << "Quantity-On-Hand: " << qty << endl;
	cout << "Wholesale Cost: " << wholesale << endl;
	cout << "Retail Price: " << retail << endl;

	return 0;
}

int reports() {
	int userChoice = 0;

	while (userChoice != 7) {
		cout << "\nSerendipity BookSellers" << endl;
		cout << "\tReports" << endl;

		cout << "\n1. Inventory Listing" << endl;
		cout << "2. Inventory Wholesale Value" << endl;
		cout << "3. Inventory Retail Value" << endl;
		cout << "4. Listing by Quantity" << endl;
		cout << "5. Listing by Cost" << endl;
		cout << "6. Listing by Age" << endl;
		cout << "7. Return to Main Menu" << endl;

		cout << "\nEnter Your Choice: ";
		cin >> userChoice;

		switch (userChoice) {
		case 1:
			repListing();
			break;
		case 2:
			repWholesale();
			break;
		case 3:
			repRetail();
			break;
		case 4:
			repQty();
			break;
		case 5:
			repCost();
			break;
		case 6:
			repAge();
			break;
		case 7:
			cout << "\nYou selected item 7." << endl;
			return 0;
		default:
			cout << "ERROR: INVALID INPUT ~ Please enter a number between 1 and 7." << endl;
		}
	}
	return 0;
}

void strUpper(string* str) {
	transform(str->begin(), str->end(), str->begin(), ::toupper);
}

void lookUpBook() {
	string bookName;
	char* strptr = nullptr;
	char userInput;
	char nameHolderPrevious[51] = {};

	cout << "\nEnter the name of the book: ";
	getline(cin, bookName);
	strUpper(&bookName);

	for (int i = 0; i < 20; i++) {
		if (i == 19 && newBook[i].bookMatch(bookName) == false) {
			cout << "\nTHE BOOK IS NOT IN THE INVENTORY. EXITING NOW...";
			exit(0);
		}

		else if (newBook[i].bookMatch(bookName) == true) {
			if (strcmp(newBook[i].getTitle().c_str(), nameHolderPrevious) == 0) {
				cout << "\nNO OTHER BOOKS WITH THAT NAME. EXITING NOW...";
				exit(0);
			}

			do {
				cout << "\nIs " << newBook[i].getTitle() << " the book you were looking for? (Y or N): ";
				cin.get(userInput);
				cin.ignore();
			} while (userInput != 'y' && userInput != 'Y' && userInput != 'n' && userInput != 'N');

			if (userInput == 'y' || userInput == 'Y') {
				//BOOKINFO TEMPS
				char iTemp[14], tTemp[51], aTemp[31], pTemp[31], dTemp[11];
				strcpy_s(iTemp, newBook[i].getISBN().c_str());
				strcpy_s(tTemp, newBook[i].getTitle().c_str());
				strcpy_s(aTemp, newBook[i].getAuthor().c_str());
				strcpy_s(pTemp, newBook[i].getPub().c_str());
				strcpy_s(dTemp, secondaryInfo[i].getDateAdded().c_str());

				bookInfo(iTemp, tTemp, aTemp, pTemp, dTemp,
					secondaryInfo[i].getQty(), secondaryInfo[i].getWholesale(), secondaryInfo[i].getRetail());
				break;
			}

			else {
				strcpy_s(nameHolderPrevious, newBook[i].getTitle().c_str());
				continue;
			}
		}

		else {
			continue;
		}
	}
}

void addBook() {
	int openSpot = 0, counter = 0, iTemp = 0;
	string ptrString;
	double dTemp = 0;

	for (int i = 0; i < 20; i++) {
		if (i == 19 && secondaryInfo[i].isEmpty(i) != 0) {
			cout << "\nINVENTORY IS FULL; NO MORE BOOKS MAY BE ADDED TO THE INVENTORY" << endl;
			exit(0);
		}

		else if (secondaryInfo[i].isEmpty(i) == 1) {
			openSpot = i;
			break;
		}
	}

	cout << "\nPlease enter the title: ";
	getline(cin, ptrString);
	strUpper(&ptrString);
	newBook[openSpot].setTitle(&ptrString, openSpot);

	cout << "\nPlease enter the ISBN number: ";
	getline(cin, ptrString);
	strUpper(&ptrString);
	newBook[openSpot].setISBN(&ptrString, openSpot);

	cout << "\nPlease enter the Author's name: ";
	getline(cin, ptrString);
	strUpper(&ptrString);
	newBook[openSpot].setAuthor(&ptrString, openSpot);

	cout << "\nPlease enter the Publisher's name: ";
	getline(cin, ptrString);
	strUpper(&ptrString);
	newBook[openSpot].setPub(&ptrString, openSpot);

	cout << "\nPlease enter the date the book was added to the inventory (MMDDYYYY): ";
	getline(cin, ptrString);
	strUpper(&ptrString);
	secondaryInfo[openSpot].setDateAdded(&ptrString, openSpot);

	cout << "\nPlease enter the quantity of the book being added: ";
	cin >> iTemp;
	cin.ignore();
	secondaryInfo[openSpot].setQty(iTemp, openSpot);

	cout << "\nPlease enter the wholesale cost of the book: ";
	cin >> dTemp;
	cin.ignore();
	secondaryInfo[openSpot].setWholesale(dTemp, openSpot);

	cout << "\nPlease enter the retail price of the book: ";
	cin >> dTemp;
	cin.ignore();
	secondaryInfo[openSpot].setRetail(dTemp, openSpot);

	try {
		openFileP("InventoryDatabase.txt");

		for (int i = 0; i < 20; i++) {
			dataFile << i + 1 << "~" <<
				newBook[i].getISBN() << "," <<
				newBook[i].getTitle() << "," <<
				newBook[i].getAuthor() << "," <<
				newBook[i].getPub() << "," <<
				secondaryInfo[i].getDateAdded() << "," <<
				secondaryInfo[i].getQty() << "," <<
				secondaryInfo[i].getWholesale() << "," <<
				secondaryInfo[i].getRetail() << endl;
		}

		dataFile.close();
	}
	catch (string error) {
		cout << "\nERROR: FILE COULD NOT BE FOUND. EXITING NOW..." << endl;
		exit(1);
	}
}

void editBook() {
	string bookName, userString;
	int userChoice, userInt, i;
	double userDouble;
	char nameHolder[51] = {}, nameHolderPrevious[51] = {}, userInput;

	cout << "\nEnter the name of the book: ";
	getline(cin, bookName);
	strUpper(&bookName);

	// int i is the book's element number
	for (int i = 0; i < 20; i++) {
		if (i == 19 && newBook[i].bookMatch(bookName) == false) {
			cout << "\nTHE BOOK IS NOT IN THE INVENTORY. EXITING NOW...";
			exit(0);
		}

		else if (newBook[i].bookMatch(bookName) == true) {
			if (strcmp(newBook[i].getTitle().c_str(), nameHolderPrevious) == 0) {
				cout << "\nNO OTHER BOOKS WITH THAT NAME. EXITING NOW...";
				exit(0);
			}

			do {
				cout << "\nIs " << newBook[i].getTitle() << " the book you were looking for? (Y or N): ";

				cin.get(userInput);
				cin.ignore();
			} while (userInput != 'y' && userInput != 'Y' && userInput != 'n' && userInput != 'N');

			if (userInput == 'y' || userInput == 'Y') {
				//BOOKINFO TEMPS
				char iTemp[14], tTemp[51], aTemp[31], pTemp[31], dTemp[11];
				strcpy_s(iTemp, newBook[i].getISBN().c_str());
				strcpy_s(tTemp, newBook[i].getTitle().c_str());
				strcpy_s(aTemp, newBook[i].getAuthor().c_str());
				strcpy_s(pTemp, newBook[i].getPub().c_str());
				strcpy_s(dTemp, secondaryInfo[i].getDateAdded().c_str());

				bookInfo(iTemp, tTemp, aTemp, pTemp, dTemp,
					secondaryInfo[i].getQty(), secondaryInfo[i].getWholesale(), secondaryInfo[i].getRetail());

				do {
					cout << "\nWhich fields would you like to change? 1: ISBN |2: Title |3: Author |4: Publisher |5: Date Added |6: Quantity |7: Wholesale Price |8: Retail Price" << endl;
					cin >> userChoice;
					cin.ignore();
				}

				while (!(userChoice >= 1 && userChoice <= 8));

				switch (userChoice) {
				case 1:
					cout << "\nNew ISBN: ";
					getline(cin, userString);
					strUpper(&userString);
					newBook[i].setISBN(&userString, i);
					break;
				case 2:
					cout << "\nNew Book Title: ";
					getline(cin, userString);
					strUpper(&userString);
					newBook[i].setTitle(&userString, i);
					break;
				case 3:
					cout << "\nNew Author: ";
					getline(cin, userString);
					strUpper(&userString);
					newBook[i].setAuthor(&userString, i);
					break;
				case 4:
					cout << "\nNew publisher: ";
					getline(cin, userString);
					strUpper(&userString);
					newBook[i].setPub(&userString, i);
					break;
				case 5:
					cout << "\nNew Date: ";
					getline(cin, userString);
					strUpper(&userString);
					secondaryInfo[i].setDateAdded(&userString, i);
					break;
				case 6:
					cout << "\nNew Quantity: ";
					cin >> userInt;
					cin.ignore();
					secondaryInfo[i].setQty(userInt, i);
					break;
				case 7:
					cout << "\nNew Wholesale Price: ";
					cin >> userDouble;
					cin.ignore();
					secondaryInfo[i].setWholesale(userDouble, i);
					break;
				case 8:
					cout << "\nNew Retail Price: ";
					cin >> userDouble;
					cin.ignore();
					secondaryInfo[i].setRetail(userDouble, i);
					break;
				default:
					cout << "INVALID OPTION";
					exit(0);;
				}

				try {
					openFileP("InventoryDatabase.txt");

					for (int i = 0; i < 20; i++) {
						dataFile << i + 1 << "~" <<
							newBook[i].getISBN() << "," <<
							newBook[i].getTitle() << "," <<
							newBook[i].getAuthor() << "," <<
							newBook[i].getPub() << "," <<
							secondaryInfo[i].getDateAdded() << "," <<
							secondaryInfo[i].getQty() << "," <<
							secondaryInfo[i].getWholesale() << "," <<
							secondaryInfo[i].getRetail() << endl;
					}
					dataFile.close();
				}
				catch (string error) {
					cout << "\nERROR: FILE COULD NOT BE FOUND. EXITING NOW..." << endl;
					exit(1);
				}

				break;
			}

			else {
				strcpy_s(nameHolderPrevious, newBook[i].getTitle().c_str());
				continue;
			}
		}

		else {
			continue;
		}
	}
}

void deleteBook() {
	string bookName;
	int i, ARR_BOOK_LENGTH = 0, LENGTH = 0;
	char* strptr = nullptr;
	char userChoice, userInput, nameHolder[51] = {}, nameHolderPrevious[51] = {};

	cout << "\nEnter the name of the book: ";
	getline(cin, bookName);
	strUpper(&bookName);

	// int i is the book's position
	for (int i = 0; i < 20; i++) {
		if (i == 19 && newBook[i].bookMatch(bookName) == false) {
			cout << "\nTHE BOOK IS NOT IN THE INVENTORY. EXITING NOW...";
			exit(0);
		}

		else if (newBook[i].bookMatch(bookName) == true) {
			if (strcmp(newBook[i].getTitle().c_str(), nameHolderPrevious) == 0) {
				cout << "\nNO OTHER BOOKS WITH THAT NAME. EXITING NOW...";
				exit(0);
			}

			do {
				cout << "\nIs " << newBook[i].getTitle() << " the book you were looking for? (Y or N): ";
				cin.get(userInput);
				cin.ignore();
			} while (userInput != 'y' && userInput != 'Y' && userInput != 'n' && userInput != 'N');

			if (userInput == 'y' || userInput == 'Y') {
				//BOOKINFO TEMPS
				char iTemp[14], tTemp[51], aTemp[31], pTemp[31], dTemp[11];
				strcpy_s(iTemp, newBook[i].getISBN().c_str());
				strcpy_s(tTemp, newBook[i].getTitle().c_str());
				strcpy_s(aTemp, newBook[i].getAuthor().c_str());
				strcpy_s(pTemp, newBook[i].getPub().c_str());
				strcpy_s(dTemp, secondaryInfo[i].getDateAdded().c_str());

				bookInfo(iTemp, tTemp, aTemp, pTemp, dTemp,
					secondaryInfo[i].getQty(), secondaryInfo[i].getWholesale(), secondaryInfo[i].getRetail());

				do {
					cout << "\nAre you sure you wish to delete this book? (Y or N): ";
					cin.get(userChoice);
					cin.ignore();

					if (userChoice == 'y' || userChoice == 'Y') {
						secondaryInfo[i].removeBook(i);
					}

					else if (userChoice == 'n' || userChoice == 'N') {
						exit(0);
					}
				} while (userChoice != 'y' && userChoice != 'Y' && userChoice != 'n' && userChoice != 'N');
				break;
			}

			else {
				strcpy_s(nameHolderPrevious, newBook[i].getTitle().c_str());
				continue;
			}
		}

		else {
			continue;
		}
	}
}

//TEST FUNCTIONS
int dateCompare(string beingComparedTo, string comparing) {
	char firstDate[11] = {};
	char secDate[11] = {};
	//Individual Components
	char fMonth[2] = {};
	char sMonth[2] = {};
	char fDay[2] = {};
	char sDay[2] = {};
	char fYear[4] = {};
	char sYear[4] = {};
	//Individual 
	int fM, sM, fD, sD, fY, sY;

	strcpy_s(firstDate, beingComparedTo.c_str());
	strcpy_s(secDate, comparing.c_str());

	fMonth[0] = firstDate[0];
	fMonth[1] = firstDate[1];
	sMonth[0] = secDate[0];
	sMonth[1] = secDate[1];
	fDay[0] = firstDate[3];
	fDay[1] = firstDate[4];
	sDay[0] = secDate[3];
	sDay[1] = secDate[4];
	fYear[0] = firstDate[6];
	fYear[1] = firstDate[7];
	fYear[2] = firstDate[8];
	fYear[3] = firstDate[9];
	sYear[0] = secDate[6];
	sYear[1] = secDate[7];
	sYear[2] = secDate[8];
	sYear[3] = secDate[9];

	fM = atoi(fMonth);
	sM = atoi(sMonth);
	fD = atoi(fDay);
	sD = atoi(sDay);
	fY = atoi(fYear);
	sY = atoi(sYear);

	//CONDITION CHECKER AND RETURNER
	// IF FIRST DATE IS GREATER THAN SECOND DATE -> Return 1
	// IF SECOND DATE IS GREATER THAN FIRST DATE -> Return -1
	// IF DATES ARE EQUAL -> Return 0
	if (fY > sY) {
		return 1;
	}

	else if (fY < sY) {
		return -1;
	}

	else {
		if (fM > sM) {
			return 1;
		}

		else if (fM < sM) {
			return -1;
		}

		else {
			if (fD > sD) {
				return 1;
			}

			else if (fD < sD) {
				return -1;
			}

			else {
				return 0;
			}
		}
	}
	
}

//REPORT FUNCTIONS
void repListing() {
	bool isMore = true, isValid = false;
	int counter = 0;
	char userInput;

	cout << "\n\tReport: Total Listings" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);

	for (int i = 0; i < counter; i++) {
		//BOOKINFO TEMPS
		char iTemp[14], tTemp[51], aTemp[31], pTemp[31], dTemp[11];
		strcpy_s(iTemp, newBook[i].getISBN().c_str());
		strcpy_s(tTemp, newBook[i].getTitle().c_str());
		strcpy_s(aTemp, newBook[i].getAuthor().c_str());
		strcpy_s(pTemp, newBook[i].getPub().c_str());
		strcpy_s(dTemp, secondaryInfo[i].getDateAdded().c_str());

		bookInfo(iTemp, tTemp, aTemp, pTemp, dTemp,
			secondaryInfo[i].getQty(), secondaryInfo[i].getWholesale(), secondaryInfo[i].getRetail());

		cout << endl;
	}

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

void repWholesale() {
	bool isMore = true, isValid = false;
	int counter = 0;
	double sum = 0;
	char userInput;

	cout << "\n\tReport: Wholesale Prices" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);

	for (int i = 0; i < counter; i++) {
		cout << "\nTitle: " << newBook[i].getTitle() << endl;
		cout << "ISBN: " << newBook[i].getISBN() << endl;
		cout << "Quantity-On-Hand: " << secondaryInfo[i].getQty() << endl;
		cout << "Wholesale Cost: $" << secondaryInfo[i].getWholesale() << endl;
		sum += (secondaryInfo[i].getWholesale() * secondaryInfo[i].getQty());
	}
	cout << endl << "Total Wholesale Value of Inventory: $" << sum << endl;

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

void repRetail() {
	bool isMore = true, isValid = false;
	int counter = 0;
	double sum = 0;
	char userInput;

	cout << "\n\tReport: Retail Costs" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);

	for (int i = 0; i < counter; i++) {
		cout << "\nTitle: " << newBook[i].getTitle() << endl;
		cout << "ISBN: " << newBook[i].getISBN() << endl;
		cout << "Quantity-On-Hand: " << secondaryInfo[i].getQty() << endl;
		cout << "Retail Price: $" << secondaryInfo[i].getRetail() << endl;
		sum += (secondaryInfo[i].getRetail() * secondaryInfo[i].getQty());
	}
	cout << endl << "Total Retail Value of Inventory: $" << sum << endl;

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

void repQty() {
	bool isMore = true, isValid = false;
	int counter = 0, maxQty = secondaryInfo[0].getQty();
	int order[20];
	char userInput;

	cout << "\n\tReport: Total Quantity" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);

	for (int i = 0; i < counter; i++) {
		order[i] = secondaryInfo[i].getQty();
		
		if (order[i] > maxQty) {
			maxQty = order[i];
		}

		else {
			continue;
		}
	}

	for (int sorted = maxQty; sorted > 0; sorted--) {
		for (int i = 0; i < counter; i++) {
			if (secondaryInfo[i].getQty() == sorted) {
				cout << "\nTitle: " << newBook[i].getTitle() << endl;
				cout << "ISBN: " << newBook[i].getISBN() << endl;
				cout << "Quantity-On-Hand: " << secondaryInfo[i].getQty() << endl;
			}
		}
	}

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

void repCost() {
	bool isMore = true, isValid = false;
	int counter = 0;
	double order[20] = {}, previous = 0;
	char userInput;

	cout << "\n\tReport: Wholesale" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);
	
	for (int i = 0; i < counter; i++) {
		order[i] = secondaryInfo[i].getWholesale();
	}
	
	sort(order, order + 20);
	
	for (int i = 20; i > 0; i--) {
		
		if (order[i] == previous) {
			if (i == 19) {
				break;
			}
			continue;
		}

		for (int k = 0; k < counter; k++) {
			if (secondaryInfo[k].getWholesale() == order[i]) {
				cout << "\nTitle: " << newBook[k].getTitle() << endl;
				cout << "ISBN: " << newBook[k].getISBN() << endl;
				cout << "Quantity-On-Hand: " << secondaryInfo[k].getQty() << endl;
				cout << "Wholesale: " << secondaryInfo[k].getWholesale() << endl;
			}
		}
		previous = order[i];
	}

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

void repAge() {
	bool isMore = true, isValid = false;
	int counter = 0;
	string dates[20] = {}, sorted[20] = {}, tempHighest = "", previous = "";
	char userInput;

	cout << "\n\tReport: Date Added" << endl;

	do {
		for (int i = 0; i < 20; i++) {
			if (secondaryInfo[i].isEmpty(i) == 1) {
				counter = i;
				isMore = false;
				break;
			}

			else if (secondaryInfo[i].isEmpty(i) == 1 && i == 19) {
				isMore = false;
			}

			else {
				isMore = true;
			}
		}
	} while (isMore == true);

	//ADDS THE DATES TO "DATES[]"
	for (int i = 0; i < counter; i++) {
		dates[i] = secondaryInfo[i].getDateAdded();
	}

	//SORTS THE DATES INTO DESCENDING ORDER
	for (int i = 0; i < counter; i++) {
		for (int j = 0; j < counter - 1; j++) {
			if (dateCompare(dates[j], dates[i]) == -1) {
				tempHighest = dates[i];
				dates[i] = dates[j];
				dates[j] = tempHighest;
			}
		}
	}

	//PRINTER
	for (int i = 0; i < counter; i++) {
		if (dates[i] == previous) {
			if (i == 19) {
				break;
			}
			continue;
		}

		for (int k = 0; k < counter; k++) {
			if (secondaryInfo[k].getDateAdded() == dates[i]) {
				cout << "\nTitle: " << newBook[k].getTitle() << endl;
				cout << "ISBN: " << newBook[k].getISBN() << endl;
				cout << "Quantity-On-Hand: " << secondaryInfo[k].getQty() << endl;
				cout << "Date Added: " << secondaryInfo[k].getDateAdded() << endl;
			}
		}
		previous = dates[i];
	}

	while (isValid == false) {
		cout << "\nPress \"a\" to continue: ";
		cin >> userInput;
		cin.ignore();

		if (userInput == 'a' || userInput == 'A') {
			isValid = true;
		}

		else {
			isValid = false;
		}
	}

	cout << endl;
}

//FILE OPEN FUNCTIONS
void openFileR(string fileName) {
		dataFile.open(fileName, ios::in);

		if (dataFile.fail()) {
			string invalidFile = "INVALID FILE";
			throw invalidFile;
		}
}

void openFileP(string fileName) {
	dataFile.open(fileName, ios::out);

	if (dataFile.fail()) {
		string invalidFile = "INVALID FILE";
		throw invalidFile;
	}
}

//BOOKDATA FUNCTIONS
void BookData::setTitle(string* strptr, int arrayElement) {
	strcpy_s(newBook[arrayElement].bookTitle, strptr->c_str());
}

void BookData::setISBN(string* strptr, int arrayElement) {
	strcpy_s(newBook[arrayElement].isbn, strptr->c_str());
}

void BookData::setAuthor(string* strptr, int arrayElement) {
	strcpy_s(newBook[arrayElement].author, strptr->c_str());
}

void BookData::setPub(string* strptr, int arrayElement) {
	strcpy_s(newBook[arrayElement].publisher, strptr->c_str());
}

string BookData::getTitle() {
	return bookTitle;
}

string BookData::getISBN() {
	return isbn;
}

string BookData::getAuthor() {
	return author;
}

string BookData::getPub() {
	return publisher;
}

bool BookData::bookMatch(string str) {
	char* strptr = nullptr;
	char nameHolder[51] = {};

	strcpy_s(nameHolder, getTitle().c_str());
	strptr = strstr(nameHolder, str.c_str());

	if (strptr == 0) {
		return false;
	}

	else {
		return true;
	}
}

//InventoryBook Functions
void InventoryBook::setDateAdded(string* strptr, int arrayElement) {
	strcpy_s(secondaryInfo[arrayElement].dateAdded, strptr->c_str());
}

void InventoryBook::setQty(int quantity, int arrayElement) {
	secondaryInfo[arrayElement].qtyOnHand = quantity;
}

void InventoryBook::setWholesale(double amount, int arrayElement) {
	secondaryInfo[arrayElement].wholesale = amount;
}

void InventoryBook::setRetail(double amount, int arrayElement) {
	secondaryInfo[arrayElement].retail = amount;
}

int InventoryBook::isEmpty(int arrayElement) {
	if (newBook[arrayElement].getTitle() == "") {
		return 1;
	}

	else {
		return 0;
	}
}

void InventoryBook::removeBook(int arrayElement) {
	newBook[arrayElement].setISBN(NULL, arrayElement);
	newBook[arrayElement].setTitle(NULL, arrayElement);
	newBook[arrayElement].setTitle(NULL, arrayElement);
	newBook[arrayElement].setPub(NULL, arrayElement);
	secondaryInfo[arrayElement].dateAdded[0] = NULL;
	secondaryInfo[arrayElement].qtyOnHand = 0;
	secondaryInfo[arrayElement].wholesale = 0;
	secondaryInfo[arrayElement].retail = 0;

	try {
		openFileP("InventoryDatabase.txt");

		for (int i = 0; i < 20; i++) {
			dataFile << i + 1 << "~" <<
				newBook[i].getISBN() << "," <<
				newBook[i].getTitle() << "," <<
				newBook[i].getAuthor() << "," <<
				newBook[i].getPub() << "," <<
				secondaryInfo[i].dateAdded << "," <<
				secondaryInfo[i].qtyOnHand << "," <<
				secondaryInfo[i].wholesale << "," <<
				secondaryInfo[i].retail << endl;
		}
		dataFile.close();
	}
	catch (string error) {
		cout << "\nERROR: FILE COULD NOT BE FOUND. EXITING NOW..." << endl;
		exit(1);
	}
}

string InventoryBook::getDateAdded() {
	return dateAdded;
}

int InventoryBook::getQty() {
	return qtyOnHand;
}

double InventoryBook::getWholesale() {
	return wholesale;
}

double InventoryBook::getRetail() {
	return retail;
}

//SoldBook Functions
void SoldBook::setTaxRate(double tr) {
	taxRate = tr;
}

void SoldBook::setQtySold(int qty) {
	qtySold = qty;
}

void SoldBook::setTax(double t) {
	tax = t;
}

void SoldBook::setSubtotal(double sub) {
	subtotal = sub;
}

void SoldBook::setTotal(double tot) {
	total = tot;
}

void SoldBook::setSoldTitle(string sTitle) {
	title = sTitle;
}

void SoldBook::setSoldISBN(string sISBN) {
	cashierISBN = sISBN;
}

void SoldBook::setUnitPrices(double uPrice) {
	unitPrices = uPrice;
}

double SoldBook::getTaxRate() {
	return taxRate;
}

int SoldBook::getQtySold() {
	return qtySold;
}

double SoldBook::getTax() {
	return tax;
}

double SoldBook::getSubtotal() {
	return subtotal;
}

double SoldBook::getTotal() {
	return total;
}

string SoldBook::getSoldTitle() {
	return title;
}

string SoldBook::getSoldISBN() {
	return cashierISBN;
}

double SoldBook::getUnitPrices() {
	return unitPrices;
}