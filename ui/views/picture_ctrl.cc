#include "StdAfx.h"

#include <GdiPlus.h>

#include "ui/views/picture_ctrl.h"

using namespace Gdiplus;

BEGIN_MESSAGE_MAP(CPictureCtrl, CStatic)
  ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

namespace {
// TODO this method has problem, the right way to draw the control is to redraw
// all windows below the control area.
void InvalidateRight(CWnd* wnd) {
  wnd->Invalidate();
  // TODO only invalidate the child rect.
  wnd->GetParent()->Invalidate();
}

}

//Macro to release COM Components
#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif
#define SAFE_RELEASE(x) do{\
							if((x) != NULL)\
							{\
								while((x)->Release() != 0);\
								(x) = NULL;\
							}\
						}while(0)

CPictureCtrl::CPictureCtrl(void)
	: CStatic()
	, m_pStream(NULL)
	, m_bIsPicLoaded(FALSE) {}

CPictureCtrl::~CPictureCtrl(void)
{
  //Tidy up
	FreeData();
}

BOOL CPictureCtrl::LoadFromStream(IStream *piStream)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Check for validity of argument
	if(piStream == NULL)
	{
		SetLastError(ERROR_INVALID_ADDRESS);
		return FALSE;
	}

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if(dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Rewind the argument stream
	LARGE_INTEGER lInt;
	lInt.QuadPart = 0;
	piStream->Seek(lInt, STREAM_SEEK_SET, NULL);

	//Read the lenght of the argument stream
	STATSTG statSTG;
	dwResult = piStream->Stat(&statSTG, STATFLAG_DEFAULT);
	if(dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Copy the argument stream to the class stream
	piStream->CopyTo(m_pStream, statSTG.cbSize, NULL, NULL);

	//Mark as loaded
	m_bIsPicLoaded = TRUE;
  
  InvalidateRight(this);

	return TRUE;
}

BOOL CPictureCtrl::LoadFromStream(BYTE* pData, size_t nSize)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if(dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Copy argument data to the stream
	dwResult = m_pStream->Write(pData, (ULONG)nSize, NULL);
	if(dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Mark as loaded
	m_bIsPicLoaded = TRUE;

  InvalidateRight(this);

	return TRUE;
}

BOOL CPictureCtrl::LoadFromFile(CString &szFilePath)
{
	//Set success error state
	SetLastError(ERROR_SUCCESS);
	FreeData();

	//Allocate stream
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if(dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}

	//Open the specified file
	CFile cFile;
	CFileException cFileException;
	if(!cFile.Open(szFilePath, CStdioFile::modeRead | CStdioFile::typeBinary, &cFileException))
	{
		SetLastError(cFileException.m_lOsError);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}

	//Copy the specified file's content to the stream
	BYTE pBuffer[1024] = {0};
	while(UINT dwRead = cFile.Read(pBuffer, 1024))
	{
		dwResult = m_pStream->Write(pBuffer, dwRead, NULL);
		if(dwResult != S_OK)
		{
			SetLastError(dwResult);
			SAFE_RELEASE(m_pStream);
			cFile.Close();
			return FALSE;
		}
	}

	//Close the file
	cFile.Close();

	//Mark as Loaded
	m_bIsPicLoaded = TRUE;

  
  InvalidateRight(this);

	return TRUE;
}

 BOOL CPictureCtrl::LoadFromResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType)
 {
 	//Set success error state
 	SetLastError(ERROR_SUCCESS);
 	FreeData();
 
 	//Locate the resource
 	HRSRC hResource = FindResource(hModule, lpName, lpType);
 	if(hResource == NULL)
 	{
 		return FALSE;
 	}
 
 	//Get the size of the resource
 	DWORD dwResourceSize = SizeofResource(hModule, hResource);
 	if(dwResourceSize == 0)
 	{
 		return FALSE;
 	}
 
 	//Load the Resource
 	HGLOBAL hGlobalResource = LoadResource(hModule, hResource);
 	if(hGlobalResource == NULL)
 	{
 		return FALSE;
 	}
 
 	//Lock the resource and get the read pointer
 	BYTE* pRecource = (BYTE*)LockResource(hGlobalResource);
 	if(pRecource == NULL)
 	{
 		return FALSE;
 	}
 
 	//Allocate the Stream
 	DWORD dwResult =  CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
 	if(dwResult != S_OK)
 	{
 		FreeResource(hGlobalResource);
 		SetLastError(dwResult);
 		pRecource = NULL;
 		return FALSE;
 	}
 
 	//Copy the resource data to the stream
 	dwResult = m_pStream->Write(pRecource, dwResourceSize, NULL);
 	if(dwResult != S_OK)
 	{
 		FreeResource(hGlobalResource);
 		SAFE_RELEASE(m_pStream);
 		SetLastError(dwResult);
 		return FALSE;		
 	}
 
 	//Tidy up
 	FreeResource(hGlobalResource);
 	
 	//Mark as loaded
 	m_bIsPicLoaded = TRUE;
 
  InvalidateRight(this);

 	return TRUE;
 }

//Overload - Single load function
BOOL CPictureCtrl::Load(CString &szFilePath)
{
	return LoadFromFile(szFilePath);
}

BOOL CPictureCtrl::Load(IStream* piStream)
{
	return LoadFromStream(piStream);
}

BOOL CPictureCtrl::Load(BYTE* pData, size_t nSize)
{
	return LoadFromStream(pData, nSize);
}

void CPictureCtrl::FreeData()
{
	m_bIsPicLoaded = FALSE;
	SAFE_RELEASE(m_pStream);
}

void CPictureCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
  // must need ownredraw
	ModifyStyle(0, SS_OWNERDRAW);
}

void CPictureCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Check if pic data is loaded
	if(m_bIsPicLoaded)
	{
		//Get control measures
		RECT rc;
		this->GetClientRect(&rc);
		Graphics graphics(lpDrawItemStruct->hDC);
		Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right-rc.left), (INT)(rc.bottom-rc.top));	
	}
}

BOOL CPictureCtrl::OnEraseBkgnd(CDC *pDC) {
  return TRUE;
}