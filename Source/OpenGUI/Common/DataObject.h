#pragma once

#include "OleIdl.h"

class DataObject : public IDataObject
{
public:
	// IUnknown members
	HRESULT __stdcall QueryInterface( REFIID iid , void ** ppvObject );
	ULONG   __stdcall AddRef( void );
	ULONG   __stdcall Release( void );

	// IDataObject members
	HRESULT __stdcall GetData( FORMATETC *pFormatEtc , STGMEDIUM *pmedium );
	HRESULT __stdcall GetDataHere( FORMATETC *pFormatEtc , STGMEDIUM *pmedium );
	HRESULT __stdcall QueryGetData( FORMATETC *pFormatEtc );
	HRESULT __stdcall GetCanonicalFormatEtc( FORMATETC *pFormatEct , FORMATETC *pFormatEtcOut );
	HRESULT __stdcall SetData( FORMATETC *pFormatEtc , STGMEDIUM *pMedium , BOOL fRelease );
	HRESULT __stdcall EnumFormatEtc( DWORD      dwDirection , IEnumFORMATETC **ppEnumFormatEtc );
	HRESULT __stdcall DAdvise( FORMATETC *pFormatEtc , DWORD advf , IAdviseSink * , DWORD * );
	HRESULT __stdcall DUnadvise( DWORD      dwConnection );
	HRESULT __stdcall EnumDAdvise( IEnumSTATDATA **ppEnumAdvise );

	// Constructor / Destructor
	DataObject( FORMATETC *fmtetc , STGMEDIUM *stgmed , int count );
	~DataObject();

private:
	int LookupFormatEtc( FORMATETC *pFormatEtc );

private:
	LONG m_lRefCount;
	int m_nNumFormats;

	FORMATETC* m_pFormatEtc;
	STGMEDIUM* m_pStgMedium;
};