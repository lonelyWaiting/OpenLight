#pragma once

#include "OleIdl.h"

class DropTarget : public IDropTarget
{
public:
	DropTarget( HWND hwnd );
	~DropTarget();

	// IUnkown implementation
	HRESULT __stdcall QueryInterface( REFIID riid , _COM_Outptr_ void **ppvObject );
	ULONG	__stdcall AddRef();
	ULONG	__stdcall Release();

	// IDropTarget implementation
	HRESULT __stdcall DragEnter( IDataObject* pDataObject , DWORD keyState , POINTL pt , DWORD* pEffect );
	HRESULT __stdcall DragOver( DWORD keyState , POINTL pt , DWORD* pEffect );
	HRESULT __stdcall Dragleave( void );
	HRESULT __stdcall Drop( IDataObject* pDataObject , DWORD keyState , POINTL pt , DWORD * pEffect );
	
private:
	// internal helper function
	DWORD DropEffect( DWORD keyStae , POINTL pt , DWORD Allowed );
	bool QueryDataObject( IDataObject* pDataObject );

private:
	long m_lRefCount;
	HWND m_hWnd;
	bool m_fAllowDrop;
};