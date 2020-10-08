#pragma once

#include <string>
#include <filesystem>

namespace fibo::FileUtils
{
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

    template<typename T, typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<T>>::value
        || std::is_same<std::wstring, typename std::decay_t<T>>::value
        >
    >
    _NODISCARD decltype(auto) parseFileName(const T& filePath)
    {
        namespace fs = std::filesystem;
        try
        {
            auto pa = fs::path{ filePath };
            FileNameInformation<T> info;
            if constexpr (std::is_same<std::wstring, typename std::decay_t<T>>::value)
            {
                // full path
                info.mFullPath = filePath;
                // root name
                if (pa.has_root_name()) {
                    info.mRootName = pa.root_name().wstring();
                }
                // root directory
                if (pa.has_root_directory()) {
                    info.mRootDirectory = pa.root_directory().wstring();
                }
                // root path
                if (pa.has_root_path()) {
                    info.mRootPath = pa.root_path().wstring();
                }
                // relative path
                if (pa.has_relative_path()) {
                    info.mRelativePath = pa.relative_path().wstring();
                }
                // parent path
                if (pa.has_parent_path()) {
                    info.mParentPath = pa.parent_path().wstring();
                }
                // file name
                if (pa.has_filename()) {
                    info.mFileName = pa.filename().wstring();
                }
                // stem
                if (pa.has_stem()) {
                    info.mStem = pa.stem().wstring();
                }
                // extension
                if (pa.has_extension()) {
                    info.mExtension = pa.extension().wstring();
                }
            }
            else
            {
                // full path
                info.mFullPath = filePath;
                // root name
                if (pa.has_root_name()) {
                    info.mRootName = pa.root_name().string();
                }
                // root directory
                if (pa.has_root_directory()) {
                    info.mRootDirectory = pa.root_directory().string();
                }
                // root path
                if (pa.has_root_path()) {
                    info.mRootPath = pa.root_path().string();
                }
                // relative path
                if (pa.has_relative_path()) {
                    info.mRelativePath = pa.relative_path().string();
                }
                // parent path
                if (pa.has_parent_path()) {
                    info.mParentPath = pa.parent_path().string();
                }
                // file name
                if (pa.has_filename()) {
                    info.mFileName = pa.filename().string();
                }
                // stem
                if (pa.has_stem()) {
                    info.mStem = pa.stem().string();
                }
                // extension
                if (pa.has_extension()) {
                    info.mExtension = pa.extension().string();
                }
            }
            return info;
        }
        catch(...)
        { }
        return FileNameInformation<T>{};
    }
}
