#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include <clocale>
#include <locale>
#include <windows.h>

using namespace std;

/////////////////////////////////////////////////////////
// File
class File
{
public:

	static void copyFile(const string& srcFilePath, const string& dstFilePath);

};

void File::copyFile(const string& srcFilePath, const string& dstFilePath)
{
	ifstream in(srcFilePath);
	ofstream out(dstFilePath);
	string temp;

	if (in.fail())
	{
		cout << "\n Error: Fail to open the source file." << endl;
		in.close();
		out.close();
		return;
	}

	while (in.good())
	{
		char ch = in.get();
		if (!in.good())
			break;
		temp += ch;
	}
	in.close();

	out << temp; // copy file
	out.close();

	cout << "\n\n   copying file from :" << srcFilePath << " to: " << dstFilePath;
}



/////////////////////////////////////////////////////////
// Path
class Path
{
public:
	static std::string getFullFileSpec(const std::string& fileSpec);
	static std::string getPath(const std::string& fileSpec);
	static std::string getName(const std::string& fileSpec, bool withExt = true);
	static std::string getExtension(const std::string& fileSpec);
	static std::string fileSpec(const std::string& path, const std::string& name);
	static std::string toLower(const std::string& src);
	static std::string toUpper(const std::string& src);
};


//----< convert string to lower case chars >---------------------------
std::string Path::toLower(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += tolower(src[i]);
	return temp;
}

//----< convert string to upper case chars >---------------------------
std::string Path::toUpper(const std::string& src)
{
	std::string temp;
	for (size_t i = 0; i<src.length(); ++i)
		temp += toupper(src[i]);
	return temp;
}

//----< get path from fileSpec >---------------------------------------
std::string Path::getName(const std::string &fileSpec, bool withExt)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
	{
		pos = fileSpec.find_last_of("\\");
		if (pos >= fileSpec.length())
		{
			// no path prepended
			if (withExt)
				return fileSpec;
			else
			{
				// remove ext
				size_t pos = fileSpec.find(".");
				if (pos > fileSpec.size())
					return fileSpec;
				return fileSpec.substr(0, pos - 1);
			}
		}
	}
	if (withExt)
		return fileSpec.substr(pos + 1, fileSpec.length() - pos);
	else
	{
		// remove ext
		size_t pos2 = fileSpec.find(".", pos);
		if (pos2 > fileSpec.size())
			// no ext
			return fileSpec.substr(pos + 1);
		return fileSpec.substr(pos + 1, pos2 - pos - 1);
	}
}

//----< get extension from fileSpec >----------------------------------
std::string Path::getExtension(const std::string& fileSpec)
{
	size_t pos1 = fileSpec.find_last_of('/');
	size_t pos2 = fileSpec.find_last_of('\\');
	size_t pos = fileSpec.find_last_of('.');
	// handle ../ or ..\\ with no extension
	if (pos1 < fileSpec.length() || pos2 < fileSpec.length())
	{
		if (pos < min(pos1, pos2))
			return std::string("");
	}
	// only . is extension delimiter
	if (0 <= pos && pos < fileSpec.length())
		return toLower(fileSpec.substr(pos + 1, fileSpec.length() - pos));
	return std::string("");
}

//----< get path from fileSpec >---------------------------------------
std::string Path::getPath(const std::string &fileSpec)
{
	size_t pos = fileSpec.find_last_of("/");
	if (pos >= fileSpec.length())
		pos = fileSpec.find_last_of("\\");
	if (pos >= fileSpec.length())
		return ".";
	if (fileSpec.find(".", pos + 1))
		return fileSpec.substr(0, pos + 1);
	return fileSpec;
}

//----< get absoluth path from fileSpec >------------------------------
std::string Path::getFullFileSpec(const std::string &fileSpec)
{
	const size_t BufSize = 256;
	char buffer[BufSize];
	char filebuffer[BufSize];  // don't use but GetFullPathName will
	char* name = filebuffer;
	::GetFullPathNameA(fileSpec.c_str(), BufSize, buffer, &name);
	return std::string(buffer);
}

//----< create file spec from path and name >--------------------------
std::string Path::fileSpec(const std::string &path, const std::string &name)
{
	std::string fs;
	size_t len = path.size();
	if (path[len - 1] == '/' || path[len - 1] == '\\')
		fs = path + name;
	else
	{
		if (path.find("/") < path.size())
			fs = path + "/" + name;
		else if (path.find("\\") < path.size())
			fs = path + "\\" + name;
		else
			fs = path + "/" + name;
	}
	return fs;
}

