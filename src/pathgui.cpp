#include <filesystem>
#include "wex.h"
#include "inputbox.h"
#include "window2file.h"
#include <gdiplus.h>
#include "cPathFinder.h"

void RunDOT(cPathFinder &finder)
{
    std::ofstream f("g.dot");
    f << finder.pathViz() << "\n";
    f.close();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char cmd[100];
    snprintf(cmd, 99, "dot -Kfdp -n -Tpng -o sample.png g.dot");

    // Retain keyboard focus, minimize module2 window
    si.wShowWindow = SW_SHOWNOACTIVATE | SW_MINIMIZE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEPOSITION;
    si.dwX = 600;
    si.dwY = 200;

    if (!CreateProcess(NULL,               // No module name (use command line)
                       cmd,                // Command line
                       NULL,               // Process handle not inheritable
                       NULL,               // Thread handle not inheritable
                       FALSE,              // Set handle inheritance to FALSE
                       CREATE_NEW_CONSOLE, //  creation flags
                       NULL,               // Use parent's environment block
                       NULL,               // Use parent's starting directory
                       &si,                // Pointer to STARTUPINFO structure
                       &pi)                // Pointer to PROCESS_INFORMATION structure
    )
    {
        int syserrno = GetLastError();
        if (syserrno == 2)
        {
            wex::msgbox mb("Cannot find executable file");
            // NotifyUserOfError(wxString::Format("Cannot find executable file %S in working directory\n",
            //                                    exec_filename));
            return;
        }
        wex::msgbox mb("system error");
        // SetStatusText(wxString::Format("Sysem error no (%d)\n", GetLastError()));
        // wchar_t *lpMsgBuf;
        // FormatMessage(
        //     FORMAT_MESSAGE_ALLOCATE_BUFFER |
        //         FORMAT_MESSAGE_FROM_SYSTEM |
        //         FORMAT_MESSAGE_IGNORE_INSERTS,
        //     NULL,
        //     (DWORD)syserrno,
        //     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        //     (LPWSTR)&lpMsgBuf,
        //     0, NULL);

        // char errorbuf[200];
        // snprintf(errorbuf, 199,
        //          "Error is %S",
        //          lpMsgBuf);
        // LocalFree(lpMsgBuf);

        return;
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    Sleep(1000);
}

int main()
{
    cPathFinder finder;

    // construct top level application window
    wex::gui &form = wex::maker::make();
    form.move({50, 50, 800, 800});
    form.text("Path Finder GUI");

    wex::menubar mbar(form);
    wex::menu mfile(form);
    mfile.append("New", [&](const std::string &title) {
        finder.clear();
        RunDOT(finder);
        form.update();
    });
    mfile.append("Open", [&](const std::string &title) {
        finder.clear();
        wex::filebox fb(form);
        auto fname = fb.open();
        if (fname.empty())
            return;
        finder.read(fname);
        finder.path();
        std::cout << finder.pathText() << "\n";
        RunDOT(finder);
        form.text("Path Finder GUI " + fname );
        form.update();
    });
    mbar.append("File", mfile);

    wex::menu madd(form);
    madd.append("Link", [&](const std::string &title) {
        wex::inputbox ib;
        ib.text("Add link");
        ib.add("src", "");
        ib.add("dst", "");
        ib.add("cost", "1");
        ib.showModal();
        finder.addLink(
            finder.findoradd(ib.value("src")),
            finder.findoradd(ib.value("dst")),
            atoi(ib.value("cost").c_str()));
        RunDOT(finder);
        form.update();
    });

    madd.append("Start, End", [&](const std::string &title) {
        wex::inputbox ib;
        ib.text("identify start, end nodes");
        ib.add("start", "");
        ib.add("end", "");
        ib.showModal();
        finder.start(ib.value("start"));
        finder.end(ib.value("end"));
        finder.path();
        RunDOT(finder);
        form.update();
    });

    mbar.append("Add", madd);

    form.events().draw([&](PAINTSTRUCT &ps) {
        wex::window2file w2f;
        w2f.draw(form, "sample.png");
    });

    std::error_code ec;
    std::filesystem::remove("sample.png", ec );

    // show the application
    form.show();

    //Pass the control of the application to the windows message queue.
    form.run();

    return 0;
}