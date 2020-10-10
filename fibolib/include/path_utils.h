#pragma once

#include <string>

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

    template<typename T, typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<T>>::value
        || std::is_same<std::wstring, typename std::decay_t<T>>::value
        >
    >
    struct FileNameInformation
    {
        T mFullPath;
        T mRootName;
        T mRootDirectory;
        T mRootPath;
        T mRelativePath;
        T mParentPath;
        T mFileName;
        T mStem;
        T mExtension;
    };

    class PathUtils
    {
    public:
        _NODISCARD static std::string absolutePath(std::string_view inPath);
        _NODISCARD static std::wstring absolutePath(std::wstring_view inPath);

        _NODISCARD static FileNameInformation<std::string> parseFileName(std::string_view inPath, unsigned int flag = ParseFlag::All);
        _NODISCARD static FileNameInformation<std::wstring> parseFileName(std::wstring_view inPath, unsigned int flag = ParseFlag::All);
    };
}
