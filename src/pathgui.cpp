#include <filesystem>
#include <set>
#include "wex.h"
#include "inputbox.h"
#include "window2file.h"
#include <gdiplus.h>
#include "cPathFinder.h"
#include "cPathFinderReader.h"

enum class eOption
{
    costs,
    req,
    sales,
};

void RunDOT(cPathFinder &finder)
{
    auto path = std::filesystem::temp_directory_path();
    auto gdot = path / "g.dot";
    std::ofstream f(gdot);
    f << finder.pathViz() << "\n";
    f.close();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // char cmd[100];
    // snprintf(cmd, 99, "dot -Kfdp -n -Tpng -o sample.png g.dot");

    std::string scmd = "dot -Kfdp -n -Tpng -o ";
    auto sample = path / "sample.png";
    scmd += sample.string() + " " + gdot.string();

    // Retain keyboard focus, minimize module2 window
    si.wShowWindow = SW_SHOWNOACTIVATE | SW_MINIMIZE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEPOSITION;
    si.dwX = 600;
    si.dwY = 200;

    if (!CreateProcess(NULL,                 // No module name (use command line)
                       (char *)scmd.c_str(), // Command line
                       NULL,                 // Process handle not inheritable
                       NULL,                 // Thread handle not inheritable
                       FALSE,                // Set handle inheritance to FALSE
                       CREATE_NEW_CONSOLE,   //  creation flags
                       NULL,                 // Use parent's environment block
                       NULL,                 // Use parent's starting directory
                       &si,                  // Pointer to STARTUPINFO structure
                       &pi)                  // Pointer to PROCESS_INFORMATION structure
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

void doPreReqs(
    cPathFinder &finder,
    cPathFinderReader &reader)
{
    std::set<int> setSkillsNeeded;

    // read input. va are the required skills
    auto va = reader.singleParentTree();

    // starting node
    finder.start("0");

    // paths to all end nodes
    finder.end(-1);

    // run Dijsktra
    finder.path();

    // loop over required skills
    for (auto &a : va)
    {
        // skill 0 does not need to be learned
        if (a == "0")
            continue;

        std::cout << "skill " << a << " needs ";

        //skills needed to get reuired skill
        auto path = finder.pathPick(finder.find(a));

        for (int s : path)
            std::cout << finder.nodeName(s) << " ";
        std::cout << "\n";

        //loop over prerequsites
        for (auto s : path)
        {
            //record skill if not already learned
            setSkillsNeeded.insert(s);
        }
    }

    std::cout << "Total skills needed "
              << setSkillsNeeded.size() << " ( ";
    for (int s : setSkillsNeeded)
        std::cout << finder.nodeName(s) << " ";
    std::cout << " )\n";
}

void ChangeActiveOption(
    wex::menu &mOption,
    eOption option)
{
    mOption.check(0, false);
    mOption.check(1, false);
    mOption.check(2, false);
    mOption.check((int)option);
}

int main()
{
    cPathFinder finder;

    eOption opt = eOption::costs;

    // construct top level application window
    wex::gui &form = wex::maker::make();
    form.move({50, 50, 800, 800});
    form.text("Path Finder GUI");

    wex::panel &graphPanel = wex::maker::make<wex::panel>(form);
    graphPanel.move(0, 50, 800, 750);

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
        cPathFinderReader reader(finder);
        reader.set(fname);
        switch (opt)
        {
        case eOption::costs:
            reader.costs();
            finder.path();
            std::cout << finder.pathText() << "\n";
            break;

        case eOption::req:
            doPreReqs(finder, reader);
            break;

        case eOption::sales:
            reader.sales();
            finder.tsp();
            std::cout << finder.pathText() << "\n";
            break;
        }

        RunDOT(finder);
        form.text("Path Finder GUI " + fname);

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

    wex::menu mOption(form);
    mOption.append("Costed Links", [&](const std::string &title) {
        opt = eOption::costs;
        ChangeActiveOption(mOption, opt);
    });
    mOption.append("Prerequisites", [&](const std::string &title) {
        opt = eOption::req;
        ChangeActiveOption(mOption, opt);
    });
    mOption.append("Sales", [&](const std::string &title) {
        opt = eOption::sales;
        ChangeActiveOption(mOption, opt);
        graphPanel.move(0, 50, 800, 750);
    });
    mOption.check(0);
    mbar.append("Option", mOption);

    form.events().draw([&](PAINTSTRUCT &ps) {
        wex::shapes s(ps);
        switch (opt)
        {
        case eOption::costs:
        case eOption::sales:
            s.text(
                finder.pathText(),
                {5, 5});
            break;
        }
    });

    graphPanel.events().draw([&](PAINTSTRUCT &ps) {
        wex::window2file w2f;
        auto path = std::filesystem::temp_directory_path();
        auto sample = path / "sample.png";
        w2f.draw(graphPanel, sample.string());
    });

    std::error_code ec;
    std::filesystem::remove(
        std::filesystem::temp_directory_path() / "sample.png", ec);

    // show the application
    form.show();

    //Pass the control of the application to the windows message queue.
    form.run();

    return 0;
}