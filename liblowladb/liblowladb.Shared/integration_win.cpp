#include <integration.h>

#include <ppltasks.h>
#include <thread>
#include <chrono>

using namespace concurrency;
using namespace Windows::Storage;

void SysSleepMillis(int millis)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

bool SysConfigureFile(const utf16string &file)
{
	return true;
}

utf16string SysNormalizePath(utf16string const &path)
{
	return path;
}

utf16string SysGetDataDirectory()
{
	StorageFolder ^localFolder = ApplicationData::Current->LocalFolder;

	auto t = create_task(localFolder->CreateFolderAsync(L"LowlaDB", CreationCollisionOption::OpenIfExists));
	StorageFolder ^folder = t.get();
	
	Platform::String ^str = folder->Path;
	return utf16string((utf16char *)str->Data(), 0, str->Length());
}

std::vector<utf16string> SysListFiles()
{
	std::vector<utf16string> answer;
	return answer;
}

void SysLogMessage(int level, utf16string const &context, utf16string const &message)
{
}

