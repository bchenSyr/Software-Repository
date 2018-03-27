////////////////////////////////////////////////////////////////////////////////////
// CheckIn.cpp - Test CheckIn function
// Author: Beier Chen, bchen22@syr.edu
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018       
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "CheckIn.h"
#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"
#include "../FileManager/FileManager.h"
#include "../Version/Version.h"
#include "../GraphWalkDemo/GraphWalkDemo/Graph.h"

using namespace NoSqlDb;

#ifdef TEST_CHECKIN
int main()
{
	Utilities::Title("TestApplication - demonstrates CheckIn function ");

	DbCore<PayLoad> db;
	DbElement<PayLoad> elem;
	CheckIn<PayLoad> checkIn(db);
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

	return 0;
}

#endif