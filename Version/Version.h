#pragma once
/////////////////////////////////////////////////////////////////////////////////////
// Version.h - version funtion for Repository										   //
// Author: Beier Chen, bchen22@syr.edu												 	   //
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018   //   
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* manages version numbering for all files held in the Repository

* Required Files:
* ---------------
* NoSqlDb.h, PayLoad.h
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
#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"

using namespace NoSqlDb;
using namespace std;

namespace RepositoryCore
{
	template<typename PayLoad>
	class Version
	{
	public:

		Version() = default;
		Version(DbCore<PayLoad>& db) : db_(db) {}

		DbCore<PayLoad>& database() { return *pDb_; }

		int inquireVersion(const string& key);

		string& logInName() { return logInName_; }
		string logInName() const { return logInName_; }
		void logIn(const string& logInName) { logInName_ = logInName; }

	private:
		DbCore<PayLoad>& db_;
		string logInName_;
	};

	// -----< inquireVersion() - return the version number of a file >---------------
	template<typename PayLoad>
	int Version<PayLoad>::inquireVersion(const string& fileName)
	{
		int versionNum = 0;
		int latestVerNum = 0;

		Query<PayLoad> query(db_);
		Conditions<PayLoad> condition;
		condition.name(fileName);
		query.select(condition); 		// query the records with fileName
		
		// if a record with fileName exists in database
		if (query.keys().size() != 0)
		{
			for (auto key : query.keys())
			{
				// if a record is in open for check-in, return its version number
				if (db_.dbStore()[key].payLoad().isOpen()) 
				{
					versionNum = db_.dbStore()[key].payLoad().version();
					return versionNum;
				}
				// if all the records of this file are closed, return the latest version number
				else
				{
					if (db_.dbStore()[key].payLoad().version() > latestVerNum) 
					{
						latestVerNum = db_.dbStore()[key].payLoad().version();
					}
				}				
			}
			return ++latestVerNum;
		}
		// if a record with fileName dosen't exists in database, version number is 1
		else
		{
			versionNum = 1;
			return versionNum;
		}
	}
}