#include "braincloud/internal/SaveDataHelper.h"

#if !defined(__APPLE__)

#if defined(WINDOWS) || defined(WIN32)
#include <Windows.h>
#endif


SaveDataHelper * SaveDataHelper::m_instance = NULL;

SaveDataHelper::SaveDataHelper()
: m_savePath("")
{
	
}

SaveDataHelper * SaveDataHelper::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new SaveDataHelper();
	}
	
	return m_instance;
}

void SaveDataHelper::initialize(const char * companyName, const char * gameName)
{
#if defined(WINDOWS) || defined(WIN32)
	if (companyName != NULL && gameName != NULL)
	{
		std::string companyNameStr = companyName;
		std::string gameNameStr = gameName;
		m_savePath = std::string("Software\\" + companyNameStr + "\\" + gameNameStr + "\\");
	}
#endif
}

void SaveDataHelper::saveData(const char * key, const char * data)
{
#if defined(WINDOWS) || defined(WIN32)
#if defined WINAPI_FAMILY && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
	std::string skey = key;
	std::string sdata = data;
	std::wstring wskey;
	std::wstring wsdata;
	for (auto c : skey) {
		wskey += (wchar_t)c;
	}
	for (auto c : sdata) {
		wsdata += (wchar_t)c;
	}
	Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	values->Insert(
		ref new Platform::String(wskey.c_str()), 
		dynamic_cast<Windows::Foundation::PropertyValue^>(Windows::Foundation::PropertyValue::CreateString(ref new Platform::String(wsdata.c_str()))));
#else
	if (m_savePath.empty())
	{
		return;
	}

	LONG status;
	HKEY hKey;

	status = RegOpenKeyExA(HKEY_CURRENT_USER, m_savePath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
	if ((status == ERROR_SUCCESS) && (hKey != NULL)) {
		status = RegSetValueExA(hKey, key, 0, REG_SZ, (BYTE*)data, ((DWORD)strlen(data) + 1)*sizeof(char));
		RegCloseKey(hKey);
	}
	else if (status == ERROR_FILE_NOT_FOUND) {
		// Key didn't exist, create it
		SECURITY_ATTRIBUTES sAttribs = { sizeof(SECURITY_ATTRIBUTES) };
		DWORD dwDisposition = 0;
		status = RegCreateKeyExA(HKEY_CURRENT_USER, m_savePath.c_str(), 0, "", REG_OPTION_VOLATILE, KEY_ALL_ACCESS, &sAttribs, &hKey, &dwDisposition);
		if (status == ERROR_SUCCESS) {
			// Set it's value
			status = RegOpenKeyExA(HKEY_CURRENT_USER, m_savePath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
			if ((status == ERROR_SUCCESS) && (hKey != NULL)) {
				status = RegSetValueExA(hKey, key, 0, REG_SZ, (BYTE*)data, ((DWORD)strlen(data) + 1)*sizeof(char));
				RegCloseKey(hKey);
			}
		}
	}
#endif
#endif
}

std::string SaveDataHelper::readData(const char * key)
{
    std::string sdata;

#if defined(WINDOWS) || defined(WIN32)
#if defined WINAPI_FAMILY && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
	std::string skey = key;
	std::wstring wskey;
	for (auto c : skey) {
		wskey += (wchar_t)c;
	}
	Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	Platform::String^ value = safe_cast<Platform::String^>(localSettings->Values->Lookup(ref new Platform::String(wskey.c_str())));
	if (!value) return "";
	std::wstring wsdata = value->Data();
	for (auto c : wsdata) {
		sdata += (char)c;
	}
#else
	if (m_savePath.empty())
	{
		return "";
	}

	LONG status;
	HKEY hKey;
	status = RegOpenKeyExA(HKEY_CURRENT_USER, m_savePath.c_str(), 0, KEY_READ, &hKey);
	if ((status == ERROR_SUCCESS) && (hKey != NULL)) {
		unsigned long type = REG_SZ, size = 1024;
		char res[1024] = "";

		status = RegQueryValueExA(hKey,
			key, NULL, &type, (LPBYTE)&res[0], &size);

		RegCloseKey(hKey);

		if (status == ERROR_SUCCESS) {
			sdata = res;
		}
	}
#endif
#else
    // android/linux implementation?
#endif

    return sdata;
}

void SaveDataHelper::deleteData(const char * key)
{
#if defined(WINDOWS) || defined(WIN32)
#if defined WINAPI_FAMILY && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP)
	std::string skey = key;
	std::wstring wskey;
	for (auto c : skey) {
		wskey += (wchar_t)c;
	}
	Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	values->Remove(ref new Platform::String(wskey.c_str()));
#else
	if (m_savePath.empty())
	{
		return;
	}

	std::string data;

	LONG status;
	HKEY hKey;
	status = RegOpenKeyExA(HKEY_CURRENT_USER, m_savePath.c_str(), 0, KEY_SET_VALUE, &hKey);
	if ((status == ERROR_SUCCESS) && (hKey != NULL)) {
		unsigned long type = REG_SZ, size = 1024;
		char res[1024] = "";

		status = RegDeleteValueA(hKey, key);
		RegCloseKey(hKey);
	}
#endif
#endif
}

#endif //__APPLE__