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
    T getFilename(const T& filePath)
    {
        namespace fs = std::filesystem;
        try
        {
            if constexpr (std::is_same<std::wstring, typename std::decay_t<T>>::value)
            {
                return fs::path(filePath).filename().wstring();
            }
            else
            {
                return fs::path(filePath).filename().string();
            }
        }
        catch (...)
        {
        }
        return T{};
    }
}
