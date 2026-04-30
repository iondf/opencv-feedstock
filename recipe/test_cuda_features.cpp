#include <opencv2/core.hpp>
#include <opencv2/cudafeatures2d.hpp>

#include <initializer_list>
#include <iostream>
#include <string>

namespace {
bool line_has_any_token(const std::string& info, const std::string& label, std::initializer_list<const char*> tokens)
{
    const auto pos = info.find(label);
    if (pos == std::string::npos) {
        std::cerr << "Missing OpenCV build information line: " << label << '\n';
        return false;
    }

    const auto end = info.find('\n', pos);
    const auto line = info.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
    for (const auto* token : tokens) {
        if (line.find(token) != std::string::npos) {
            return true;
        }
    }

    std::cerr << "OpenCV build information line does not contain expected token: " << line << '\n';
    return false;
}

bool line_has_sm120(const std::string& info, const std::string& label)
{
    return line_has_any_token(info, label, {"120", "12.0"});
}
}

int main()
{
    const auto info = cv::getBuildInformation();
    if (!line_has_any_token(info, "NVIDIA CUDA:", {"YES"})) {
        std::cerr << "OpenCV was not built with CUDA support\n";
        return 1;
    }

    if (!line_has_sm120(info, "NVIDIA GPU arch:") || !line_has_sm120(info, "NVIDIA PTX archs:")) {
        std::cerr << "OpenCV CUDA build does not include sm_120 support\n";
        return 1;
    }

    return 0;
}
