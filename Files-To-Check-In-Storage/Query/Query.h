#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Implements Query funtion for NoSql database                      
// Author: Beier Chen, bchen22@syr.edu                                                         
// Source: Jim Fawcett
// Application: Project1 - CSE687 - Object Oriented Design, Spring 2018       
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides Query funtion to:
*  - query keys that matches specified condtions
*  - query on the keys set returned by previous queries
*
* Public Interface:
* -------------------
* 	show() - display result of query
*	getValue() - query the value part of a specified key
*	getChildren() - query the children of a specified key
*	getKeyMatchPattern() - query keys matching a specified regular-expression pattern
*	selectName() - query keys based on "name" metadata section
*	select() - query keys that contain a specified string in their metadata section
*	getKeyMatchTime() - query keys that contain values written within between a time interval
*  from() - get the set of keys returned by previous queries
*	operator&&() - get the set of keys from the "and" union of previous queries
*	operator||() - get the set of keys from the "or" union of previous queries
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 9 Feb 2018
* - first release
*/

#include"../DbCore/DbCore.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <regex>

using namespace std;

namespace NoSqlDb 
{
	template<typename T>
	class Query
	{
	public:
		using Key = string;
		using Keys = vector<Key>;
		using Children = Keys;

		Query(DbCore<T>& db) : pDb_(&db) { keys_ = db.keys(); }
		DbCore<T>& database() { return *pDb_; }

		Keys& keys() { return keys_; }
		Keys keys() const { return keys_; }
		void keys(const Keys& keys) { keys_ = keys;}

		Keys& returnKeys() { return returnKeys_; }
		Keys returnKeys() const { return returnKeys_; }
		void returnKeys(const Keys& keys) { returnKeys_ = keys; }
		
		// Query methods
		void getValue(const Key& key);
		void getChildren(const Key& key);
		Query<T>& show();	
		Query<T>& getKeyMatchPattern(const string condition);
		Query<T>& selectName(const DbElement<T>& condition);
		Query<T>& select(const DbElement<T>& condition);
		Query<T>& getKeyMatchTime(DateTime dt1, DateTime dt2);
		Query<T>& getKeyMatchTime(DateTime dt);
		Query<T>& from(vector<string>& newKeys);
		vector<string>& operator&&(Query<T>& anotherQuery);
		vector<string>& operator||(Query<T>& anotherQuery);

	private:
		DbCore<T>* pDb_;
		Keys keys_;  
		Keys returnKeys_;
	};

	//----< get the set of keys from the "or" union of previous queries>-----------------------------
	template<typename T>
	vector<string>& Query<T>::operator||(Query<T>& anotherQuery)
	{
		vector<string> temp = returnKeys();
		vector<string>::iterator it;

		for (auto i : anotherQuery.returnKeys())
		{
			it = find(temp.begin(), temp.end(), i);
			if (it == temp.end())
			{
				temp.push_back(i);
			}
		}
		returnKeys(temp);
		return returnKeys();
	}

	//----< get the set of keys from the "and" union of previous queries >-----------------------------
	template<typename T>
	vector<string>& Query<T>::operator&&(Query<T>& anotherQuery)
	{
		vector<string> temp;
		for (auto i : anotherQuery.returnKeys())
		{
			for (auto j : returnKeys())
			{
				if (i == j)
				{
					temp.push_back(i);
				}
			}
		}
		returnKeys(temp);
		return returnKeys();
	}
	
	//----< get the set of keys returned by previous queries >-----------------------------
	template<typename T>
	Query<T>& Query<T>::from(vector<string>& newKeys)
	{
		keys(newKeys);
		cout << "\n\n     The key set from previous queries:  ";
		for (auto key : keys())
		{
			cout << " " << key;
		}
		return *this;
	}

