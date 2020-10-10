#pragma once

#include <string>
#include <optional>

namespace fibo
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

    class PathUtils
    {
    public:
        _NODISCARD static std::wstring absolutePath(std::wstring_view inPath);
        _NODISCARD static std::optional<FileNameInformation> parseFileName(std::wstring_view inPath, unsigned int flag = ParseFlag::All);
    };
}
