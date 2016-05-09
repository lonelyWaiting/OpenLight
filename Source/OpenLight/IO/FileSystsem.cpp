#include <Utilities/PCH.h>
#include "FileSystem.h"

FileSystem::FileSystem()
{
	sDataFolder       = L"../Data/";
	sModelSubFolder   = L"Model/";
	sTextureSubFolder = L"Texture/";
	sLogSubFolder     = L"Log/";
	sSceneFolder      = L"Scene/";
}

FileSystem::~FileSystem()
{

}

std::wstring FileSystem::GetDataFolder()
{
	return sDataFolder;
}

std::wstring FileSystem::GetLogFolder()
{
	return sDataFolder + sLogSubFolder;
}

std::wstring FileSystem::GetModelFolder()
{
	return sDataFolder + sModelSubFolder;
}

std::wstring FileSystem::GetTextureFolder()
{
	return sDataFolder + sTextureSubFolder;
}

std::wstring FileSystem::GetSceneFolder()
{
	return sDataFolder + sSceneFolder;
}