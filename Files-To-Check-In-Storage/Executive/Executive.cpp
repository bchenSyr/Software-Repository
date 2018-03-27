/////////////////////////////////////////////////////////////////////
// Executive.cpp - Help to demonstrate Project1 
// Author: Beier Chen, bchen22@syr.edu                                                         
// Application: Project1 - CSE687 - Object Oriented Design, Spring 2018       
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include"../XmlDocument/XmlElement/XmlElement.h"
#include"../XmlDocument/XmlDocument/XmlDocument.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>
#include <unordered_map>
#include <fstream>

using namespace std;
using namespace NoSqlDb;
using namespace Utilities;
using namespace XmlProcessing;

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
	DbCore<string>& db() { return db_; }
	void RestoreDbFromXml(string Xml);
	void MakeTestDatabase();

private:
	static DbCore<string> db_;
};

DbCore<std::string> DbProvider::db_;

//----< helper function to demonstrate requirement#8 >---------------------------
void DbProvider::RestoreDbFromXml(string Xml)
{
	DbCore<string> database;
	cout << "\n  -  Create XmlDocument from XML string";
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	XmlDocument newXDoc(Xml, XmlDocument::str);

	cout << "\n  -  Restore databse from XmlDocument";
	using Key = std::string;
	std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
	for (auto pRecord : records)
	{
		Key key;
		DbElement<string> elem;
		std::vector<Sptr> pChildren = pRecord->children();
		for (auto pChild : pChildren)
		{
			if (pChild->tag() == "key") // record's children are key and value
			{
				key = pChild->children()[0]->value(); // get value of TextElement child
			}
			else
			{
				// value represents a DbElement with name, dateTime and description
				std::vector<Sptr> pValueChildren = pChild->children();

				for (auto pValueChild : pValueChildren)
				{
					if (pValueChild->tag() == "name")
					{
						elem.name(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "dateTime")
					{
						elem.dateTime(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "description")
					{
						elem.descrip(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "child")
					{
						elem.children().push_back(pValueChild->children()[0]->value());
					}
				}
			}
		}
		database[key] = elem;
	}
	showDbForReq8(database);
}

//----< make a database to help demonstrationrequirement#8>---------------------------
void DbProvider::MakeTestDatabase()
{
	DbCore<string> database;
	DbProvider databaseProvider;
	DbElement<std::string> dbElem;

	dbElem.name("DateTime");
	dbElem.descrip("package");
	dbElem.dateTime(DateTime().now());
	database["DateTime"] = dbElem;

	dbElem.name("DbCore");
	database["DbCore"] = dbElem;

	dbElem.name("Executive");
	database["Executive"] = dbElem;

	dbElem.name("Query");
	database["Query"] = dbElem;

	dbElem.name("TestUtilities");
	database["TestUtilities"] = dbElem;

	dbElem.name("XmlDocument");
	database["XmlDocument"] = dbElem;

	database["Executive"].children().push_back("DbCore");
	database["Executive"].children().push_back("DateTime");
	database["Executive"].children().push_back("Query");
	database["Executive"].children().push_back("XmlDocument");
	database["Executive"].children().push_back("TestUtilities");

	database["DbCore"].children().push_back("DateTime");
	database["DbCore"].children().push_back("Query");
	database["Query"].children().push_back("DateTime");
	database["Query"].children().push_back("DbCore");

	databaseProvider.db() = database;
}

///////////////////////////////////////////////////////////////////////
// test functions
//----< demo requirement #1 >------------------------------------------
bool DemonReq1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions."
		<< "\n  Examine code in Visual Studio to verify that it is C++, using standard libraries.";
	putLine(2);
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------
bool DemonReq2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine(2);
	return true;
}

//----< demo requirement #3 >----------------------------
bool DemonReq3()
{
	Utilities::title("Demonstrating Requirement #3: "
		"Each value consists of Metadata and an instance of the generic type");

	DbCore<std::string> database; // create database to hold std::string payload
	DbProvider databaseProvider;
	databaseProvider.db() = database;

	// create a demo elements and insert into db
	DbElement<std::string> dbElem;
	dbElem.name("Albus");
	dbElem.descrip("Headmaster of Hogwarts");
	dbElem.payLoad("Lumos");
	DateTime dt1("1 Dec 1 12 0 0 2017");
	dbElem.dateTime(dt1);

	// add value part with key "Dumbledore"
	database["Dumbledore"] = dbElem;
	databaseProvider.db() = database;

	// display result
	std::cout << "\n  - Creating a datebase element with key \"Dumbledore\":";
	showDb(database);

	putLine(2);
	return true;
}

//----< demo requirement #4>---------------------------
bool DemonReq4()
{
	Utilities::title("Demonstrating Requirement #4 - support addition and deletion of key/value pairs.");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	DbElement<string> dbElem;

	// -----< demonstrate addition of key/value pairs.
	std::cout << "\n  - add five elements with key "
		"\"Riddle\", \"Potter\", \"Weasley\", \"Granger\", \"Snape\": ";

	dbElem.name("Tom");
	dbElem.descrip("Student of Slytherin house");
	dbElem.payLoad("Accio");
	DateTime dt2("1 Jan 20 12 0 0 2018");
	dbElem.dateTime(dt2);
	database["Riddle"] = dbElem;

	dbElem.name("Harry");
	dbElem.descrip("The boy who lived");
	dbElem.payLoad("Expecto Patronum");
	DateTime dt3("1 Feb 1 12 0 0 2018");
	dbElem.dateTime(dt3);
	database["Potter"] = dbElem;

	dbElem.name("Ron");
	dbElem.descrip("Harry's best friend");
	dbElem.payLoad("Expelliarmus");
	dbElem.dateTime(DateTime().now());
	database["Weasley"] = dbElem;

	dbElem.name("Hermione");
	dbElem.descrip("Student of Gryffindor house");
	dbElem.payLoad("Wingardium Leviosa");
	database["Granger"] = dbElem;

	dbElem.name("Severus");
	dbElem.descrip("Head of Slytherin house");
	dbElem.payLoad("Sectumsempra");
	database["Snape"] = dbElem;

	database["Snape"].children().push_back("Potter");
	database["Snape"].children().push_back("Weasley");
	showDb(database);

	// -----< demonstrate deletion of key/value pairs.
	std::cout << "\n\n  - delete pairs with key \"Snape\": "; //
	database.deletePair("Snape");
	showDb(database);

	databaseProvider.db() = database;
	putLine(2);
	return true;
}

//----< demo requirement #5>---------------------------
bool DemonReq5() {
	Utilities::title("Demonstrating Requirement #5 - "
		"support editing of values including child relationship, metadata text and payload");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	DbElement<string> dbElem;

	// -----< demonstrate addition of child relationships
	std::cout << "\n  - add \"Potter\", \"Weasley\", \"Granger\", \"Riddle\" to children collection of \"Dumbledore\":";
	database["Dumbledore"].children().push_back("Potter");
	database["Dumbledore"].children().push_back("Weasley");
	database["Dumbledore"].children().push_back("Granger");
	database["Dumbledore"].children().push_back("Riddle");
	showDb(database);

	std::cout << "\n\n  - add \"Weasley\", \"Granger\" to children collection of \"Potter\":";
	database["Potter"].children().push_back("Weasley");
	database["Potter"].children().push_back("Granger");
	showDb(database);

	// -----< demonstrate deletion of child relationships
	std::cout << "\n\n  - after deleting \"Riddle\" from the children collection of element with Key \"Dumbledore\":";
	database["Dumbledore"].deleteChild("Riddle");
	showHeader();
	showElemWithKey(database, "Dumbledore");

	// -----< demonstrate edition of the matadata
	std::cout << "\n  - after changing the name of elemement with key: \"Riddle\":";
	database["Riddle"].name("Voldemort");
	showHeader();
	showElemWithKey(database, "Riddle");

	std::cout << "\n  - after changing the description of elemement with key: \"Riddle\":";
	database["Riddle"].descrip("You-Know-Who");
	showHeader();
	showElemWithKey(database, "Riddle");

	std::cout << "\n  - after changing the payload of elemement with key: \"Riddle\":";
	database["Riddle"].payLoad("Avada Kedavra");
	showHeader();
	showElemWithKey(database, "Riddle");

	std::cout << "\n\n  - after all the changes, show the database:";
	showDb(database);

	databaseProvider.db() = database;
	putLine(2);
	return true;
}

//----< demo requirement #6>---------------------------
bool DemonReq6() {
	Utilities::title("Demonstrating Requirement #6 - support basic queries");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	Query<string> demoQuery(database);

	cout << "\n\n  - Query the value of elements with key \"Potter\":";
	showHeader();
	cout << "\n    " << std::setw(15) << std::left << "Potter";
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

	DateTime begin_time("5 Jan 10 12 0 0 2018");
	DateTime end_time("5 Feb 3 12 0 0 2018");

	cout << "\n  - Query keys that contain values written within between";
	demoQuery.keys(database.keys());
	demoQuery.getKeyMatchTime(begin_time, end_time).show();

	cout << "\n  - Only provide one end of the interval. Query keys that contain values written within between";
	demoQuery.keys(database.keys());
	demoQuery.getKeyMatchTime(begin_time).show();
	putLine(2);
	return true;
}

//----< demo requirement #7>---------------------------
bool DemonReq7()
{
	Utilities::title("Demonstrating Requirement #7 - support compound queries");
	DbProvider databaseProvider;
	DbCore<string> database = databaseProvider.db();
	DbElement<string> condition;
	Query<string> demoQuery1(database), demoQuery2(database),
		demoQuery3(database), demoQuery4(database), demoQuery5(database);
	
	showDb(database);
	cout << "\n  - demoQuery1"
		"\n    Query the keys with metadata name matching pattern \".*r.*\" "
		"and description matching pattern \".*in.*\" :";
	condition.name(".*r.*");
	condition.descrip(".*in.*");
	demoQuery1.select(condition).show();

	cout << "\n  - demoQuery2"
		"\n    Query the keys with metadata name matching pattern \".*(o|i|a).*\" "
		"and description matching pattern \".*e.*\" :";
	condition.name(".*(o|i|a).*");
	condition.descrip(".*e.*");
	demoQuery2.select(condition).show();

	cout << "\n  - demoQuery3"
		"\n    Query the keys with metadata name matching pattern \".*(o|i).*\" "
		"and description matching pattern \".*i.*\", "
		"\n    on the set of keys returned by demoQuery2:";
	condition.name(".*(o|i).*");
	condition.descrip(".*i.*");
	demoQuery3.from(demoQuery2.returnKeys()).select(condition).show();

	cout << "\n  - demoQuery4"
		"\n    Query the keys with metadata name \"Hermione\", "
		"\n    on the \"and/&&\" union of results of demoQuery1 and demoQuery2:";
	condition.name("Hermione");
	demoQuery4.from(demoQuery1 && demoQuery2).selectName(condition).show();

	cout << "\n  - demoQuery5"
		"\n    Query the keys with metadata name \"Ron\" "
		"\n    on the \"or/||\" union of results of demoQuery3 and demoQuery4:";
	condition.name("Ron");
	demoQuery5.from(demoQuery3 || demoQuery4).selectName(condition).show();
	putLine(2);
	return true;
}

//----< demo requirement #8>---------------------------
bool DemonReq8a() 
{
	Utilities::title("Demonstrating Requirement #8a"
		" - Persist a collection of database contents to an XML file.");
	DbProvider databaseProvider;
	cout << "\n  -  Construct a database to illustrate project's package structure and dependency relationships:";
	databaseProvider.MakeTestDatabase();
	DbCore<string> database = databaseProvider.db();
	showDbForReq8(database);

	cout << "\n  -  Persist the database into a XML string ";
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Sptr pDb = makeTaggedElement("db");
	pDb->addAttrib("type", "testDb");
	Sptr pDocElem = makeDocElement(pDb);

	XmlDocument xDoc(pDocElem);
	for (auto item : database.dbStore())
	{
		Sptr pRecord = makeTaggedElement("dbRecord");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);
		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDateTime = makeTaggedElement("dateTime", item.second.dateTime());
		pValue->addChild(pDateTime);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);

		for (auto kid : item.second.children()) 
		{
			Sptr pChild = makeTaggedElement("child", kid);
			pValue->addChild(pChild);
		}
	}
	std::string Xml = xDoc.toString();
	std::cout << Xml << std::endl;

	cout << "\n  -  Save XML string to file \"packageFile.xml\" to project folder";
	ofstream out("..\\package_structure.xml"); 
	out << Xml;  
	out.close(); 
	putLine(2);
	return true;
}

