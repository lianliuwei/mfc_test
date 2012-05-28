#pragma once

// change for http://www.codeproject.com/Articles/24969/An-MFC-picture-control-to-dynamically-show-picture
// using GDIPlus to draw picture. you need init the GDIPlus lib.
// TODO using double buffer. or check if the parent hwnd lead to flicker.
class CPictureCtrl : public CStatic
{
public:
	CPictureCtrl(void);
	virtual ~CPictureCtrl(void);

public:
	//Loads an image from a file
	BOOL LoadFromFile(CString &szFilePath);

	//Loads an image from an IStream interface
	BOOL LoadFromStream(IStream* piStream);

	//Loads an image from a byte stream;
	BOOL LoadFromStream(BYTE* pData, size_t nSize);

	//Loads an image from a Resource
 	BOOL LoadFromResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);

  // Load From Resource help
  BOOL LoadBitmapResource(UINT res_id) {
    return LoadFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(res_id), RT_BITMAP);
  }
  BOOL LoadPngResource(UINT res_id) {
    return LoadFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(res_id), _T("PNG"));
  }

	//Overload - Single load function
	BOOL Load(CString &szFilePath);
	BOOL Load(IStream* piStream);
	BOOL Load(BYTE* pData, size_t nSize);
 	BOOL Load(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);

  // dude using PNG, the bitmap is suck! the resource edit even create a type
  // for PNG no for gif and jpeg.
  BOOL Load(UINT res_id, LPCTSTR lpType = _T("PNG")) {
    return LoadFromResource(AfxGetResourceHandle(), MAKEINTRESOURCE(res_id), lpType);
  }

	//Frees the image data
	void FreeData();

protected:
	virtual void PreSubclassWindow();

	//Draws the Control
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	BOOL OnEraseBkgnd(CDC* pDC);
  DECLARE_MESSAGE_MAP()

private:

	//Internal image stream buffer
	IStream* m_pStream;

	//Control flag if a pic is loaded
	BOOL m_bIsPicLoaded;
};
