#include "braincloud/internal/android/AndroidSaveDataHelper.h"

#import "SSKeychain/SSKeychain.h"
#import "SSKeychain/SSKeychainQuery.h"

namespace BrainCloud
{
	SaveDataHelper* SaveDataHelper::create()
	{
		return new AndroidSaveDataHelper();
	}

	AndroidSaveDataHelper::AndroidSaveDataHelper()
	{
	}

	void AndroidSaveDataHelper::initialize(const char* companyName, const char* appName, const char* wrapperName)
	{
		if (companyName != NULL && appName != NULL)
		{
			std::string companyNameStr = companyName;
			std::string gameNameStr = appName;

			std::string prefix = wrapperName == NULL ? "" : wrapperName;
			if (strlen(prefix.c_str()) > 0) {
				prefix += ".";
			}

			m_savePath = prefix + companyNameStr + "." + gameNameStr;
		}
	}

	void AndroidSaveDataHelper::saveData(const char* key, const char* data)
	{
		if (m_savePath.empty())
			return;

		NSString* serviceString = [NSString stringWithUTF8String : m_savePath.c_str()];
		NSString* keyString = [NSString stringWithUTF8String : key];
		NSString* dataString = [NSString stringWithUTF8String : data];

		[SSKeychain deletePasswordForService : serviceString account : keyString] ;

		if (dataString.length == 0)
		{
			[[NSUserDefaults standardUserDefaults]removeObjectForKey:keyString];
		}
		else
		{
			[[NSUserDefaults standardUserDefaults]setValue:dataString forKeyPath : keyString];
		}
	}

	std::string AndroidSaveDataHelper::readData(const char* key)
	{
		if (m_savePath.empty())
			return "";

		NSString* serviceString = [NSString stringWithUTF8String : m_savePath.c_str()];
		NSString* keyString = [NSString stringWithUTF8String : key];

		NSString* dataString = [SSKeychain passwordForService : serviceString account : keyString];

		std::string data = "";

		if (dataString != nil)
		{
			[[NSUserDefaults standardUserDefaults]setValue:dataString forKeyPath : keyString];
			[SSKeychain deletePasswordForService : serviceString account : keyString] ;
			data = [dataString UTF8String];
		}

		return data;
	}

	void AndroidSaveDataHelper::deleteData(const char* key)
	{
		if (m_savePath.empty())
			return;

		NSString* serviceString = [NSString stringWithUTF8String : m_savePath.c_str()];
		NSString* keyString = [NSString stringWithUTF8String : key];

		[SSKeychain deletePasswordForService : serviceString account : keyString] ;
		[[NSUserDefaults standardUserDefaults]removeObjectForKey:keyString];
	}
}
