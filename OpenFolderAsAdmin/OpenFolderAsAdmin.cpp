#include "stdafx.h"

#include "OpenFolderAsAdmin.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

// http://www.flaticon.com/free-icon/delivery-pack-security-symbol-with-a-shield_45926





BOOL CALLBACK myEnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buffer[1024];buffer[0]=0;
	GetClassName(hwnd, buffer, countof(buffer));
	if(lstrcmp(buffer, L"SysListView32")==0)
	{
		buffer[0]=0;
		GetWindowText(hwnd, buffer, countof(buffer));
		if(lstrcmp(buffer, L"FolderView")==0)
		{
			*(HWND*)lParam=hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

UINT_PTR CALLBACK OFNHookProc(
  HWND hdlg,
  UINT uiMsg,
  WPARAM wParam,
  LPARAM lParam
)
{
	static TCHAR buffer[1024];
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
			//ShowWindow(hComboFilename, SW_HIDE);

			HWND hComboFilekind = GetDlgItem(hParent, 0x470);
			EnableWindow(hComboFilekind, FALSE);



			// PostMessage(hdlg, WM_APP_AFTERINIT, 0,0);

		}
		break;

		case WM_APP_AFTERINIT:
		{
			HWND hFolder = NULL;
			EnumChildWindows(GetParent(hdlg),myEnumChildProc, (LPARAM)&hFolder);
			int count =ListView_GetItemCount(hFolder);
			static int lastcount;
			if(count==0 || count != lastcount)
			{
				PostMessage(hdlg, WM_APP_AFTERINIT, 0,0);
			}
			else
			{
				HWND hComboFilename = GetDlgItem(GetParent(hdlg), 0x47c);
				buffer[0]=0;
				GetWindowText(hComboFilename,buffer,countof(buffer));
				if(buffer[0])
				{
					wstring filename=buffer;
					int index=-1;
					for(int i=0 ; i < count ; ++i)
					{
						buffer[0]=0;
						ListView_GetItemText(hFolder,i,0,buffer,countof(buffer));
						if(lstrcmpi(filename.c_str(),buffer)==0)
						{
							index=i;
							break;
						}
					}
					if(index>=0)
					{
						ListView_SetItemState(hFolder, index, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
						ListView_EnsureVisible(hFolder, index, FALSE);
					}
				}
			}
			lastcount = count;
		}
		break;

		case WM_NOTIFY:
		{
			NMHDR* pNotify = (NMHDR*)lParam;
			switch(pNotify->code)
			{
				case CDN_FILEOK:
				{
					SetWindowLong(hdlg, DWL_MSGRESULT, 1);
					return 1;
				}
				break;
				
				case CDN_INITDONE:
				{
					PostMessage(hdlg, WM_APP_AFTERINIT, 0,0);
					//HWND hFolder = NULL;
					//EnumChildWindows(GetParent(hdlg),myEnumChildProc, (LPARAM)&hFolder);
					//ListView_SetItemState(hFolder, 1, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				}
				break;
			}
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
	TCHAR szArg[MAX_PATH] = {0};
	TCHAR* buffer = (TCHAR*)malloc(4096);
	STLSCOPEDFREE(buffer);
	buffer[0]=0;
	if(__argc > 1)
	{
		if(lstrcmp(__targv[1], L"/secondlaunch")==0)
			bSL = true;
		else
		{
			lstrcpy(szArg,__targv[1]);
			lstrcpy(buffer, __targv[1]);
			PathRemoveBackslash(buffer);
			if(__argc > 2)
			{
				if(lstrcmp(__targv[2], L"/secondlaunch")==0)
					bSL = true;
			}
		}
	}
	else
	{
		lstrcpy(szArg,__T("C:\\"));
	}


	PathRemoveBackslash(szArg);

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
	// HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced\SeparateProcess
	
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
	ofn.lpstrInitialDir = szArg;
	ofn.lpstrTitle = _T("OpenFolderAsAdmin");
	ofn.Flags = 
		OFN_ALLOWMULTISELECT | 
		OFN_ENABLESIZING | 
		OFN_FORCESHOWHIDDEN |
		OFN_HIDEREADONLY |
		OFN_EXPLORER |
		OFN_FORCESHOWHIDDEN |
		OFN_ENABLEHOOK;
	
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = 0;
	ofn.lCustData = NULL;
	ofn.lpfnHook = OFNHookProc;
	ofn.lpTemplateName = NULL;

	if(!GetOpenFileName(&ofn))
	{
		DWORD dwError = CommDlgExtendedError();
		if(dwError==CDERR_DIALOGFAILURE)
		{
			return 0;
		}
	}



	WaitWindowClose();
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
