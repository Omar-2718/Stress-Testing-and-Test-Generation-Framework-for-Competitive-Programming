#include <bits/stdc++.h>
#include <windows.h>
#include "Input.h"
#include "vector"
#include "UI.h"
class Compare_Outputs {
private:
    static std::vector<int>failed_tests;
    const static int time_limit = 2000;
public:
    static bool run_command(const std::string &command) {
        std::cout << command.c_str() << '\n';
        return system(command.c_str()) == 0;
    }

    static bool compile_file(const std::string &file_name, const std::string &output) {
        return run_command(Input::Compiler_path() + " -O2 -D ONLINE_JUDGE " + file_name + " -o " + output);
    }
    static bool run_code_with_timeout(const std::string& command, int time_limit_ms) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "cmd.exe /c \"%s\"", command.c_str());
        std::cout << "Running command: " << cmd << std::endl;

        // Start the child process
        if (!CreateProcess(nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            std::cout << "Failed to start process.\n";
            return false;
        }

        DWORD result = WaitForSingleObject(pi.hProcess, time_limit_ms);

        if (result == WAIT_TIMEOUT) {
            TerminateProcess(pi.hProcess, 1);
            std::cout << "Process terminated. " << GetLastError() << "\n";
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return false;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }

    static bool run_code(const std::string& program_name,const std::string& input,const std::string& output){
        std::string command;
        if(input.empty())
             command = ("\"" + program_name + "\" > \"" + output + "\"");
        else
             command = ("\"" + program_name + "\" < \"" + input + "\" > \"" + output + "\"");
        return run_code_with_timeout( command,Compare_Outputs::time_limit);
    }
    static void compare_tests(int num_of_tests){
        failed_tests.clear();
        for(int i = 1;i<=num_of_tests;i++) {
            if(Input::read_file(CORRECT_CODE_OUTPUT_DIR + std::to_string(i)) != Input::read_file(MY_CODE_OUTPUT_DIR + std::to_string(i))){
                failed_tests.push_back(i);
            }
        }
        UI::compare_result((int)num_of_tests - (int)failed_tests.size(),num_of_tests);
        int curr =0;
        if(!failed_tests.empty()){
            while (true) {
                UI::browse_test(failed_tests[curr],num_of_tests);
                std::string ch = Input::read<std::string>();
                if (ch == "d") {
                    curr = std::min(curr + 1, (int) failed_tests.size() - 1);
                } else if (ch == "a") {
                    curr = std::max(curr - 1, 0ll);
                } else if (ch == "q") {
                    break;
                }
            }
        }
        else{
            curr = 1;
            while (true) {
                UI::browse_test(curr,num_of_tests);
                std::string ch = Input::read<std::string>();
                if (ch == "d") {
                    curr = std::min(curr + 1, (int) num_of_tests);
                } else if (ch == "a") {
                    curr = std::max(curr - 1, 1ll);
                } else if (ch == "q") {
                    break;
                }
            }
        }
    }

    static void run_tests(int num_of_tests){
        compile_file(MY_CODE_DIR,MY_CODE_EXE_DIR);
        compile_file(CORRECT_CODE_DIR,CORRECT_CODE_EXE_DIR);
        int completed = 1;
        for(int i =1;i<=num_of_tests;i++){
            bool ret1 = run_code(MY_CODE_EXE_DIR,GENERATED_TESTS_DIR + std::to_string(i) + ".txt",MY_CODE_OUTPUT_DIR + std::to_string(i) + ".txt");
            bool ret2 = run_code(CORRECT_CODE_EXE_DIR,GENERATED_TESTS_DIR + std::to_string(i) + ".txt",CORRECT_CODE_OUTPUT_DIR + std::to_string(i) + ".txt");
            if(!ret1 || !ret2) {
                UI::time_limit_reached(Compare_Outputs::time_limit);
                break;
            }
            completed++;
        }
        compare_tests(completed-1);
    }
};
std::vector<int> Compare_Outputs::failed_tests;