//----< demo requirement #8>---------------------------
bool DemonReq8b() 
{
	Utilities::title("Demonstrating Requirement #8b - Restore  database from an existing XML file.");
	DbProvider databaseProvider;
	string Xml;
	ifstream in("..\\package_structure.xml");

	cout << "\n  -  Read the XML string from file \"package_structure.xml\" in project folder ";
	while (in.good())
	{
		char ch = in.get();
		if (!in.good())
			break;
		Xml += ch;
	}
	in.close();
	cout << "\n" << Xml;

	databaseProvider.RestoreDbFromXml(Xml);
	putLine(2);
	return true;
}

//----< demo requirement #9>---------------------------
bool DemonReq9()
{
	Utilities::title("Demonstrating Requirement #9 - "
		"The Payload type contains a vector<string>");

	DbCore<vector<string>> database;
	DbElement<vector<string>> dbElem;

	dbElem.name("Voldemort");
	dbElem.descrip("He-Who-Must-Not-Be-Named");
	dbElem.dateTime(DateTime().now());
	vector<string> vec;
	vec.push_back("Avada Kedavra,");
	vec.push_back("Crucio,");
	vec.push_back("Imperio");

	dbElem.payLoad(vec);
	database["Lord"] = dbElem;

	dbElem.name("Harry");
	dbElem.descrip("The Boy Who Lived");
	dbElem.dateTime(DateTime().now());
	vector<string> vec1;
	vec1.push_back("Expelliarmus,");
	vec1.push_back("Disapparation,");
	vec1.push_back("Accio");

	dbElem.payLoad(vec1);
	database["Potter"] = dbElem;
	showDb(database);
	putLine(2);
	return true;
}

