#include "path_utils.h"
#include <algorithm>
#include "platform_api.h"

namespace fs = std::filesystem;

namespace fibo::PathUtils
{
    std::wstring absolutePath(std::wstring const& inPath)
    {
        std::wstring abPath{ inPath };

        fs::path pa{ inPath };
        if (pa.is_relative()) {
            abPath = PlatformAPI::absolutePath(inPath);
        }

        // absolute path length valid
        if (!PlatformAPI::validPathLength(abPath.length())) {
            return std::wstring{};
        }

        if (isCanonical(abPath)) {
            abPath = PlatformAPI::canonicalize(abPath);
        }
        return abPath;
    }

    //std::filesystem::path absolutePath(const std::filesystem::path& inPath)
    //{
    //    fs::path pa{ inPath };
    //    if (pa.is_relative()) {
    //        pa = fs::absolute(pa);
    //    }

    //    if (isCanonical(pa.native())) {
    //        pa = fs::canonical(pa);
    //    }
    //    return pa;
    //}

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