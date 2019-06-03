#pragma once
#include <vector>
#include <string>
#include "Systemdefs.h"
#include <iostream>
#include "..\Math\Vector.h"
#include <fstream>
#include <cstdarg>


//Shortcuts
//#define Util Utility

//Macro definitions
//The current working directory not proceeded with '\\'
#define WORKDIR Utility::getWorkingDir()
#define WORKINGDIR Utility::getWorkingDir()
#define ASSETS WORKDIR + "Assets\\"

#define APPDATA getAppdata()

//Normal defines
#define ENABLE_CONSOLE true
#define INVALID_ERROR -1
#define APPEND_CODE -2

template <typename I> std::string hex(I w, size_t hex_len = sizeof(I) << 1) {
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}

namespace Utility
{
	//public:
	void setWorkingDir(const std::string& dir);

	std::string getWorkingDir();

	std::string getAppdata();

#pragma region "String utilities"
	std::string Capitalize(const std::string& str);

	std::string Uncapitalize(const std::string& str);

	std::string Title(const std::string& str);

	//Will return a list of all indices of keyW in str
	std::vector<unsigned int> StrFind(const std::string& str, const std::string& keyW);

	//Will divide the string at every keyW(default space)
	std::vector<std::string> StrSplit(const std::string& str, const std::string& keyW = " ");

	std::string Purge(const std::string& str, const std::string& keyW);

	//Will add $lead to string if it doesn't already end with $lead. Good for trailing slashes
	inline std::string Lead(const std::string& str, char lead)
	{
		if (str.back() == lead)
			return str;
		return str + lead;
	}

	//Will return a string vector of all folder in a specified directory
	std::string ListToString(float* list, int length);


	std::string ListToString(std::vector<std::string> list, const std::string& separator = ", ");
	int ParseTime(const std::string& str);

	std::string FormatTime(int seconds);

	bool Contains(std::vector<std::string> list, const std::string& item);

	//Returns true if $item occurs in $list
	template <typename T>
	bool in(T item, std::vector<T> list);

	template <typename N>
	std::vector<int> Search(std::vector<N> list, N query);

	std::string digits(int value, size_t nDigits);
#pragma endregion
#pragma region "File Utilities"
	//Will return true if the given path is a file and return false if the given path is a directory
	bool IsFile(std::string path);
	//Will return false if the given path is a file and return true if the given path is a directory
	bool IsDir(std::string path);
	
	//Will list all files with full path and sub-directories in specified directory
	std::vector<std::string> ListDir(const std::string& directory = ASSETS);


	std::vector<std::string> ListDirectories(const std::string& directory = ASSETS);


	//Will return a string vector of all files with full path in a specified directory	
	std::vector<std::string> ListFiles(const std::string& directory = ASSETS);

	//Will return a string vector of all files with full path in a specified directory including all sub directories
	std::vector<std::string> ListAllFiles(const std::string& directory = ASSETS);
	//Will return a string vector of all sub directories recursively with full path
	std::vector<std::string> ListAllDirectories(const std::string& directory = ASSETS);

	//Will return a string vector of all directories and files recursively with full path
	std::vector<std::string> ListAll(const std::string& directory = ASSETS);


	//Will go through all files in the directory and all of it's subfolder searching for a specified file and return the path for it
	std::string FindFile(const std::string& filename, const std::string& directory = ASSETS);


	//Will return the file as a string vector with each element being a separate line
	std::vector<std::string> ReadFile(const std::string& filePath, bool create = false);


	//Will create the specified path
	void GeneratePath(const std::string& path);
	void GenerateFile(const std::string& path, const std::string& contents, bool append = true);
	
	void Copy(const std::string& oldPath, const std::string& newPath);
    void Rename(const std::string& oldPath, const std::string& newPath);
	//Will return the extension of a file
	std::string getExtension(const std::string& path);

	//Will return the filename of a path
	std::string getFilename(const std::string& path, bool keepExtension = true);

	//Will return the path of folders leading up to the file with a leading '\\'
	std::string getPath(const std::string& path);

	//Will return the file path moved up one directory
	std::string DirectoryUp(const std::string& path, unsigned int steps = 1);

#pragma endregion

#pragma region "Errors and logging"
	void Error(unsigned int code);

	void LoadErrorDef();

	//Will append a new error definition to the errorDef. Be sure to save it afterwards for permanens. If error code is -1 it will choose a new err code 
	unsigned int AddError(const std::string& definition, unsigned int code = APPEND_CODE);

	void SaveErrorDef();
	//Will compose a message complete with time message origin and the original message and write it both to the console and a instance specific logfile
	void Log(const std::string& msg, const std::string& msgOrigin = "Log");

	//void Log(std::initializer_list<std::string> msg, const std::string& msgOrigin = "Log");

	//Will compose a message complete with time message origin and the original message and write it to the log file but no the console
	void Log_s(const std::string& msg, const std::string& msgOrigin = "Log");
	//void Log_s(std::initializer_list<std::string> msg, const std::string& msgOrigin = "Log");

};

/*std::ostream& operator<<(std::ostream& os, std::vector<std::string> in)
{
	for (std::string& str : in)
		os << str << std::endl;;
	return os;
}
//For shortcuts in writing*/
namespace Util = Utility;

template<typename T>
inline bool Utility::in(T item, std::vector<T> list)
{
	for (T i : list)
	{
		if (i == item)
			return true;
	}
	return false;
}

template<typename N>
inline std::vector<int> Utility::Search(std::vector<N> list, N query)
{
	std::vector<N> result;
	for (N i : list)
	{
		if (i == query)
			result.push_back(i);
	}
	return result;
}


/*//Make sure the logs folder exists
GeneratePath(WORKDIR + "Logs");

if (!s_logFile.is_open())
//fopen((WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt").c_str());
s_logFile.open(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt", std::ios::app);
std::string fullMsg = "(" + msgOrigin + " @ " + Time::getDateAndTime(Time::ONLY_TIME) + "): \n" + msg;

s_logFile.write((fullMsg + "\n").c_str(), fullMsg.size() + 1);

if (ENABLE_CONSOLE)
std::cout.write((fullMsg + "\n").c_str(), fullMsg.size() + 1);
s_logFile.close();
return;*/