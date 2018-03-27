#pragma once
/////////////////////////////////////////////////////////////////////////////////////
// CheckIn.h - check-in funtion for Repository											   //
// Author: Beier Chen, bchen22@syr.edu												 	   //
// Application: Project 2 - CSE687 - Object Oriented Design, Spring 2018   //   
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* provides the functionality to start a new package version by accepting files,
* appending version numbers to their filenames, creating metadata,
* storing the files in a designated location.
*
* Required Files:
* ---------------
* NoSqlDb.h, PayLoad.h, FileManager.h, Version.h, Graph.h
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
#include "../FileManager/FileManager.h"
#include "../Version/Version.h"
#include "../GraphWalkDemo/GraphWalkDemo/Graph.h"

using namespace NoSqlDb;
using namespace std;

namespace RepositoryCore 
{
	template<typename PayLoad>
	class CheckIn
	{
	public:
		CheckIn() = default;
		CheckIn(DbCore<PayLoad>& db) : db_(db), version_(db) {}
		void checkIn(const string &sourceFilePath, DbElement<PayLoad>& elem);

		string& logInName() { return logInName_; }
		string logInName() const { return logInName_; }
		void logIn(const string& logInName) { logInName_ = logInName; }

		bool close(const string &filePath);
		bool checkDependency(const string &key);

		using Graph = Graph<std::string>;
		using Node = Node<std::string>;
		using Sptr_graph = Sptr_graph<std::string>;

		Graph& graph() { return graph_; }
		Graph graph() const { return graph_; }
		void graph(Graph& graph) { graph_ = graph; }

	private:
		DbCore<PayLoad>& db_;
		string logInName_;
		Version<PayLoad> version_;
		Graph graph_;
		void addNode(Key key);
		string dstFolder_ = "../Storage/";
	};

	// -----< checkIn() - check in a file >---------------
	template<typename PayLoad>
	void CheckIn<PayLoad>::checkIn(const string &srcFilePath, DbElement<PayLoad> &elem)
	{
		string srcFilename = Path::getName(srcFilePath);	

		int versionNum = version_.inquireVersion(srcFilename);
			
		string dstFileName = srcFilename + "." + to_string(versionNum); 
		string dstFilePath = dstFolder_ + dstFileName;

		for (size_t i = 0; i < elem.children().size(); i++)
		{
			int childVersionNum = version_.inquireVersion(elem.children()[i]);
			string newChildName = elem.children()[i] + "." + to_string(childVersionNum);
			elem.children()[i] = newChildName;
		}

		elem.name(srcFilename);
		elem.payLoad().value(dstFilePath);
		elem.payLoad().author(logInName());
		elem.payLoad().version(versionNum);
		elem.payLoad().setOpenState(true);

		db_.dbStore()[dstFileName] = elem;

		File::copyFile(srcFilePath, dstFilePath);

		cout << endl;
	}

	// -----< close() - close check-in for a file >---------------
	template<typename PayLoad>
	bool CheckIn<PayLoad>::close(const string &filePath)
	{
		string fileName = Path::getName(filePath);
		cout << "\n - try to close file " << fileName;
		Query<PayLoad> query(db_);
		Conditions<PayLoad> condition;
		condition.name(fileName);
		query.select(condition); 		// query the records with fileName

		if (query.keys().size() != 0) // if the file exists in db
		{
			for (auto key : query.keys())
			{
				// get the record with open check-in state
				if (db_.dbStore()[key].payLoad().isOpen())
				{
					// check the check-in state of all the dependent file
					if (checkDependency(key))
					{
						cout << "\n   all dependant files's check-in state are closed, close file \"" << key << "\" " << endl;
						db_[key].payLoad().setOpenState(false);
						return true;
					}
				}
			}
		}
		return false;
	}

	// -----< checkDependency() - used by close(), check the check-in state of dependant files >---------------
	template<typename PayLoad>
	bool CheckIn<PayLoad>::checkDependency(const string &key)
	{
		graph().nodeMap().clear();
		db_.keyToCheckIn(key);
		addNode(key); // construct the dependency graph
	
		// used by walker(), to check each child's check-in state 
		auto proc = [](Sptr_graph pNode, DbCore<PayLoad>& db_)
		{ 	
			if (db_[pNode->name()].payLoad().isOpen() && pNode->name() != db_.keyToCheckIn())
			{
				std::cout << "\n   --> " << pNode->name();
				cout << "  check-in state: open, cannot close file " << db_.keyToCheckIn() << endl;
				return false;
			}
			else if(pNode->name() != db_.keyToCheckIn())
			{
				std::cout << "\n   --> " << pNode->name();
				cout << " check-in state: closed";
				return true;
			}
			return true;
		};

		using P = decltype(proc);
		using V = std::string;
		using D = DbCore<PayLoad>&;

		cout << "\n   checking the check-in state of dependency files of " << key << " : ";
		Sptr_graph pNode = graph().find(key);

		//  travse the graph
		if (walker(DFS_r_ltr<V, P, D>, proc, db_, graph(), pNode, WALKALL))
		{
			return true; // return true if all files are closed
		}
		else return false; // return false if found a file is open check - in
	}

	// -----< addNode() - construct the graph with child relationship of records >---------------
	template<typename PayLoad>
	void CheckIn<PayLoad>::addNode(Key key)
	{
		if (db_.dbStore().find(key) != db_.dbStore().end())
		{
			if (graph().containsNode(key) == false)
			{			
				graph().addNode(Sptr_graph(new Node(key)));
			}

			if (db_.dbStore()[key].children().size() != 0)
			{
				Children key_set = db_.dbStore()[key].children();
				for (auto child : key_set)
				{
					graph().addEdge(key, child);

					// add node and edges to the graph recursively
					if (graph().containsNode(child) == false) addNode(child); 
				}
			}
		}
		else
		{
			cout << "\n - item " << key << " doesn't exists";
		}
	}
}