#pragma once
/////////////////////////////////////////////////////////////////////////////////////
// CheckOut.h - check-out funtion for Repository										   //
// Author: Beier Chen, bchen22@syr.edu												 	   //
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018   //   
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* retrieves package files, removing version information from their filenames.
* retrieved files will be copied to a specified directory
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
#include <map>
#include "../NoSqlDb/Executive/NoSqlDb.h"
#include "../NoSqlDb/PayLoad/PayLoad.h"
#include "../FileManager/FileManager.h"
#include "../Version/Version.h"

using namespace NoSqlDb;
using namespace std;

namespace RepositoryCore
{
	template<typename PayLoad>
	class CheckOut
	{
	public:
		CheckOut() = default;
		CheckOut(DbCore<PayLoad>& db) : db_(db), version_(db) {}
		void checkout(const string &fileName);

		void findDependencyFiles(const string &key);

	private:
		DbCore<PayLoad>& db_;
		Version<PayLoad> version_;
		string dstFolder_ = "../Check-Out-Storage/";
		map<string, int>childFilesMap_;
	};

	// -----< checkout() - check out a file >---------------
	template<typename PayLoad>
	void CheckOut<PayLoad>::checkout(const string &fileName)
	{
		cout << "\n\n - check out file " << fileName;
		int versionNum = version_.inquireVersion(fileName);
		string srcFileName = fileName + "." + to_string(versionNum - 1);

		string dstFilePath = dstFolder_ + fileName;
		string srcFilePath = db_.dbStore()[srcFileName].payLoad().value();

		File::copyFile(srcFilePath, dstFilePath);

		childFilesMap_.clear();
		findDependencyFiles(srcFileName);

		cout << "\n\n - check out all dependant files for " << srcFileName;
		for (auto item : childFilesMap_)
		{
			srcFileName = item.first;
			srcFilePath = db_.dbStore()[srcFileName].payLoad().value();
			dstFilePath = dstFolder_ + db_.dbStore()[srcFileName].name();
			File::copyFile(srcFilePath, dstFilePath);
		}
		cout << endl;
	}

	// -----< findDependencyFiles() - find all dependant files>---------------
	void CheckOut<PayLoad>::findDependencyFiles(const string &key)
	{
		Children children_set = db_.dbStore()[key].children();

		for (auto child : children_set)
		{
			if (childFilesMap_.find(child) == childFilesMap_.end())
			{
				childFilesMap_[child] = 1;
			}
			
			if (db_.dbStore()[key].children().size() != 0)
			{
				findDependencyFiles(child);
			}
		}
	}
}