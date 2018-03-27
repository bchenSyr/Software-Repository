#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  
// Author: Beier Chen, bchen22@syr.edu                                                         
// Source: Jim Fawcett
// Application: Project1 - CSE687 - Object Oriented Design, Spring 2018       
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations
* - DbElement provides the value part of the key-value database.
* The package also provides functions for displaying
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

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"

using namespace std;

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database
	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;

		// methods to get and set DbElement fields
		std::string& name() { return name_; }
		std::string name() const { return name_; } // getter
		void name(const std::string& name) { name_ = name; }  // setter

		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		void descrip(const std::string& name) { descrip_ = name; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		void children(const Children& children) { children_ = children; }

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		void payLoad(const T& payLoad) { payLoad_ = payLoad; }

		// deletion of child relationship
		void deleteChild(const Key& childToDel);

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
		T payLoad_;
	};

	//----< deletion of child relationship >-------------------------------
	template<typename T>
	void DbElement<T>::deleteChild(const Key& childToDel)
	{
		for (int i = 0; i < children().size(); i++) {
			if (children()[i] == childToDel) 
			{
				children().erase(children().begin() + i);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - does not provide editing, querying, or persistance operations
	template <typename T>
	class DbCore
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;

		// methods to access database elements
		Keys keys();
		bool contains(const Key& key);
		size_t size();
		void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }

		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;

		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }

		// methods to get and set the private database hash-map storage
		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

		void deletePair(const Key& key);

	private:
		DbStore dbStore_;
		bool doThrow_ = false;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore<T> methods

	//----< delet key/value pair >----------------------------------
	template<typename T>
	void DbCore<T>::deletePair(const Key& key) 
	{
		for (auto item : dbStore_)
		{
			// remove the key that needs to be removed from the child collection of other records
			item.second.deleteChild(key);
		}

		iterator iter = dbStore_.find(key);
		dbStore_.erase(iter);
	}

	//----< does db contain this key? >----------------------------------
	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}

	//----< returns current key set for db >-----------------------------
	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}

	//----< return number of db elements >-------------------------------
	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}

	//----< extracts value from db with key >----------------------------
	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			if (doThrow_)
				throw(std::exception("key does not exist in db"));
			else
				return (dbStore_[key] = DbElement<T>());
		}
		return dbStore_[key];
	}

	//----< extracts value from db with key >----------------------------
	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			throw(std::exception("key does not exist in db"));
		}
		return dbStore_[key];
	}

	/////////////////////////////////////////////////////////////////////
	// display functions
	//----< display database key set >-----------------------------------
	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n   Database keys are : \n      ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}

	//----< show record header items >-----------------------------------
	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n\n    ";
		out << std::setw(15) << std::left << "Key";
		out << std::setw(10) << std::left << "Name";
		out << std::setw(26) << std::left << "DateTime";	
		out << std::setw(30) << std::left << "Description";
		out << std::setw(25) << std::left << "Payload";
		out << "\n    ";
		out << std::setw(15) << std::left << "-----------";
		out << std::setw(10) << std::left << "--------";
		out << std::setw(26) << std::left << "------------------------";		
		out << std::setw(30) << std::left << "---------------------------";
		out << std::setw(25) << std::left << "-----------------------";
	}

	//----< show record header items without Key section>-----------------------------------
	inline void showHeaderNoKey(std::ostream& out = std::cout)
	{
		out << "\n\n    ";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(15) << std::left << "Name";
		out << std::setw(30) << std::left << "Description";
		out << std::setw(25) << std::left << "Payload";
		out << "\n    ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(15) << std::left << "-------------";
		out << std::setw(30) << std::left << "---------------------------";
		out << std::setw(25) << std::left << "-----------------------";
		out << "\n    ";
	}

	//----< display DbElements >-----------------------------------------
	template<typename T>
	void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
	{
		out << std::setw(10) << std::left << el.name();
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(30) << std::left << el.descrip();
		out << std::setw(25) << std::left << el.payLoad();

		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n      child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
		out << "\n  ";
	}

	//----< display DbElements >-----------------------------------------
	template<typename T>
	void showElemWithKey(const DbCore<T>& db, string key, std::ostream& out = std::cout)
	{
		DbElement<T> el = db.dbStore()[key];

		out << "\n    " << std::setw(15) << std::left << key;
		out << std::setw(10) << std::left << el.name();
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(30) << std::left << el.descrip();
		out << std::setw(25) << std::left << el.payLoad();

		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n      child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
		out << "\n  ";
	}

	//----< display DbElements >-----------------------------------------
	void showElem(const DbElement<vector<string>>& el, std::ostream& out = std::cout)
	{
		out << std::setw(10) << std::left << el.name();
		out << std::setw(26) << std::left << std::string(el.dateTime());	
		out << std::setw(30) << std::left << el.descrip();
		
		if (el.payLoad().size() > 0)
		{
			for (auto str : el.payLoad())
			{
				if ( str == el.payLoad()[0]) {
					out << str;
				}	
				else 
				{
					out << " " << str;
				}
			}
		}
		typename DbElement<vector<string>>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n      child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
		out << "\n  ";
	}

	//----< display all records in database >----------------------------
	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			out << "\n    " << std::setw(15) << std::left << item.first;
			showElem(item.second, out);
		}
	}

	//----< display all records in database for package>----------------------------
	template<typename T>
	inline void showDbForReq8(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		// show header
		out << "\n\n    ";
		out << std::setw(15) << std::left << "Key";
		out << std::setw(15) << std::left << "Name";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(15) << std::left << "Description";
		out << "\n    ";
		out << std::setw(15) << std::left << "--------------";
		out << std::setw(15) << std::left << "--------------";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(30) << std::left << "--------------";

		for (auto item : db.dbStore())
		{
			out << "\n    " << std::setw(15) << std::left << item.first;

			out << std::setw(15) << std::left << item.second.name();
			out << std::setw(26) << std::left << std::string(item.second.dateTime());
			out << std::setw(30) << std::left << item.second.descrip();

			typename DbElement<T>::Children children = item.second.children();
			if (children.size() > 0)
			{
				out << "\n      child: ";
				for (auto key : children)
				{
					out << " " << key;
				}
			}
			out << "\n  ";
		}
	}
}
