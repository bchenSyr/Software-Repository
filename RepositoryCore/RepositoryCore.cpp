////////////////////////////////////////////////////////////////////////////////////
// RepositoryCore.cpp - Test RepositoryCore function
// Author: Beier Chen, bchen22@syr.edu
// Source: Jim Fawcett
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018       
////////////////////////////////////////////////////////////////////////////////////

#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"
#include "../RepositoryCore/RepositoryCore.h"

using namespace NoSqlDb;

#ifdef TEST_CORE
int main()
{
	Utilities::Title("TestApplication - demonstrates RepositoryCore function ");
	Utilities::putline();

	RepositoryCore::Repository<NoSqlDb::PayLoad> repo;
	DbElement<PayLoad> elem;
	PayLoad pl;
	string filePath;
	repo_.checkIn().logIn("Beier C.");
	cout << "\n - Currently loging as: " << repo_.checkIn().logInName() << endl;

	Utilities::title("Domonstrating check-in by accepting a files and append a version number to the end of file name.");
	filePath = "../Files-To-Check-In-Storage/DateTime/DateTime.h";
	cout << "\n - Checking-in file: " << filePath;
	pl.categories().push_back("DateTime");
	elem.descrip("DateTime Package");
	elem.payLoad(pl);
	repo_.checkIn().checkIn(filePath, elem); // 1st check in
	showDb(repo_.db());
	elem.payLoad().showDb(repo_.db());

	return 0;
}

#endif