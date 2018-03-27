#pragma once
/////////////////////////////////////////////////////////////////////////////////////
// Browse.h - browse funtion for Repository											   //
// Author: Beier Chen, bchen22@syr.edu												 	   //
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018   //   
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* provides the capability to locate files and view their contents
* support browsing of one or more packages by displaying package descriptions
* the full package text shall be displayed on demand
*
* Required Files:
* ---------------
* NoSqlDb.h, PayLoad.h, FileManager.h, Version.h
*
* Build Process:
* --------------
* devenv Project2-Repository.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*
*/

#include <iostream>
#include "../RepositoryCore/RepositoryCore.h"
#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"
#include "../FileManager/FileManager.h"
#include "../Version/Version.h"

using namespace NoSqlDb;
using namespace std;

namespace RepositoryCore
{
	template<typename PayLoad>
	class Browse
	{
	public:
		Browse() = default;
		Browse(DbCore<PayLoad>& db) : db_(db), version_(db), query_(db){}
		Browse<PayLoad> browse(const string &fileName);
		void showContent();
	private:
		DbCore<PayLoad>& db_;
		Version<PayLoad> version_;
		Query<PayLoad> query_;
	};

	// -----< browse() - query file and display its description>---------------
	template<typename PayLoad>
	Browse<PayLoad> Browse<PayLoad>::browse(const string &fileName)
	{
		Conditions<PayLoad> condition;
		condition.name(fileName);
		query_.select(condition); 		// query the records with fileName
										
		cout << "\n - query file " << fileName;
		if (query_.keys().size() != 0)
		{
			for (auto key : query_.keys())
			{
				cout << "\n   destription of " << fileName << ": \"" << db_.dbStore()[key].descrip() <<"\""
					<< " version: " << db_.dbStore()[key].payLoad().version();
			}
		}
		else
		{
			cout << "\n - no result matching query " << fileName;
		}
		return *this;
	}

	// -----< showContent() - display the content of file >---------------
	void Browse<PayLoad>::showContent()
	{
		for (auto key : query_.keys())
		{
			cout << "\n - Display the content of file \"" << key << "\": ";
			string display;
			string filePath = db_.dbStore()[key].payLoad().value();
			ifstream in(filePath);

			while (in.good())
			{
				char ch = in.get();
				if (!in.good())
					break;
				display += ch;
			}
			in.close();
			cout << "\n" << display;
		}
	}
}