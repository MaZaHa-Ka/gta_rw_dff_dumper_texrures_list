#include "tools.h"

//#include <iostream>
//#include <vector>
//#include <string>
//#include <sstream>
//#include <fstream>

namespace TOOLS {
	bool DirectoryCreate(std::string folderPath) // 1251 input
	{
		//std::wstring folderPathw = cp1251_to_wstring(folderPath);
		//if (CreateDirectoryW(folderPathw.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) { return true; }
		return false;
		//// Use the mkdir function to create the directory
		//if (mkdir(folderpath.c_str(), s_irwxu | s_irwxg | s_iroth | s_ixoth) != 0) {
		//	return false; // error creating directory
		//}
		//return true; // directory created successfully
	}

	bool FileExists(std::string& filePath) {
		std::ifstream file(filePath);
		return file.good();
	}

	std::vector<std::string> FileReadAllLines(std::string filePath)
	{
		//if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

		std::vector<std::string> lines;
		std::ifstream file(filePath);

		if (!file.is_open()) { return lines; }

		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}

		file.close();
		return lines;
	}

	std::string FileReadAllText(std::string filePath)
	{
		//if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

		std::ifstream file(filePath);
		std::string content;

		if (!file.is_open()) {
			//std::cerr << "Error opening the file." << std::endl;
			return content; // Return an empty string in case of an error.
		}

		std::string line;
		while (std::getline(file, line)) {
			content += line + '\n'; // Concatenate the line and a newline character.
		}

		file.close();
		return content;
	}
	void FileClearMake(std::string filePath) // or make empty file
	{
		//if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }
		std::ofstream file(filePath, std::ios::out | std::ios::trunc);
		if (file.is_open()) { file.close(); }
	}
	void FileWriteAllLines(std::string filePath, std::vector<std::string> lines)
	{
		//if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }
		if (!FileExists(filePath)) { FileClearMake(filePath); }
		std::ofstream file(filePath);
		if (!file.is_open()) { return; }
		for (const std::string& line : lines) { file << line << '\n'; }

		file.close();
	}
	void FileWrite(std::string filePath, std::string text)
	{
		//if (is_valid_utf8(filePath)) { filePath = utf8_to_cp1251(filePath); }

		// Open the file for writing. If it doesn't exist, create it.
		std::ofstream file(filePath, std::ios::out | std::ios::trunc);

		if (file.is_open()) {
			// Write the text to the file.
			file << text;

			// Close the file.
			file.close();
			//std::cout << "File written successfully." << std::endl;
		}
		//else {
		//	std::cerr << "Error opening or creating the file." << std::endl;
		//}
	}

	std::vector<std::string> GetObjReqTextures(void* p)
	{
		std::vector<std::string> vec = std::vector<std::string>();
		if (!p) { return vec; } // empty

		return vec;
	}

	void Obj(int id, const char* dff, const char* txd)
	{

	}

	bool gbIsSettedCurrDir = false;
	#include "Windows.h"
	void SetCurrentDirectory()
	{
		// mod by diktor SET CURRENT PATH
		// char currentDir[MAX_PATH]; // STATIC PATH
		// GetCurrentDirectory(MAX_PATH, currentDir);
		// printf("DIR: %s\n", currentDir);
		// memset(currentDir, 0, MAX_PATH);
		// strncpy(currentDir, "C:\\_GTA_RE\\revc\\reVC_GAME", MAX_PATH);
		// currentDir[MAX_PATH - 1] = '\0'; // Ensure null-termination
		// SetCurrentDirectory(currentDir);

		char currentDir[MAX_PATH]; // dynamic set curr dir to exe
		GetModuleFileName(NULL, currentDir, MAX_PATH);
		std::string::size_type pos = std::string(currentDir).find_last_of("\\/");
		SetCurrentDirectory(std::string(currentDir).substr(0, pos).c_str());
		gbIsSettedCurrDir = true;
	}

	std::string GetFileNameFromPath(const std::string& filePath, bool withoutExtension)
	{
		// ����� ��������� ������-����������� ���� (��������, '/' ��� '\\')
		size_t lastSlashIndex = filePath.find_last_of("/\\");
		std::string fileName;

		// ������� ��������� �� ������� ���������� �������-����������� �� ����� ������
		if (lastSlashIndex != std::string::npos) {
			fileName = filePath.substr(lastSlashIndex + 1);
		} else {
			fileName = filePath;  // ���� ����������� �� ������, ������������, ��� ������ ��� �������� ������ �����
		}

		// ���� ��������� ������� ��� ����� ��� ����������
		if (withoutExtension) {
			size_t lastDotIndex = fileName.find_last_of('.');
			if (lastDotIndex != std::string::npos) {
				fileName = fileName.substr(0, lastDotIndex); // ������� ����������
			}
		}

		return fileName;
	}

	/*std::string GetFileNameFromPath(const std::string& filePath)
	{
		// ����� ��������� ������-����������� ���� (��������, '/' ��� '\\')
		size_t lastSlashIndex = filePath.find_last_of("/\\");

		// ������� ��������� �� ������� ���������� �������-����������� �� ����� ������
		if (lastSlashIndex != std::string::npos) {
			return filePath.substr(lastSlashIndex + 1);
		}

		// ���� ����������� �� ������, ������� �������� ���� ��� ��� �����
		return filePath;
	}*/
}