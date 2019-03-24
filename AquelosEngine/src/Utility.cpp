#include "Utility.h"


#include <fstream>
#include <iostream>
#include <filesystem>
#include <Windows.h>

#include <shlobj_core.h>
#include <sys/types.h>


#include "Time.h"
std::vector<std::string> Utility::s_errorDef = {};
std::ofstream Utility::s_logFile = std::ofstream();
std::string Utility::s_workingDir = WORKDIR;


void Utility::setWorkingDir(std::string dir)
{
	dir = getFilepath(dir);
	Log("Working directory changed from " + s_workingDir + " to " + dir);
	s_workingDir = dir;
}

std::string Utility::Capitalize(std::string text)
{
	for (int i = 0; i < text.size(); i++)
	{
		text[i] = toupper(text[i]);
	}
	return text;
}

std::string Utility::Uncapitalize(std::string text)
{
	for (int i = 0; i < text.size(); i++)
	{
		text[i] = tolower(text[i]);
	}
	return text;
}

std::string Utility::getAppdata()
{
	TCHAR szPath[MAX_PATH];

	SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath));

	return std::string(szPath) + "\\" + APPNAME + "\\";

}

std::string Utility::ListToString(float * list, int length)
{
	std::string out;
	for (int i = 0; i < length; i++)
	{
		out += std::to_string(list[i]);
		if (i != length - 1)
			out += "\n";
	}
	return out;
}

std::string Utility::ListToString(std::vector<std::string> list)
{
	std::string out;
	for (int i = 0; i < list.size(); i++)
	{
		out += (list[i]);
		if (i != list.size() - 1) //Putting carriage return on all entries except the last
			out += "\n";
	}
	return out;
}

std::vector<int> Utility::StrFind(std::string str, std::string keyW)
{
	std::vector<int> results;

	for (unsigned int i = 0; i < str.length() - (keyW.length() - 1); i++)
	{
		if (str.substr(i, keyW.length()) == keyW)
		{
			results.push_back(i);
		}
	}
	return results;
}

std::vector<std::string> Utility::StrSplit(std::string str, std::string keyW)
{
	str += keyW;
	std::vector<std::string> results;
	for (int i = 0; i < str.length(); i++)
	{
		if (str.substr(i, keyW.length()) == keyW) //Cursor att keyword
		{
			results.push_back(str.substr(0, i)); //Push left bit to results
			str = str.substr(i + 1); //And remove it from string and reset cursor
			i = 0;
		}
	}
	results.push_back(str);
	return results;
}

bool Utility::IsFile(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			//it's a directory
			return false;
		}
		else if (s.st_mode & S_IFREG)
		{
			//it's a file
			return true;
		}
		else
		{
			//something else
			return false;
		}
	}
	else
	{
		//error
		return false;
	}
}

//Will list all files and sub-directories in specified directory
std::vector<std::string> Utility::ListAll(std::string directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular || p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string());
	return r;
}

//Will list all sub-directorie in specified directory
std::vector<std::string> Utility::ListDirectories(std::string directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::directory)
			r.push_back(p.path().string());
	return r;
}

std::vector<std::string> Utility::ListFiles(std::string directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
	return r;
}

std::vector<std::string> Utility::ListAllFiles(std::string directory)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(directory))
		if (p.status().type() == std::filesystem::file_type::regular)
			r.push_back(p.path().string());
	return r;
}

std::string Utility::FindFile(std::string filename, std::string directory)
{
	size_t pos = filename.find_last_of("\\");
	if (pos >= 0)
	{
		filename = filename.substr(pos + 1, filename.length());
	}

	std::vector<std::string> files = ListAllFiles(directory);
	for (int i = 0; i < files.size(); i++)
	{
		size_t pos = files[i].find_last_of("\\");
		if (pos != std::string::npos)
		{
			std::string without = files[i].substr(pos + 1, files[i].length());

			if (without == filename)
			{
				return files[i];
			}
		}
	}
	return "";
}

std::vector<std::string> Utility::ReadFile(std::string filePath, bool create)
{
	if (create)
		GenerateFile(filePath, "");

	std::ifstream file(filePath);
	std::string line;
	std::vector<std::string> fileCont; //File content

	if (file.is_open())
	{
		int i = 0;
		while (file.good())
		{
			getline(file, line);
			fileCont.push_back(line);

			i++;
		}
	}
	else
	{
		MessageBox(nullptr, "File could not be opened", filePath.c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	file.close();
	return fileCont;

}

std::string Utility::GeneratePath(std::string path)
{
	//path is to long
	if (path.size() >= 248)
	{
		Error(1);
		return path;
	}
	std::vector<int> folderAt = StrFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}
	
	//Creates the last folder if not followed by '\\'
	std::filesystem::create_directory(path + "\\");

	return path;
}

std::string Utility::GenerateFile(std::string path, std::string contents, bool append)
{
	//Path is to long
	if (path.size() >= 248)
	{
		Error(1);
		return path;
	}

	//Creates all directories leading up to the file
	std::vector<int> folderAt = StrFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}
	std::ofstream fstream(path, append ? std::ios::app : std::ios::trunc);
	fstream << contents;
	fstream.close();
	
	return path;
}

