#pragma once

#include <string>
#include <filesystem>
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

    template<typename T, typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<T>>::value
        || std::is_same<std::string_view, typename std::decay_t<T>>::value
        || std::is_same<std::wstring, typename std::decay_t<T>>::value
        || std::is_same<std::wstring_view, typename std::decay_t<T>>::value
        >
    >
    _NODISCARD bool isCanonical(const T& sPath)
    {
        auto it = std::find_if(std::cbegin(sPath), std::cend(sPath), [](auto c) {
            return '.' == c;
            });
        return std::cend(sPath) != it;
    }

    _NODISCARD std::wstring absolutePath(std::wstring const& inPath);
    //_NODISCARD std::filesystem::path absolutePath(const std::filesystem::path& inPath);

    _NODISCARD std::optional<FileNameInformation> parseFileName(const std::wstring& sPath, unsigned int flag = ParseFlag::All);
    
}
