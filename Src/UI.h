#ifndef MAIN_PROJECT_UI_H
#define MAIN_PROJECT_UI_H
#include <windows.h>
#include "iostream"
#include "vector"
class UI{
public:
    static void clear_screen() {
        system("cls");
    }
    static void generate_test_query(){
        clear_screen();
        std::cout << "Enter number of tests : ";
    }
    static void start_menu(){
        clear_screen();
        std::cout << "1 - Create tests\n";
        std::cout << "2 - Create Custom tests\n";
        std::cout << "3 - Use existing test script\n";
        std::cout << "4 - Edit test script\n";
        std::cout << "5 - Run tests\n";
        std::cout << "6 - Run stress test\n";
        std::cout << "7 - Exit\n";
    }
    static void after_creation(){
        clear_screen();
        std::cout << "1 - Run tests\n";
        std::cout << "2 - Generate new tests\n";
        std::cout << "3 - Exit\n";
    }
    static void couldnt_find_help(){
        std::cout << "Couldnt find Command/Flag\n";
        std::cout << "Press anything to continue\n";
    }
    static void help(const std::string &command,std::map<std::string,std::string>&command_description,const std::map<std::string,std::string>&flag_description,const std::map<std::string,std::set<std::string>>&available_flag){
        std::cout<<'\n';
        if(command.empty()){
                std::cout << "q Quits the program.\n";
                std::cout << "wq Saves and quits the program.\n";
            for(auto cmnd : command_description){
                std::cout << cmnd.first << " - " << cmnd.second << "\n\n";
                if(available_flag.count(cmnd.first) == 0)continue;
                for(auto e : available_flag.at(cmnd.first)){
                    std::cout << e << " - " << flag_description.at(e) << '\n';
                }
                long_divider();
                std::cout << "\n\n";
            }
            Input::read<std::string>();
            return;
        }
        if(command_description.count(command)){
            std::cout << command << " - " << command_description.at(command) << "\n\n";
            for(auto e : available_flag.at(command)){
                std::cout << e << " - " << flag_description.at(e) << '\n';
            }
            Input::read<std::string>();
            return;
        }
        if(flag_description.count(command)){
            std::cout << command << " - " << flag_description.at(command) << '\n';
            Input::read<std::string>();
            return;
        }
        couldnt_find_help();
        Input::read<std::string>();
    }
    static void long_divider(){
        std::cout << "******************************************************\n";
    }
    static void divider(){
        std::cout << "***************************\n";
    }
    static void small_divider(){
        std::cout << "---------\n";
    }
    static void show_test(int num,const std::string& dir){
        std::vector<std::string>lines = Input::read_file(dir + std::to_string(num));
        for(auto e : lines){
            std::cout << e << "\n";
        }
    }
    static void browse_test(int num,int total){
        clear_screen();
        std::cout << "Press D to go forward A to go backward\n";
        std::cout << "Press Q to quit\n";
        divider();
        std::cout << "Test " << num << "/" << total << '\n';
        small_divider();
        for(auto e : Input::read_generated_test(std::to_string(num))){
            std::cout << e << "\n";
        }
        divider();
        std::cout << "Output : \n";
        small_divider();
        show_test(num,MY_CODE_OUTPUT_DIR);
        divider();
        std::cout << "Expected Output : \n";
        small_divider();
        show_test(num,CORRECT_CODE_OUTPUT_DIR);
        divider();
    }
    static void compare_result(int passed,int total){
        clear_screen();
        std::cout << "Passed " << passed << " tests out of " << total << '\n';
        divider();
        std::cout << "Press D to view tests: ";
        std::string ch = Input::read<std::string>();
        while (ch != "d"){
            std::string ch = Input::read<std::string>();
        }
    }
    static void couldntOpenFile(){
        divider();
        std::cout << "Could not open file \n";
    }
    static void sample_test(std::string name = "default"){
        clear_screen();
        if(name == "d")
            name = "default";
        std::cout << "Running sample script from " << name << '\n';
        std::cout << "Press D for next sample\n";
        std::cout << "Press Q to exit\n";
        std::cout << "Press S to start\n";
        divider();

    }
    static void test_name_query(){
        std::cout << "Enter the name of the test or D for unsaved test \n";
    }
    static void line_query(int lvl,const std::vector<std::string>&line = std::vector<std::string>()){
        // int t -r 10 100
        // int n -r 10 200 k -r 100 200
        // for n -r -1e9 1e9 -unique -sum le k
        // for n int ni -r -1e9 1e9 -unique -sum le k
//        system("cls");
        clear_screen();
        std::cout << "Enter help for commands w for going one line up s for going one line down\n";
        divider();
        std::cout << "Line " << lvl+1 << " : ";
        if(line.size() > lvl){
            std::cout << line[lvl] << '\n';
            std::cout << "Line " << lvl+1 << " : ";
        }

    }
    static void time_limit_reached(int time_limit){
        clear_screen();
        std::cout << "Time limit exceeded " << time_limit << " ms\n";
        std::cout << "Type anything : ";
        int ch;std::cin>>ch;
    }
};

#endif //MAIN_PROJECT_UI_H
