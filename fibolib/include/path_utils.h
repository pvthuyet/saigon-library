#pragma once

#include <string>
#include <optional>

namespace fibo::PathUtils
{
    enum ParseFlag : unsigned int
    {
        RootName        = 0x0001,
        RootDirectory   = 0x0002,
        RootPath        = 0x0004,
        RelativePath    = 0x0008,
        ParentPath      = 0x0010,
        FileName        = 0x0020,
        Stem            = 0x0040,
        Extension       = 0x0080,
        All             = 0xFFFF
    };

    struct FileNameInformation
    {
        std::wstring mFullPath;
        std::wstring mRootName;
        std::wstring mRootDirectory;
        std::wstring mRootPath;
        std::wstring mRelativePath;
        std::wstring mParentPath;
        std::wstring mFileName;
        std::wstring mStem;
        std::wstring mExtension;
    };

    _NODISCARD std::wstring absolutePath(std::wstring const& inPath);
    _NODISCARD std::optional<FileNameInformation> parseFileName(const std::wstring& sPath, unsigned int flag = ParseFlag::All);
}
