#pragma once
#include <vector>
#include <string>
#include "Systemdefs.h"


//Shortcuts
#define Util Utility

//Macro definitions
//The current working directory proceeded with '\\'
#define WORKDIR Utility::getWorkingDir()
#define WORKINGDIR Utility::getWorkingDir()

#define APPDATA getAppdata()

//Normal defines
#define ENABLE_CONSOLE true
#define INVALID_ERROR -1
#define APPEND_CODE -2


class Utility
{
public:
	static void setWorkingDir(std::string dir);
	static std::string getWorkingDir() { return s_workingDir; };
	static std::string Capitalize(std::string text);


	static std::string Uncapitalize(std::string text);

	static std::string getAppdata();


	//Will return a string vector of all folder in a specified directory
	static std::string ListToString(float* list, int length);


	static std::string ListToString(std::vector<std::string> list);


	//Will return a list of all indices of keyW in str
	static std::vector<int> StrFind(std::string str, std::string keyW);


	//Will divide the string at every keyW(default space)
	static std::vector<std::string> StrSplit(std::string str, std::string keyW = " ");

public:
	//Fileutilites

	//Will return true if the given path is a file and return false if the given path is a directory
	static bool IsFile(std::string path);
	//Will return false if the given path is a file and return true if the given path is a directory
    static bool IsDir(std::string path) {return !IsFile(path);};

	static std::vector<std::string> ListAll(std::string directory = WORKDIR);


	static std::vector<std::string> ListDirectories(std::string directory = WORKDIR);


	//Will return a string vector of all files in a specified directory	
	static std::vector<std::string> ListFiles(std::string directory = WORKDIR);

	//Will return a string vector of all files in a specified directory including sub directories
	static std::vector<std::string> ListAllFiles(std::string directory = WORKDIR);


	//Will go through all files in the directory and all of it's subfolder searching for a specified file and return the path for it
	static std::string FindFile(std::string filename, std::string directory = WORKDIR);


	//Will return the file as a string vector with each element being a separate line
	static std::vector<std::string> ReadFile(std::string filePath, bool create = false);


	//Will create the specified path
	static std::string GeneratePath(std::string path);
    static std::string GenerateFile(std::string path, std::string contents, bool append = true);


	/*Will create a folder or file at the path specified. If the path doesnt exist it will create it. fill in contents to fill you file(optional)
	static std::string CreatePath(std::string path, bool createFile);*/

	/*Will create a folder or file at the path specified. If the path doesnt exist it will create it. fill in contents to fill you file(optional)
	static std::string CreatePath(std::string path, bool createFile, std::string contents, bool append = true);*/

	/*Will create a folder or file at the path specified. If the path doesnt exist it will create it. fill in contents to fill you file(optional), append to file is default
	static std::string CreatePath(std::string path, std::string contents, bool append = true);*/


	//Will return the extension of a file
	static std::string getExtension(std::string path);

	//Will return the filename of a path
	static std::string getFilename(std::string path);

	//Will return the path of folders leading up to the file
	static std::string getFilepath(std::string path);



	static std::string Error(int code);

	static void LoadErrorDef();

	//Will append a new error definition to the errorDef. Be sure to save it afterwards for permanens. If error code is -1 it will choose a new err code 
	static int AddError(std::string definition, unsigned int code = APPEND_CODE);

	static void SaveErrorDef();


	//Logging
	static std::string Log(std::string msg, std::string msgOrigin = "Log");

private:
	static std::vector<std::string> s_errorDef;
	static std::ofstream s_logFile;
	static std::string s_workingDir;

};

static std::ostream& operator<<(std::ostream& os, std::vector<std::string> in)
{
	for (std::string& str : in)
		os << str << std::endl;;
	return os;
}