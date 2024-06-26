void GetBoard();
void OpenShared();
void SaveBoard();

void GetBoard(){
	PWSTR pszFilePath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

	COMDLG_FILTERSPEC filter[1] = {{L"Board", L"*.brd"}};

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
                        std::wstring ss = pszFilePath;
                        char *output = new char[ss.length() + 1];
                        output[ss.size()] = '\0';
                        WideCharToMultiByte(CP_ACP, 0,ss.c_str(), -1, output, (int)ss.length(), NULL, NULL);
                        board = GetName(output);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
}

void SaveBoard(){
	PWSTR pszFilePath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog *pFileSave;

	COMDLG_FILTERSPEC filter[1] = {{L"Board", L"*.brd"}};

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, 
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

        if (SUCCEEDED(hr))
        {
		pFileSave->SetFileTypes(1,filter);
            // Show the Open dialog box.
            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        std::wstring ss = pszFilePath;
                        char *output = new char[ss.length() + 1];
                        output[ss.size()] = '\0';
                        WideCharToMultiByte(CP_ACP, 0,ss.c_str(), -1, output, (int)ss.length(), NULL, NULL);
                        board = GetName(output);
                        if (board.length() >= 4){
                            if (board.substr(board.length()-4) != ".brd")
                                board += ".brd";
                        }else{
                            board += ".brd";
                        }
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileSave->Release();
        }
        CoUninitialize();
    }
}