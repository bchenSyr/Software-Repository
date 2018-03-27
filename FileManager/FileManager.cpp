#include <iostream>
#include <string>
#include <sstream>

#include "FileManager.h"

using namespace std;

// #ifdef TEST_FILESYSTEM


int main(int argc, char* argv[])
{
	cout << "Demonstrate Path Class" << endl;

	string srcFilePath = "../Files-To-Check-In-Storage/DateTime";
	string dstFilePath = "../Repository-Storage/";


	string fs = Path::fileSpec(srcFilePath, "DateTime.h");
	cout << "\n  path = " << fs;

	string path = Path::getPath(fs);
	cout << "\n getPath = " << path;

	string ffs = Path::getFullFileSpec(fs);
	cout << "\n  getFullFileSpec = " << ffs;

	string name = Path::getName(fs);
	cout << "\n  Path::getName = " << name;

	std::cout << std::endl;


	getchar();
	std::cout << "\n\n";
}
// #endif