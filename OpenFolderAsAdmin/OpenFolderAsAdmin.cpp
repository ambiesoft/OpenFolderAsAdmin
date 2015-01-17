// OpenFolderAsAdmin.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


#include "OpenFolderAsAdmin.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

//
//HRESULT BasicFileOpen()
//{
//	// CoCreate the File Open Dialog object.
//	IFileDialog *pfd = NULL;
//	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, 
//		NULL, 
//		CLSCTX_INPROC_SERVER, 
//		IID_PPV_ARGS(&pfd));
//	if (SUCCEEDED(hr))
//	{
//		// Create an event handling object, and hook it up to the dialog.
//		IFileDialogEvents *pfde = NULL;
//		hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
//		if (SUCCEEDED(hr))
//		{
//			// Hook up the event handler.
//			DWORD dwCookie;
//			hr = pfd->Advise(pfde, &dwCookie);
//			if (SUCCEEDED(hr))
//			{
//				// Set the options on the dialog.
//				DWORD dwFlags;
//
//				// Before setting, always get the options first in order 
//				// not to override existing options.
//				hr = pfd->GetOptions(&dwFlags);
//				if (SUCCEEDED(hr))
//				{
//					// In this case, get shell items only for file system items.
//					hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
//					if (SUCCEEDED(hr))
//					{
//						// Set the file types to display only. 
//						// Notice that this is a 1-based array.
//						hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
//						if (SUCCEEDED(hr))
//						{
//							// Set the selected file type index to Word Docs for this example.
//							hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
//							if (SUCCEEDED(hr))
//							{
//								// Set the default extension to be ".doc" file.
//								hr = pfd->SetDefaultExtension(L"doc;docx");
//								if (SUCCEEDED(hr))
//								{
//									// Show the dialog
//									hr = pfd->Show(NULL);
//									if (SUCCEEDED(hr))
//									{
//										// Obtain the result once the user clicks 
//										// the 'Open' button.
//										// The result is an IShellItem object.
//										IShellItem *psiResult;
//										hr = pfd->GetResult(&psiResult);
//										if (SUCCEEDED(hr))
//										{
//											// We are just going to print out the 
//											// name of the file for sample sake.
//											PWSTR pszFilePath = NULL;
//											hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, 
//												&pszFilePath);
//											if (SUCCEEDED(hr))
//											{
//												TaskDialog(NULL,
//													NULL,
//													L"CommonFileDialogApp",
//													pszFilePath,
//													NULL,
//													TDCBF_OK_BUTTON,
//													TD_INFORMATION_ICON,
//													NULL);
//												CoTaskMemFree(pszFilePath);
//											}
//											psiResult->Release();
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//				// Unhook the event handler.
//				pfd->Unadvise(dwCookie);
//			}
//			pfde->Release();
//		}
//		pfd->Release();
//	}
//	return hr;
//}


TCHAR buffer[4096];
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LPCTSTR pArgPath = NULL;
	if(__argc > 1)
	{
		pArgPath = __targv[1];
		lstrcpy(buffer, pArgPath);
	}
	else
		pArgPath = _T("C:\\");
	
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = NULL;
	ofn.lpstrCustomFilter = NULL;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = sizeof(buffer)/sizeof(buffer[0]);
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = pArgPath;
	ofn.lpstrTitle = _T("OpenFolderAsAdmin");
	ofn.Flags = OFN_ALLOWMULTISELECT | 
		OFN_ENABLESIZING | 
		OFN_FORCESHOWHIDDEN |
		OFN_HIDEREADONLY |
		OFN_EXPLORER;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = 0;
	ofn.lCustData = NULL;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;

	GetOpenFileName(&ofn);



	return 0;
}
