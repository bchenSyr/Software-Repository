////////////////////////////////////////////////////////////////////////////////////
// Version.cpp - Test Version function
// Author: Beier Chen, bchen22@syr.edu
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018       
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Version.h"
#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"
#include "../FileManager/FileManager.h"
#include "../Version/Version.h"
#include "../GraphWalkDemo/GraphWalkDemo/Graph.h"

using namespace NoSqlDb;

#ifdef TEST_VERSION
int main()
{
	Utilities::Title("TestApplication - demonstrates Version function ");

	DbCore<PayLoad> db;
	DbElement<PayLoad> elem;
	CheckIn<PayLoad> checkIn;
	CheckOut<PayLoad> checkOut;
	PayLoad pl;
	string filePath;

	// check in 
	filePath = "../Files-To-Check-In-Storage/XmlDocument/XmlElement.h";
	cout << "\n - Checking-in file: " << filePath;
	pl.categories().clear();
	elem.children().clear();
	pl.categories().push_back("XmlElement");
	elem.descrip("XmlDocument Package");
	elem.payLoad(pl);
	checkIn.checkIn(filePath, elem);

	showDb(repo_.db());
	elem.payLoad().showDb(repo_.db());

	checkOut.checkout("XmlElement.h");
	showDb(repo_.db());
	elem.payLoad().showDb(repo_.db());

	Utilities::title("Domonstrating checking in a file with closed check-in state, version number increase");
	checkIn.close(filePath);
	showDb(repo_.db());
	elem.payLoad().showDb(repo_.db());

	return 0;
}

#endif