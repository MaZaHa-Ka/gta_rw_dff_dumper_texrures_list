#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#define TOOLS_PATHS1 "log\\_\\txdlog1.txt"
//#define TOOLS_PATHS1 "txdlog.txt"
namespace TOOLS {
	bool DirectoryExists(std::string folderPath);
	bool DirectoryCreate(std::string folderPath);

	bool FileExists(std::string& filePath);
	void FileWrite(std::string filePath, std::string text);
	std::vector<std::string> FileReadAllLines(std::string filePath);
	std::string FileReadAllText(std::string filePath);
	void FileWriteAllLines(std::string filePath, std::vector<std::string> lines);
	std::vector<std::string> GetObjReqTextures(void* p);

	extern bool gbIsSettedCurrDir;
	void SetCurrentDirectory();
	std::string GetFileNameFromPath(const std::string& filePath, bool withoutExtension/* = false*/);
	////std::string GetFileNameFromPath(const std::string& filePath);

	bool createDirectory(const std::string& path);
	bool copyFile(const std::string& source, const std::string& destination);

	//https://github.com/Ma-ZaHaKa/TOOLS/blob/main/TOOLS/CPP/Tools.h
	std::string ToUpper(std::string strToConvert);
	std::string ToLower(std::string strToConvert);
	std::string Trim(std::string str);
	std::string Replace(std::string input, std::string target, std::string replacement);
}