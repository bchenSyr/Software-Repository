/////////////////////////////////////////////////////////////////////
// Query.cpp - test Query function              
// Author: Beier Chen, bchen22@syr.edu                                                         
// Application: Project1 - CSE687 - Object Oriented Design, Spring 2018       
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "Query.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;
using namespace std;

//----< reduce the number of characters to type >----------------------
auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.
class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }

private:
	static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions
//----< make a database to help demonstration>---------------------------
void MakeTestDatabase()
{
	DbCore<string> database;
	DbProvider databaseProvider;

	DbElement<std::string> dbElem;
	dbElem.name("Albus");
	dbElem.descrip("Headmaster of Hogwarts");
	DateTime dt1("1 Dec 1 12 0 0 2017");
	dbElem.dateTime(dt1);
	dbElem.payLoad("Lumos");
	database["Dumbledore"] = dbElem;

	dbElem.name("Tom");
	dbElem.descrip("Student of Slytherin house");
	DateTime dt2("1 Jan 20 12 0 0 2018");
	dbElem.dateTime(dt2);
	dbElem.payLoad("Accio");
	database["Riddle"] = dbElem;

	dbElem.name("Harry");
	DateTime dt3("1 Feb 1 12 0 0 2018");
	dbElem.dateTime(dt3);
	dbElem.descrip("Student of Gryffindor house");
	dbElem.payLoad("Expecto Patronum");
	database["Potter"] = dbElem;

	dbElem.name("Ron");
	dbElem.dateTime(DateTime().now());
	dbElem.payLoad("Expelliarmus");
	database["Weasley"] = dbElem;

	dbElem.name("Luna");
	dbElem.descrip("Student of Ravenclaw house");
	dbElem.payLoad("Wingardium Leviosa");
	database["Lovegood"] = dbElem;

	dbElem.name("Severus");
	dbElem.descrip("Head of Slytherin house");
	dbElem.payLoad("Sectumsempra");
	database["Snape"] = dbElem;

	database["Dumbledore"].children().push_back("Potter");
	database["Dumbledore"].children().push_back("Weasley");
	database["Dumbledore"].children().push_back("Lovegood");
	database["Dumbledore"].children().push_back("Riddle");

	databaseProvider.db() = database;
}

//----< demo second part of requirement #6>---------------------------
void TestSimpleQuery() {
	Utilities::title("Demonstrating simple query function");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	showDb(database);
	Query<string> demoQuery(database);
	demoQuery.keys(database.keys());

	cout << "\n\n  - Query the value of elements with key \"Potter\":";
	demoQuery.getValue("Potter");

	cout << "\n  - Query a non-existing key \"Utopia\":";
	demoQuery.getValue("Utopia");

	cout << "\n  - Query the children of element with key \"Dumbledore\":";
	demoQuery.getChildren("Dumbledore");

	cout << "\n  - Query the set of keys matching "
		"a specified regular-expression pattern \".*o.*\":";
	demoQuery.getKeyMatchPattern(".*o.*").show();

	cout << "\n  - Query the set of keys matching "
		"a specified regular-expression pattern. \".*le.*\":";
	demoQuery.keys(database.keys());
	demoQuery.getKeyMatchPattern(".*le.*").show();

	cout << "\n  - Query the keys with metadata name matching pattern \".*(o|i).*\" "
		"and description matching pattern \".*en.*\" :";

	DbElement<string> condition;
	condition.name(".*(o|i).*");
	condition.descrip(".*en.*");
	demoQuery.keys(database.keys());
	demoQuery.select(condition).show();

	cout << "\n  - Query the keys with metadata name matching pattern \".*ab.*\" "
		"and description matching pattern \".*cd.*\" :";
	condition.name(".*n.*");
	condition.descrip(".*cd.*");
	demoQuery.keys(database.keys());
	demoQuery.select(condition).show();

	DateTime begin_time("1 Jan 10 12 0 0 2018");
	DateTime end_time("1 Feb 3 12 0 0 2018");

	cout << "\n  - Query keys that contain values written within between";
	demoQuery.keys(database.keys());
	demoQuery.getKeyMatchTime(begin_time, end_time).show();

	cout << "\n  - Only provide one end of the interval. Query keys that contain values written within between";
	demoQuery.keys(database.keys());
	demoQuery.getKeyMatchTime(begin_time).show();
}

void TestCompoundQuery()
{
	Utilities::title("Demonstrating compound query function");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	showDb(database);
	Query<string> demoQuery1(database);
	Query<string> demoQuery2(database);
	Query<string> demoQuery3(database);
	Query<string> demoQuery4(database);
	Query<string> demoQuery5(database);
	DbElement<string> condition;

	cout << "\n  - demoQuery1";
	condition.name(".*r.*");
	condition.descrip(".*in.*");
	demoQuery1.select(condition).show();

	cout << "\n  - demoQuery2";
	condition.name(".*(o|i|a).*");
	condition.descrip(".*e.*");
	demoQuery2.select(condition).show();

	cout << "\n  - demoQuery3";
	condition.name(".*(o|i).*");
	condition.descrip(".*i.*");
	demoQuery3.from(demoQuery2.returnKeys()).select(condition).show();

	cout << "\n  - demoQuery4";
	condition.name("Harry");
	demoQuery4.from(demoQuery1 && demoQuery2).selectName(condition).show();

	cout << "\n  - demoQuery5";
	condition.name("Tom");
	demoQuery5.from(demoQuery3 || demoQuery4).selectName(condition).show();
}

//----< test stub >----------------------------------------------------
#ifdef TEST_QUERY

using namespace Utilities;

int main()
{
	Utilities::Title("Testing Query function ");
	putLine();

	MakeTestDatabase();
	TestSimpleQuery();
	TestCompoundQuery();

	std::cin.get();
	putLine(2);
	return 0;
}
#endif
