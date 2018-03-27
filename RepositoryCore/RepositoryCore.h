#pragma once
/////////////////////////////////////////////////////////////////////////////////////
// RepositoryCore.h - Repository function										               //
// Author: Beier Chen, bchen22@syr.edu												 	   //
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018   //   
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* provides means to check-in, version, browse, and check-out source code packages.
*
* Required Files:
* ---------------
* NoSqlDb.h, PayLoad.h, FileManager.h, Version.h, CheckIn.h, CheckOut.h Browse.h
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
#include "../CheckIn/CheckIn.h"
#include "../CheckOut/CheckOut.h"
#include "../Browse/Browse.h"

using namespace NoSqlDb;
using namespace std;

namespace RepositoryCore
{
	template<typename PayLoad>
	class Repository
	{
	public:
		Repository(DbCore<PayLoad>& db) : db_(db) , checkIn_(db), checkOut_(db){ keys_ = db.keys(); }

		Keys& keys() { return keys_; }
		Keys keys() const { return keys_; }
		void keys(const Keys& keys) { keys_ = keys; }

		CheckIn<PayLoad>& checkIn() { return checkIn_; }
		CheckIn<PayLoad> checkIn() const { return checkIn_; }

		CheckOut<PayLoad>& checkOut() { return checkOut_; }
		CheckOut<PayLoad> checkOut() const { return checkOut_; }

		DbCore<PayLoad>& db() { return db_; }
		DbCore<PayLoad> db() const { return db_; }

	private:
		DbCore<PayLoad>& db_;
		Keys keys_;
		CheckIn<PayLoad> checkIn_;
		CheckOut<PayLoad> checkOut_;
	};
}


