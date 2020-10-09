#include "path_utils.h"
#include <filesystem>

#if USE_WINDOWS_API
#include "windows_api.h"
#define OSAPI	fibo::WindowsApi
#else
#define OSAPI	fibo::Standard
#endif // _WIN32

namespace fs = std::filesystem;

namespace fibo::PathUtils
{
    template<typename T, typename = typename std::enable_if_t<
        std::is_same<std::string, typename std::decay_t<T>>::value
        || std::is_same<std::wstring, typename std::decay_t<T>>::value
        || std::is_same<std::string_view, typename std::decay_t<T>>::value
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

    std::wstring absolutePath(std::wstring const& inPath)
    {
        std::wstring absPath{ inPath };

        fs::path pa{ inPath };
        if (pa.is_relative()) {
            absPath = OSAPI::absolutePath(inPath);
        }

        // absolute path length valid
        if (!OSAPI::validPathLength(absPath.length())) {
            return std::wstring{};
        }

        if (isCanonical(absPath)) {
            absPath = OSAPI::canonicalize(absPath);
        }
        return absPath;
    }

    std::optional<FileNameInformation> parseFileName(const std::wstring& sPath, unsigned int flag)
    {
        /*
        namespace fs = std::filesystem;
        try
        {
            auto pa = absolutePath(fs::path{ sPath });
            FileNameInformation info;

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
        }
        catch (...)
        {
        }
        */
        return std::nullopt;
    }
}