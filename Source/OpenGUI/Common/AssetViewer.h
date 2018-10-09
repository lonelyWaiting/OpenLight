#pragma once

#include <tchar.h>
#include <OpenLight.h>
#include <Core/Scene.h>
#include <windows.h>

namespace AssetViewer
{
	void LoadAsset( TCHAR* lpszFile )
	{
		int len = WideCharToMultiByte( CP_ACP , 0 , lpszFile , -1 , NULL , 0 , NULL , NULL );
		char* filename = new char[MAX_PATH];
		WideCharToMultiByte( CP_ACP , 0 , lpszFile , -1 , filename , len , NULL , NULL );
		OpenLight::scene.Deserialization( filename );
	}
}