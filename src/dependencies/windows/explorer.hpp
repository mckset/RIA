#include <shobjidl.h>

// I hate windows
char* GetFile(){
	PWSTR pszFilePath;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)){
		IFileOpenDialog *pFileOpen;
		COMDLG_FILTERSPEC filter[1] = {{L"Images", L"*.jpg;*.jpeg;*.png"}};

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr)) {
			pFileOpen->SetFileTypes(1,filter);
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr)) {
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr)) {
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
						CoTaskMemFree(pszFilePath);
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	std::wstring ss;
	ss = pszFilePath;
	char *output = new char[ss.length() + 1];
	output[ss.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0,ss.c_str(), -1, output, (int)ss.length(), NULL, NULL);
	return output;
}

char* GetFolder(){
	char* output;
	PWSTR pszFilePath;
	
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileOpenDialog *pFileOpen;
		COMDLG_FILTERSPEC filter[1] = {{L"Images", L"*.jpg;*.jpeg;*.png"}};

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr)) {
			pFileOpen->SetOptions(FOS_PICKFOLDERS);
			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr)) {
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr)) {
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr)) {
						std::wstring ss = pszFilePath;
						output = (char*)malloc((ss.length()+1)*2);
						WideCharToMultiByte(CP_ACP, 0, ss.c_str(), -1, output, (int)(ss.length()+1)*2, NULL, NULL);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}else
				return (char*)"";
			pFileOpen->Release();
		}
		CoUninitialize();
	}    
	if (strlen(output))
		return output;
	return (char*)"";
}
