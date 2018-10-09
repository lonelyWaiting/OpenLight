#include "DataObject.h"

DataObject::DataObject( FORMATETC *fmtetc , STGMEDIUM *stgmed , int count )
{
	// reference count must ALWAYS start at 1
	m_lRefCount = 1;
	m_nNumFormats = count;

	m_pFormatEtc = new FORMATETC[count];
	m_pStgMedium = new STGMEDIUM[count];

	for( int i = 0; i < count; i++ )
	{
		m_pFormatEtc[i] = fmtetc[i];
		m_pStgMedium[i] = stgmed[i];
	}
}

HRESULT CreateDataObject( FORMATETC* fmtetec , STGMEDIUM* stgmeds , UINT count , IDataObject **ppDataObject )
{
	if( ppDataObject == 0 )
	{
		return E_INVALIDARG;
	}

	*ppDataObject = new DataObject( fmtetec , stgmeds , count );

	return ( *ppDataObject ) ? S_OK : E_INVALIDARG;
}

HRESULT __stdcall DataObject::QueryGetData( FORMATETC *pFormatEtc )
{
	return ( LookupFormatEtc( pFormatEtc ) == -1 ) ? DV_E_FORMATETC : S_OK;
}

int DataObject::LookupFormatEtc( FORMATETC *pFormatEtc )
{
	for( int i = 0; i < m_nNumFormats; i++ )
	{
		if( ( m_pFormatEtc[i].tymed & pFormatEtc->tymed ) &&
			m_pFormatEtc[i].cfFormat == pFormatEtc->cfFormat &&
			m_pFormatEtc[i].dwAspect == pFormatEtc->dwAspect )
		{
			// return index of stored format
			return i;
		}
	}

	return -1;
}

