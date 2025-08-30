#pragma once
#include <string>
#include <locale>
#include <codecvt>

inline std::string wstr2str(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(wstr);
}
static std::vector<std::wstring> split_ws(const std::wstring& s) {
    std::wstringstream ss(s);
    std::vector<std::wstring> v;
    std::wstring t;
    while (ss >> t) v.push_back(t);
    return v;
}
inline std::wstring str2wstr(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}
static bool parseIntWS(const std::wstring& w, int& out) {
    if (w.empty()) return false;
    try {
        size_t idx = 0;
        int v = std::stoi(w, &idx);
        if (idx != w.size()) return false;
        out = v;
        return true;
    }
    catch (...) {
        return false;
    }

}

static inline std::wstring trim(const std::wstring& s) {
    size_t a = s.find_first_not_of(L" \t\r\n");
    if (a == std::wstring::npos) return L"";
    size_t b = s.find_last_not_of(L" \t\r\n");
    return s.substr(a, b - a + 1);
}

static inline bool wtoi(const std::wstring& ws, int& out) {
    try {
        std::size_t idx = 0;
        int v = std::stoi(ws, &idx, 10);
        if (idx != ws.size()) return false;
        out = v;
        return true;
    }
    catch (...) { return false; }
}

static inline bool wtoi_u32(const std::wstring& ws, unsigned int& out) {
    try {
        std::size_t idx = 0;
        long long v = std::stoll(ws, &idx, 10);
        if (idx != ws.size() || v < 0 || v > 0xFFFFFFFFLL) return false;
        out = static_cast<unsigned int>(v);
        return true;
    }
    catch (...) { return false; }
}
// 工具函数
inline std::string wstring_to_string(const std::wstring& wstr) {
    return std::string(wstr.begin(), wstr.end());
}
inline std::string w2u8(const std::wstring& ws) {
    if (ws.empty()) return {};
    int nbytes = ::WideCharToMultiByte(
        CP_UTF8, 0,
        ws.c_str(), static_cast<int>(ws.size()),
        nullptr, 0,
        nullptr, nullptr
    );
    if (nbytes <= 0) return {};

    std::string out;
    out.resize(nbytes); // 分配空间

    ::WideCharToMultiByte(
        CP_UTF8, 0,
        ws.c_str(), static_cast<int>(ws.size()),
        &out[0], nbytes,   // ★ 用 &out[0]
        nullptr, nullptr
    );
    return out;
}

// UTF-8 -> 宽字（读文件需要时）
inline std::wstring u8w(const std::string& s) {
    if (s.empty()) return {};

    int nchars = ::MultiByteToWideChar(
        CP_UTF8, 0,
        s.c_str(), static_cast<int>(s.size()),
        nullptr, 0
    );
    if (nchars <= 0) return {};

    std::wstring out;
    out.resize(nchars);   // 分配空间

    ::MultiByteToWideChar(
        CP_UTF8, 0,
        s.c_str(), static_cast<int>(s.size()),
        &out[0], nchars   // ★ 传 &out[0] 而不是 out.data()
    );

    return out;
}