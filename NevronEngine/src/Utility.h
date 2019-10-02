#pragma once
#include <src/pch.h>

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

std::string format(std::string format, ...);
std::string vformat(std::string format, va_list vl);

namespace Utility
{
	//public:
	void setWorkingDir(const std::string& dir);

	std::string getWorkingDir();

	std::string getAppdata();

#pragma region "string utilities"
	std::string Capitalize(const std::string& str);

	std::string Uncapitalize(const std::string& str);

	std::string Title(const std::string& str);

	//Will return a list of all indices of keyW in str
	std::vector<unsigned int> strFind(const std::string& str, const std::string& keyW);

	//Will divide the string at every keyW(default space)
	std::vector<std::string> strSplit(const std::string& str, const std::string& keyW,  bool ignore_quotes = false);
	std::vector<std::string> strSplit(const std::string& str, char keyW, bool ignore_quotes = false);

	std::string strClamp(const std::string& str, unsigned int size);

	//Will return a string to the keyW
	std::string strStop(const std::string& str, std::string keyW);

	std::string strPurge(const std::string& str, const std::string& keyW);
	std::string strPurge(const std::string& str, char chr);

	//Removes all characters from str that exist in pattern
	std::string strPurgeAll(const std::string& str, const std::string& pattern);

	//Will add $lead to string if it doesn't already end with $lead. Good for trailing slashes
	inline std::string strLead(const std::string& str, char lead)
	{
		if (str.back() == lead)
			return str;
		return str + lead;
	}

	//Will add $lead to string if it doesn't already end with $lead. Good for trailing slashes
	inline std::string strLead(const std::string& str, const std::string& lead)
	{
		if (lead.size() <= str.size() && str.substr(str.size() - lead.size()) == lead)
			return str;
		return str + lead;
	}

	//Will return a string vector of all folder in a specified directory
	std::string ListTostring(float* list, int length);


	std::string ListTostring(std::vector<std::string> list, const std::string& separator = ", ");
	int ParseTime(const std::string& str);

	std::string FormatTime(int seconds);

	//Will format seconds into either seconds, milliseconds or nanoseconds
	std::string FormatSeconds(float seconds);

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
	//Will go through all files in the directory and all of it's subfolder searching for a specified file and return the path for it optionally ignore extension
	std::string FindFile(const std::string& filename, bool useExtension, const std::string& directory = ASSETS);


	//Will return the file as a string vector with each element being a separate line
	std::vector<std::string> ReadFile(const std::string& filePath, bool create = false);


	//Will create the specified path
	void GeneratePath(const std::string& path);
	void GenerateFile(const std::string& path, const std::string& contents, bool append = true);

	bool Copy(const std::string& oldPath, const std::string& newPath);
	bool Rename(const std::string& oldPath, const std::string& newPath);
	//Will return the extension of a file
	std::string getExtension(const std::string& path);

	//Will return the filename of a path
	std::string getFilename(const std::string& path, bool keepExtension = true);

	//Will return the path of folders leading up to the file with a leading '\\'
	std::string getPath(const std::string& path);

	//Will only display the $depth directories back
	std::string ShortenPath(const std::string& path, int depth = 3, bool omitIndicator = false);

	//Will strClamp the string preserving the end to a set size and add "..." to the beginning
	std::string ShortenString(const std::string& str, unsigned int size, bool omitIndicator = false);

	//Will return the file path moved up one directory
	std::string DirectoryUp(const std::string& path, unsigned int steps = 1);

#pragma endregion

#pragma region "Errors and logging"
	void Error(unsigned int code);

	void LoadErrorDef();

	//Will append a new error definition to the errorDef. Be sure to save it afterwards for permanens. If error code is -1 it will choose a new err code 
	unsigned int AddError(const std::string& definition, unsigned int code = APPEND_CODE);

	void SaveErrorDef();
	//Will compose a message complete with time message origin and the original message and write it both to the console and a instance specific LogSile
	/*void Log(const std::string& msg, const std::string& msgOrigin = "Log");

	//void Log(std::initializer_list<std::string> msg, const std::string& msgOrigin = "Log");

	//Will compose a message complete with time message origin and the original message and write it to the log file but no the console
	void Log_s(const std::string& msg, const std::string& msgOrigin = "Log");*/
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

static bool parseBool(const std::string& str)
{
	return Utility::Uncapitalize(str) == "true" ? true : str == "1" ? 1 : 0;
}

static std::string FormatBool(bool boolean)
{
	return boolean ? "true" : "false";
}

//Will attempt to convert a string to float or integer. Does nothing if failed
template <typename T = float>
static T num(const std::string & str) = delete;
static float num(const std::string& str)
{
	return atof(str.c_str());
}

template <>
static int num(const std::string& str)
{
	return atoi(str.c_str());
}

template <>
static unsigned int num(const std::string& str)
{
	return atoi(str.c_str());
}

//Will attempt to convert a string to float or integer. If it fails it will return 0 and if $failed is passed set it to true
template <typename N = float>
static N num(const std::string & in, bool* str) = delete;
static float num(const std::string& str, bool* failed)
{
	if (failed != nullptr)
		* failed = false;
	try
	{
		return std::stof(str);
	}
	catch (...)
	{
		if (failed != nullptr)
			* failed = true;
		return 0;
	}
}
template<>
static int num(const std::string& in, bool* failed)
{
	if (failed != nullptr)
		* failed = false;
	try
	{
		return std::stoi(in);
	}
	catch (...)
	{
		if (failed != nullptr)
			* failed = true;
		return 0;
	}
}
template<>
static unsigned int num(const std::string& in, bool* failed)
{
	if (failed != nullptr)
		* failed = false;
	try
	{
		return std::stoi(in);
	}
	catch (...)
	{
		if (failed != nullptr)
			* failed = true;
		return 0;
	}
}


inline float min(float a, float b) { return a < b ? a : b; };
inline int min(int a, int b) { return a < b ? a : b; };
//inline unsigned int min(unsigned int a, unsigned int b) { return a < b ? a : b; };

inline float max(float a, float b) { return a > b ? a : b; };
inline int max(int a, int b) { return a > b ? a : b; };
//inline unsigned int max(unsigned int a, unsigned int b) { return a > b ? a : b; };