/*std::string Utility::CreatePath(std::string path, bool createFile)
{
	std::vector<int> folderAt = StrFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}
	if (createFile)
	{
		std::ofstream file(path, std::fstream::in | std::fstream::out);
		file.close();
	}


	return path;
}

std::string Utility::CreatePath(std::string path, std::string contents, bool append)
{
	std::vector<int> folderAt = StrFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}
	//File creation is given by contents
	{
		std::ofstream file(path, std::fstream::in | std::fstream::out | (append ? std::fstream::app : std::fstream::trunc));

		//Remove empty lines in the beginning
		while (true)
		{
			std::string emptyLine = contents.substr(0, 1); //If it starts with a blank
			if (contents.size() <= 1 || (emptyLine != "" && emptyLine != "\n"))
				break;
			contents = contents.substr(1); //Remove it
		}

		file << contents;
		file.close();
	}


	return path;
}

std::string Utility::CreatePath(std::string path, bool createFile, std::string contents, bool append)
{
	std::vector<int> folderAt = StrFind(path, "\\");
	for (unsigned int i = 1; i < folderAt.size(); i++)
	{
		std::string tmpPath = path.substr(0, folderAt[i]);
		std::filesystem::create_directory(tmpPath);
		//CreateDirectory(tmpPath.c_str(), nullptr);
	}
	if (createFile)
	{
		std::ofstream file(path, std::fstream::in | std::fstream::out | (append ? std::fstream::app : std::fstream::trunc));

		//Remove empty lines in the beginning
		while (true)
		{
			std::string emptyLine = contents.substr(0, 1); //If it starts with a blank
			if (contents.size() <= 1 || (emptyLine != "" && emptyLine != "\n"))
				break;
			contents = contents.substr(1); //Remove it
		}

		file << contents;
		file.close();
	}
	else
	{
		std::filesystem::create_directory(path);
	}

	return path;
}*/

std::string Utility::getExtension(std::string path)
{
	return path.substr(path.find_last_of("."));
}

std::string Utility::getFilename(std::string path)
{
	size_t slashIndex = path.find_last_of("\\");
	return path.substr(slashIndex + 1, path.find_last_of(".") - slashIndex - 1);
}

std::string Utility::getFilepath(std::string path)
{
	//Chack if user already specified a directory
	if (!IsFile(path))
		return path;

	size_t slashIndex = path.find_last_of("\\");
	return path.substr(0, slashIndex + 1);
}

std::string Utility::Error(int code)
{

	//Load errordef if its not loaded
	if (s_errorDef.size() == NULL)
		LoadErrorDef();

	std::string explanation = "Undefined Error";

	for (int i = 0; i < s_errorDef.size(); i++)
	{
		if (s_errorDef[i].substr(0, 3) == std::to_string(code) + ": ")
		{
			explanation = s_errorDef[i].substr(3);
			break;
		}
	}


	//Print and prompt the user
	MessageBox(NULL, explanation.c_str(), "Error", MB_ICONWARNING | MB_OK);
	std::cout << "(Error): " << code << "; " << explanation << std::endl;

	return explanation;
}

void Utility::LoadErrorDef()
{
	s_errorDef = Utility::ReadFile(WORKDIR + "Assets\\Errordef.txt");
}

int Utility::AddError(std::string definition, unsigned int code)
{
	if (s_errorDef.size() == NULL)
		LoadErrorDef();

	if (code == APPEND_CODE)
		code = s_errorDef.size();


	//Check if proposed error already exists, if so return
	if (s_errorDef[0].size() > 0)
		for (auto err : s_errorDef)
		{
			//Check to see if error code already exists
			if (err.substr(0, 3) == code + ": ")
				return code;
			//Check to see if error definition already exists
			if (err.substr(3) == definition)
				return stoi(err.substr(0, 1));
		}

	s_errorDef.push_back(std::to_string(code) + ": " + definition);
	return code;
}

void Utility::SaveErrorDef()
{
	Utility::GenerateFile(WORKDIR + "Assets\\Errordef.txt", Utility::ListToString(s_errorDef), false);
}

std::string Utility::Log(std::string msg, std::string msgOrigin)
{
	//Make sure the logs folder exists
	GeneratePath(WORKDIR + "Logs");

	if (!s_logFile.is_open())
		s_logFile.open(WORKDIR + "Logs\\" + Time::startDateAndTime + ".txt", std::ios::app);
	std::string fullMsg = "(" + msgOrigin + " @ " + Time::getDateAndTime(Time::ONLY_TIME) + "): " + msg;

	s_logFile << fullMsg << std::endl;

	if (ENABLE_CONSOLE)
		std::cout << fullMsg << std::endl;
	return msg;
}