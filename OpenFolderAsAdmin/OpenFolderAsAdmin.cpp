#include "stdafx.h"


#include "OpenFolderAsAdmin.h"

#define MAX_LOADSTRING 100


HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

// http://www.flaticon.com/free-icon/delivery-pack-security-symbol-with-a-shield_45926



TCHAR buffer[4096];

UINT_PTR CALLBACK OFNHookProc(
  HWND hdlg,
  UINT uiMsg,
  WPARAM wParam,
  LPARAM lParam
)
{
	switch(uiMsg)
	{
		case WM_INITDIALOG:
		{
			HWND hParent = GetParent(hdlg);
			HWND hOK = GetDlgItem(hParent, IDOK);
			ShowWindow(hOK, SW_HIDE);

			HWND hCancel = GetDlgItem(hParent, IDCANCEL);
			SetWindowText(hCancel, _T("Close"));

			HWND hStaticFilename = GetDlgItem(hParent, 0x442);
			ShowWindow(hStaticFilename, SW_HIDE);

			HWND hStaticFilekind = GetDlgItem(hParent, 0x441);
			ShowWindow(hStaticFilekind, SW_HIDE);

			HWND hComboFilename = GetDlgItem(hParent, 0x47c);
			ShowWindow(hComboFilename, SW_HIDE);

			HWND hComboFilekind = GetDlgItem(hParent, 0x470);
			ShowWindow(hComboFilekind, SW_HIDE);


		}
		break;
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	bool bSL=false;
	LPCTSTR pArgPath = NULL;
	if(__argc > 1)
	{
		if(lstrcmp(__targv[1], L"/secondlaunch")==0)
			bSL = true;
		else
		{
			pArgPath = __targv[1];
			lstrcpy(buffer, pArgPath);
			if(__argc > 2)
			{
				if(lstrcmp(__targv[2], L"/secondlaunch")==0)
					bSL = true;
			}
		}
	}
	else
		pArgPath = _T("C:\\");
	

/* this need clr
	if(!IsAdminDll::IsAdminDll::Is3Admin())
	{
		if(bSL)
		{
			MessageBox(NULL,L"Recursive call",
				L"OpenFolderAsAdmin",
				MB_ICONERROR);
			return -1;
		}

		wstring cmdline = GetCommandLine();
		cmdline += L" /secondlaunch";
		HINSTANCE nShellRet = ShellExecute(NULL, L"runas", cmdline.c_str(), NULL, NULL, nCmdShow);
		return 0;
	}
*/
	//HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced\SeparateProcess

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
		OFN_EXPLORER |
		OFN_ENABLEHOOK;
	
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = 0;
	ofn.lCustData = NULL;
	ofn.lpfnHook = OFNHookProc;
	ofn.lpTemplateName = NULL;

	GetOpenFileName(&ofn);



	return 0;
}



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
