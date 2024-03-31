#include <shobjidl.h>

char* GetFile();
char* GetFolder();

char* GetFile(){
	PWSTR pszFilePath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

	COMDLG_FILTERSPEC filter[1] = {{L"Images", L"*.jpg;*.jpeg;*.png"}};

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
		pFileOpen->SetFileTypes(1,filter);
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
	std::wstring ss;
	ss = pszFilePath;
	//(char*)pszFilePath
	char *output = new char[ss.length() + 1];
	output[ss.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0,ss.c_str(), -1, output, (int)ss.length(), NULL, NULL);
    return output;
}

char* GetFolder(){
	PWSTR pszFilePath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

	COMDLG_FILTERSPEC filter[1] = {{L"Images", L"*.jpg;*.jpeg;*.png"}};

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
		//pFileOpen->SetFileTypes(1,filter);
			pFileOpen->SetOptions(FOS_PICKFOLDERS);
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        //MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
	std::wstring ss;
	ss = pszFilePath;
	//(char*)pszFilePath
	char *output = new char[ss.length() + 1];
	output[ss.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0,ss.c_str(), -1, output, (int)ss.length(), NULL, NULL);
    return output;
}