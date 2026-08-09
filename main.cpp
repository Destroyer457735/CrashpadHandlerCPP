#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>
#include <string.h>
#include <filesystem>
#ifdef Linux
#include <sys/types.h>
#include <signal.h>
#include <sys/resource.h>
#elif Windows
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <winuser.h>
#endif
using namespace std;
namespace fs = std::filesystem;

bool processIsAlive(int pid)
{
#ifdef Linux

    int result = kill(pid, 0);
    if(result == 0)
        return true;
    else
        return false;

#elif Windows

    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;

#endif
}

int debugInformation(int argc, char *argv[]) 
{
    std::cout << "OS : Linux\n";
    if(argc > 1)
    {
        std::cout << "Arguments are available\n";
        for(int i = 0; i < argc; i++)
        {
            std::cout << argv[i] << "\n";
        }
        return 0;
    } else 
    {
        std::cout << "ABORT: Arguments are not available\n";
        return -1;
    }
}
#ifdef Linux
int main(int argc, char *argv[])
{
    int argumentsAvailable = debugInformation(argc,argv);
    if(argumentsAvailable != 0)
    {
        return argumentsAvailable;
    }
    std::string string = argv[1];
    while(processIsAlive(std::stoi(string)))
    {
        std::chrono::seconds dura(1);
        std::this_thread::sleep_for( dura );
    }
    std::cout << "Dead\n";
    FILE* pipe;

    pipe = popen("zenity --question --icon-name=\"dialog-error\" --text=\"The process has terminated unexpectedly.\\nIt's recommended to enable core dumps.\\nWould you like to do that?\" --title=\"Crashpad Handler Linux (x64)\"","r");
    int status = pclose(pipe);
    std::cout << status << "\n";
    if(status == 0)
    {
        struct rlimit core_limit;
        if (getrlimit(RLIMIT_CORE, &core_limit) == -1) {
            system("zenity --error --text=\"getrlimit function failed\" --title=\"Crashpad Handler Linux (x64)\"");
            return 51;
        }
        core_limit.rlim_cur = RLIM_INFINITY;
        if (setrlimit(RLIMIT_CORE, &core_limit) == -1) {
            system("zenity --error --text=\"setrlimit function failed\" --title=\"Crashpad Handler Linux (x64)\"");
            return 52;
        }
        system("zenity --info --text=\"Core dumps have been enabled successfuly.\\nIt's recommended to reboot your computer for changes to take effect.\" --title=\"Crashpad Handler Linux (x64)\"");
    }
    else
        std::cout << "Not cool\n";
}
#elif Windows

int debugInformationWindows(int argc, char *argv[]) 
{
    if(argc > 2)
    {
        MessageBoxA(nullptr, "Arguments are available", "Debug", MB_OK | MB_ICONINFORMATION);
        for(int i = 0; i < argc; i++)
        {
            MessageBoxA(nullptr, argv[i], "Debug", MB_OK | MB_ICONINFORMATION);
        }
        return 0;
    } else
    {
        MessageBoxA(nullptr, "ABORT: Arguments are not available", "Debug", MB_OK | MB_ICONERROR);
        return -1;
    }
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    int argumentsAvailable = debugInformationWindows(__argc,__argv);
    if(argumentsAvailable = 0)
    {
        return argumentsAvailable;
    }
    std::string string = __argv[1];
    while(processIsAlive(std::stoi(string)))
    {
        std::chrono::seconds dura(1);
        std::this_thread::sleep_for( dura );
    }
    int response = MessageBoxA(nullptr, "The process has terminated unexpectedly.\nIt's recommended to enable core dumps.\nWould you like to do that?", "Crashpad Handler Windows (x64)", MB_YESNO | MB_ICONERROR);
    if(response == IDYES)
    {
        const char *parta = "regedit /s ";
        const char *partb = __argv[2];
        fs::path registryFilePath{ __argv[2] };
        if(fs::exists(registryFilePath))
        {
            char buffer[512];
            strncpy(buffer, parta, sizeof(buffer));
            strncat(buffer, partb, sizeof(buffer));
            int returncode = system(buffer);
            if(returncode != -1)
            {
                MessageBoxA(nullptr, "Core dumps have been enabled successfuly.\nIt's recommended to reboot your computer for changes to take effect.", "Crashpad Handler Windows (x64)", MB_OK | MB_ICONASTERISK);
            }
        } else 
        {
            MessageBoxA(nullptr, "The specified .reg file doesn't exist.", "Crashpad Handler Windows (x64)", MB_OK | MB_ICONERROR);
            return 101;
        }
    }
    return 0;
}
#endif