	//----< query keys that contain values written within between a time interval >-----------------------------
	template<typename T>
	Query<T>& Query<T>::getKeyMatchTime(DateTime dt1, DateTime dt2)
	{
		returnKeys_.clear();
		cout << "\n    " << dt1.time()<< " and " << dt2.time();

		for (auto key : keys())
		{
			//std::cout << "\n\n    key : " << key << " has dateTime : " << string(pDb_->dbStore()[key].dateTime());
			if (dt1 < pDb_->dbStore()[key].dateTime() && pDb_->dbStore()[key].dateTime() < dt2)
			{
				returnKeys_.push_back(key);
			}
		}
		return *this;
	}

	//----< query keys that contain values written within between a time interval >-----------------------------
	template<typename T>
	Query<T>& Query<T>::getKeyMatchTime(DateTime dt)
	{
		returnKeys_.clear();
		DateTime dtNow = dtNow.now();
		cout << "\n    " << dt.time() << " and " << dtNow.time();

		for (auto key : keys())
		{
			if (dt < pDb_->dbStore()[key].dateTime() && pDb_->dbStore()[key].dateTime() < dtNow)
			{
				returnKeys_.push_back(key);
			}
		}
		return *this;
	}

	//----< query keys that contain a specified string in their metadata section >-----------------------------
	template<typename T>
	Query<T>& Query<T>::select(const DbElement<T>& condition)
	{
		returnKeys_.clear();
		std::regex pattern1(condition.name());
		std::regex pattern2(condition.descrip());

		for (auto key : keys())
		{
			if (std::regex_match(pDb_->dbStore()[key].name(), pattern1)) 
			{
				if (std::regex_match(pDb_->dbStore()[key].descrip(), pattern2)) 
				{
					cout << "\n\n      find name that matches \"" << condition.name() << "\":  " << pDb_->dbStore()[key].name();
					cout << "\n      and desciption that matches \"" << condition.descrip() << "\":  " << pDb_->dbStore()[key].descrip();
					returnKeys_.push_back(key);
				}
			}
		}
		return *this;
	}

	//----< query keys that matches a specified name >-----------------------------
	template<typename T>
	Query<T>& Query<T>::selectName(const DbElement<T>& condition)
	{
		returnKeys_.clear();
		for (auto key : keys())
		{
			if (pDb_->dbStore()[key].name() == condition.name())
			{
				returnKeys_.push_back(key);
			}
		}
		return *this;
	}

	//----< query keys matching a specified regular-expression pattern>-----------------------------
	template<typename T>
	Query<T>& Query<T>::getKeyMatchPattern(const string condtion)
	{
		returnKeys_.clear();
		std::regex pattern(condtion);
	
		for (auto key : keys())
		{
			if (std::regex_match(key, pattern))
			{
				returnKeys_.push_back(key);
			}
		}
		return *this;
	}

	//----< display result of query >-----------------------------
	template<typename T>
	Query<T>& Query<T>::show()
	{
		cout << "\n\n     The result of query:";
		if (keys_.size() > 0)
		{
			for (auto key : returnKeys())
			{
				cout << "\n      " << key;
			}
			cout << "\n";
		}
		else
		{
			cout << "\n      doesn't find any key that matches pattern\n";
		}
		return *this;
	}

	//----< query the children of a specified key >-----------------------------
	template<typename T>
	void Query<T>::getChildren(const Key& key)
	{
		if (pDb_->contains(key))
		{
			cout << "\n     ";
			for (auto key : pDb_->dbStore()[key].children())
			{
				cout << " " << key;
			}
			cout << "\n  ";
		}
		else
		{
			cout << "\n     doesn't find key that matches \"" << key << "\"" << endl;
		}
	}

	//----< query the value of a specified key >-----------------------------
	template<typename T>
	void Query<T>::getValue(const Key& key)
	{
		if (pDb_->contains(key))
		{
			showElem(pDb_->dbStore()[key]);
		}
		else
		{
			cout << "\n      doesn't find key that matches \"" << key << "\"" << endl;
		}
	} 
}