//----< test stub >----------------------------------------------------
int main()
{
	Utilities::Title("Testing DbCore ");
	TestExecutive ex;

	// define test structures with test function and message
	TestExecutive::TestStr ts1{ DemonReq1, "Use C++11" };
	TestExecutive::TestStr ts2{ DemonReq2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3{ DemonReq3, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts4{ DemonReq4, "Addition and deletion of key/value pairs" };
	TestExecutive::TestStr ts5{ DemonReq5, "Edition of value part" };

	TestExecutive::TestStr ts6{ DemonReq6, "Simple query" };
	TestExecutive::TestStr ts7{ DemonReq7, "Compound query" };
	TestExecutive::TestStr ts8a{ DemonReq8a, "Persist database contents to a XML file" };
	TestExecutive::TestStr ts8b{ DemonReq8b, "Restore a XML file to database" };
	TestExecutive::TestStr ts9{ DemonReq9, "Payload type contain std::vector<std::string>" };

	// register test structures with TestExecutive instance, ex
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8a);
	ex.registerTest(ts8b);
	ex.registerTest(ts9);

	// run tests
	bool result = ex.doTests();
	if (result == true)
	  std::cout << "\n  all tests passed";
	else
	  std::cout << "\n  at least one test failed";

	getchar();
	putLine(2);
	return 0;
}
