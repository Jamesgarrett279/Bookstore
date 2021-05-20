#pragma once
#include <string>
using namespace std;

class BookData {
private:
	char isbn[14];
	char bookTitle[51];
	char author[31];
	char publisher[31];
public:
	void setTitle(string*, int);
	void setISBN(string*, int);
	void setAuthor(string*, int);
	void setPub(string*, int);

	string getTitle();
	string getISBN();
	string getAuthor();
	string getPub();

	bool bookMatch(string);
};

class InventoryBook : BookData {
private:
	char dateAdded[11];
	int qtyOnHand;
	double wholesale;
	double retail;
public:
	void setDateAdded(string*, int);
	void setQty(int, int);
	void setWholesale(double, int);
	void setRetail(double, int);
	int isEmpty(int);
	void removeBook(int);

	string getDateAdded();
	int getQty();
	double getWholesale();
	double getRetail();
};

class SoldBook : InventoryBook {
private:
	static double taxRate;
	int qtySold;
	double tax;
	double subtotal;
	static double total;
	
	string cashierISBN;
	string title;
	double unitPrices = 0;
public:


	void setTaxRate(double);
	void setQtySold(int);
	void setTax(double);
	void setSubtotal(double);
	void setTotal(double);

	void setSoldTitle(string);
	void setSoldISBN(string);
	void setUnitPrices(double);

	double getTaxRate();
	int getQtySold();
	double getTax();
	double getSubtotal();
	double getTotal();

	string getSoldTitle();
	string getSoldISBN();
	double getUnitPrices();
};

int cashier();
int invMenu();
int bookInfo(char[14], char [51], char [31], char[31], char [11], int, double, double); 
int reports();

void strUpper(string*);

void openFileR(string);
void openFileP(string);

void lookUpBook();
void addBook();
void editBook();
void deleteBook();

int dateCompare(string, string);

void repListing();
void repWholesale();
void repRetail();
void repQty();
void repCost();
void repAge();
