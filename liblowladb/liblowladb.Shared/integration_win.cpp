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
	static critical_section cs;
	critical_section::scoped_lock lock(cs);

	static utf16string dir;
	if (dir.length() != 0) {
		return dir;
	}

	StorageFolder ^localFolder = ApplicationData::Current->LocalFolder;
	Platform::String ^str = localFolder->Path;
	str += ref new Platform::String(L"\\LowlaDB");
	dir = utf16string((utf16char *)str->Data(), 0, str->Length());

	CreateDirectory(str->Data(), nullptr);
	return dir;
}

std::vector<utf16string> SysListFiles()
{
	std::vector<utf16string> answer;
	return answer;
}

void SysLogMessage(int level, utf16string const &context, utf16string const &message)
{
}

