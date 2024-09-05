// 该代码需要使用C++17或更高版本进行编译。
// This code requires compilation with C++17 or a later version.

// 请确保使用多字节字符集来编译此代码。
// Ensure that the code is compiled with a multi-byte character set (MBCS).

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// 检查是否是管理员权限运行
bool IsRunAsAdmin()
{
    BOOL fIsRunAsAdmin = FALSE;
    PSID pAdministratorsGroup = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup))
    {
        CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin);
        FreeSid(pAdministratorsGroup);
    }

    return fIsRunAsAdmin;
}

// 以管理员权限重新启动程序
void RelaunchAsAdmin()
{
    TCHAR szPath[MAX_PATH];
    if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
    {
        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.lpVerb = TEXT("runas");
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (!ShellExecuteEx(&sei))
        {
            MessageBox(NULL, TEXT("需要管理员权限才能运行此程序。"), TEXT("权限不足"), MB_OK | MB_ICONEXCLAMATION);
            exit(EXIT_FAILURE);
        }
    }
}

// 查找当前目录内所有的exe文件
std::vector<fs::path> FindExeFiles(const fs::path& rootPath)
{
    std::vector<fs::path> exeFiles;
    for (const auto& entry : fs::recursive_directory_iterator(rootPath))
    {
        if (entry.path().extension() == ".exe")
        {
            exeFiles.push_back(entry.path());
        }
    }
    return exeFiles;
}

// 禁止exe文件的防火墙权限
void BlockExeFiles(const std::vector<fs::path>& exeFiles, const fs::path& selfExecutablePath)
{
    for (const auto& exeFile : exeFiles)
    {
        // 检查是否为程序自身
        if (exeFile == selfExecutablePath)
        {
            continue;  // 跳过程序自身
        }

        std::wstring message = L"是否要禁止 " + exeFile.wstring() + L" 的出站和入站通信？";
        int response = MessageBoxW(NULL, message.c_str(), L"确认操作", MB_OKCANCEL | MB_ICONQUESTION);

        if (response == IDCANCEL)
        {
            continue;  // 如果用户选择取消，则跳过当前文件
        }

        // 禁止出站通信
        std::wstring command = L"netsh advfirewall firewall add rule name=\"Block All Out: " + exeFile.wstring() +
            L"\" dir=out program=\"" + exeFile.wstring() +
            L"\" action=block";  // 全局禁止出站

        _wsystem(command.c_str());

        command = L"netsh advfirewall firewall add rule name=\"Block All In: " + exeFile.wstring() +
            L"\" dir=in program=\"" + exeFile.wstring() +
            L"\" action=block";  // 全局禁止入站

        _wsystem(command.c_str());
    }
}

// 打开防火墙的高级设置
void OpenFirewallAdvancedSettings()
{
    ShellExecute(NULL, "open", "wf.msc", NULL, NULL, SW_SHOWNORMAL);
}

int main()
{
    if (!IsRunAsAdmin())
    {
        RelaunchAsAdmin();
        return 0;
    }

    fs::path currentPath = fs::current_path();
    auto exeFiles = FindExeFiles(currentPath);

    // 获取程序自身的可执行文件路径
    TCHAR szPath[MAX_PATH];
    GetModuleFileName(NULL, szPath, MAX_PATH);
    fs::path selfExecutablePath(szPath);

    // 排除了程序自身的可执行文件
    if (exeFiles.size() <= 1)
    {
        MessageBox(NULL, TEXT("目录下未找到任何exe文件。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    if (exeFiles.size() > 10)
    {
        int response = MessageBox(NULL, TEXT("目录下找到大量exe文件，确定要全部禁止联网吗？"), TEXT("警告"), MB_OKCANCEL | MB_ICONWARNING);
        if (response == IDCANCEL)
        {
            return 0;
        }
    }

    BlockExeFiles(exeFiles, selfExecutablePath);

    MessageBox(NULL, TEXT("已禁用目录下指定可执行文件的出入站通信。"), TEXT("完成"), MB_OK | MB_ICONINFORMATION);

    OpenFirewallAdvancedSettings();

    return 0;
}
