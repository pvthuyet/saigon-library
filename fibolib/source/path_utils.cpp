#include "path_utils.h"
#include "string_utils.h"
#include <filesystem>
#include <fmt/core.h>

#if USE_WINDOWS_API
#include "windows/file_api.h"
using OSAPI	= fibo::WindowsApi::FileApi;
#else
using OSAPI = fibo::Standard;
#endif // _WIN32

namespace fs = std::filesystem;

namespace fibo
{
    template<typename TString,
        typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<TString>>::value
        || std::is_same<std::wstring, typename std::decay_t<TString>>::value
        || std::is_same<std::string_view, typename std::decay_t<TString>>::value
        || std::is_same<std::wstring_view, typename std::decay_t<TString>>::value
        >
    >
    _NODISCARD bool isCanonical(const TString& sPath)
    {
        auto it = std::find_if(std::cbegin(sPath), std::cend(sPath), [](auto c) {
            return '.' == c;
            });
        return std::cend(sPath) != it;
    }

    template<typename TString, typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<TString>>::value
        || std::is_same<std::wstring, typename std::decay_t<TString>>::value>,
        typename U = std::conditional_t<std::is_same<std::string, typename std::decay_t<TString>>::value, std::wstring, std::string>
    >
    decltype(auto) convertFileNameInfo(const FileNameInformation<TString>& input)
    {
        FileNameInformation<U> info;
        // convert wstring => string
        if constexpr (std::is_same<std::wstring, typename std::decay_t<TString>>::value)
        {
            info.mFullPath      = StringUtils::wc2mb(input.mFullPath);
            info.mRootName      = StringUtils::wc2mb(input.mRootName);
            info.mRootDirectory = StringUtils::wc2mb(input.mRootDirectory);
            info.mRootPath      = StringUtils::wc2mb(input.mRootPath);
            info.mRelativePath  = StringUtils::wc2mb(input.mRelativePath);
            info.mParentPath    = StringUtils::wc2mb(input.mParentPath);
            info.mFileName      = StringUtils::wc2mb(input.mFileName);
            info.mStem          = StringUtils::wc2mb(input.mStem);
            info.mExtension     = StringUtils::wc2mb(input.mExtension);
            return info;
        }
        else // convert string => wstring
        {
            info.mFullPath      = StringUtils::mb2wc(input.mFullPath);
            info.mRootName      = StringUtils::mb2wc(input.mRootName);
            info.mRootDirectory = StringUtils::mb2wc(input.mRootDirectory);
            info.mRootPath      = StringUtils::mb2wc(input.mRootPath);
            info.mRelativePath  = StringUtils::mb2wc(input.mRelativePath);
            info.mParentPath    = StringUtils::mb2wc(input.mParentPath);
            info.mFileName      = StringUtils::mb2wc(input.mFileName);
            info.mStem          = StringUtils::mb2wc(input.mStem);
            info.mExtension     = StringUtils::mb2wc(input.mExtension);
            return info;
        }
    }

    std::string PathUtils::absolutePath(std::string_view inPath)
    {
        return inPath.empty() ? std::string{} : StringUtils::wc2mb(absolutePath(StringUtils::mb2wc(inPath)));
    }

    std::wstring PathUtils::absolutePath(std::wstring_view inPath)
    {
        // Empty input path
        if (inPath.empty()) {
            return std::wstring{};
        }

        std::wstring absPath{ inPath };

        fs::path pa{ absPath };
        if (pa.is_relative()) {
            absPath = OSAPI::absolutePath(inPath);
        }

        // absolute path length valid
        if (!OSAPI::validPathLength(absPath.length())) 
        {
            throw std::length_error(fmt::format("Invalid file path length. {}:{}",
                __FILE__,
                __LINE__));
        }

        if (isCanonical(absPath)) {
            absPath = OSAPI::canonicalize(absPath);
        }
        return absPath;
    }

    FileNameInformation<std::string> PathUtils::parseFileName(std::string_view inPath, unsigned int flag)
    {
        // Empty input path
        if (inPath.empty()) {
            return FileNameInformation<std::string>{};
        }

        // Convert to std::string
        return convertFileNameInfo(parseFileName(StringUtils::mb2wc(inPath), flag));
    }

    FileNameInformation<std::wstring> PathUtils::parseFileName(std::wstring_view inPath, unsigned int flag)
    {
        // Empty input path
        if (inPath.empty()) {
            return FileNameInformation<std::wstring>{};
        }

        fs::path pa{ absolutePath(inPath) };
        FileNameInformation<std::wstring> info;

        // full path
        info.mFullPath = pa.native();
        // root name
        if (flag & ParseFlag::RootName) {
            info.mRootName = pa.root_name().native();
        }
        // root directory
        if (flag & ParseFlag::RootDirectory) {
            info.mRootDirectory = pa.root_directory().native();
        }
        // root path
        if (flag & ParseFlag::RootPath) {
            info.mRootPath = pa.root_path().native();
        }
        // relative path
        if (flag & ParseFlag::RelativePath) {
            info.mRelativePath = pa.relative_path().native();
        }
        // parent path
        if (flag & ParseFlag::ParentPath) {
            info.mParentPath = pa.parent_path().native();
        }
        // file name
        if (flag & ParseFlag::FileName) {
            info.mFileName = pa.filename().native();
        }
        // stem
        if (flag & ParseFlag::Stem) {
            info.mStem = pa.stem().native();
        }
        // extension
        if (flag & ParseFlag::Extension) {
            info.mExtension = pa.extension().native();
        }
        
        return info;
    }
}