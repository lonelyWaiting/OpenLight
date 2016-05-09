#pragma once

#include <string>

class FileSystem
{
public:
	FileSystem();

	~FileSystem();

	std::wstring GetLogFolder();
	std::wstring GetDataFolder();
	std::wstring GetModelFolder();
	std::wstring GetTextureFolder();
	std::wstring GetSceneFolder();

public:
	std::wstring sDataFolder;
	std::wstring sModelSubFolder;
	std::wstring sTextureSubFolder;
	std::wstring sLogSubFolder;
	std::wstring sSceneFolder;
};