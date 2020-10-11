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
        /**
        * Retrieves the full path and file name of the specified file.
        * This function does not verify that the resulting path and file name are valid, 
        * or that they see an existing file on the associated volume.
        * Path length should be between 3 - 260
        * Version string
        *
        * @exception: runtime_error, length_error
        */
        _NODISCARD static std::string absolutePath(std::string_view inPath);

        /**
        * Retrieves the full path and file name of the specified file.
        * This function does not verify that the resulting path and file name are valid,
        * or that they see an existing file on the associated volume.
        * Path length should be between 3 - 260
        * Version wstring
        *
        * @exception: runtime_error, length_error
        */
        _NODISCARD static std::wstring absolutePath(std::wstring_view inPath);

        /**
        * Retrieves modules of full file path
        * Version string
        *
        * @exception: runtime_error, length_error
        */
        _NODISCARD static FileNameInformation<std::string> parseFileName(std::string_view inPath, unsigned int flag = ParseFlag::All);

        /**
        * Retrieves modules of full file path
        * Version wstring
        *
        * @exception: runtime_error, length_error
        */
        _NODISCARD static FileNameInformation<std::wstring> parseFileName(std::wstring_view inPath, unsigned int flag = ParseFlag::All);
    };
}
