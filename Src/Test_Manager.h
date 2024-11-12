#ifndef MAIN_PROJECT_TEST_MANAGER_H
#define MAIN_PROJECT_TEST_MANAGER_H
#include "string"
#include "CommandManager.h"
#include "vector"
#include "string"
#include "fstream"
#include "sstream"
#include "Input.h"
#include "UI.h"
#ifndef int
#define int long long
#endif
class Test_Processor{
public:
    static std::vector<std::string> parser(const std::string &line){
        std::stringstream ss(line);
        std::vector<std::string>out;
        std::string tem;
        while (ss >> tem){
            out.push_back(tem);
        }
        return out;
    }
    static bool validate(const std::string& line){
        // there is a bug where it doesnt load test when opening from drive
        std::vector<std::string>token = parser(line);
        std::vector<std::vector<std::string>>commands(1);
        if(token[0] == "help"){
            if(token.size() == 1)
                UI::help("",Command_Manager::command_description,Command_Manager::flag_description,Command_Manager::available_flag);
            else
                UI::help(token[1],Command_Manager::command_description,Command_Manager::flag_description,Command_Manager::available_flag);
            return 0;
        }
        int cnt = 0;
        for(auto e : token){
            if(e.size() > 1 && e.find(',') != std::string::npos)
                return false;
            if(e == ","){
                cnt++;
                commands.emplace_back();
                continue;
            }
            commands[cnt].push_back(e);
        }
        std::set<std::string>pending_variables;
        for(auto command : commands) {
            if(command.empty()) {
                return false;
            }

            std::string type = command[0];
            if (Command_Manager::is_command.find(type) == Command_Manager::is_command.end()) {
                return false;
            }
            if(1 >= command.size()) {
                return false;
            }
            if(type != "for" && (Command_Manager::variable_exist.find(command[1]) != Command_Manager::variable_exist.end()||
                                 pending_variables.find(command[1]) != pending_variables.end())) {
                return false;
            }
            if(type == "for" && !Input::is_int(command[1]) && !Input::is_double(command[1]) && (Command_Manager::variable_exist.find(command[1]) == Command_Manager::variable_exist.end())&&
               (pending_variables.find(command[1]) == pending_variables.end())) {
                return false;
            }
            pending_variables.insert(command[1]);
            for(int i = 2;i<command.size();i++){
                if(Command_Manager::available_flag[type].find(command[i]) == Command_Manager::available_flag[type].end()){
                    return false;
                }
                if(i + Command_Manager::flag_arg_count[command[i]] >= command.size()) {
                    // later add type check
                    return false;
                }
                i += Command_Manager::flag_arg_count[command[i]];
            }
        }
        for(auto e : pending_variables){
            Command_Manager::variable_exist.insert(e);
        }
        return true;
    }
    static void remove_line(const std::string& line){
        std::vector<std::string>token = parser(line);
        std::vector<std::vector<std::string>>commands(1);
        int cnt = 0;
        for(auto e : token){
            if(e == ","){
                cnt++;
                commands.emplace_back();
                continue;
            }
            commands[cnt].push_back(e);
        }
        std::set<std::string>pending_variables;
        for(auto command : commands) {
            if(command[0] == "for")
                continue;
            Command_Manager::variable_exist.erase(command[1]);
        }
    }
    static std::vector<std::vector<std::string>> token_commands_from_line(const std::string& line){
        std::vector<std::string> token = Test_Processor::parser(line);
        std::vector<std::vector<std::string>> commands(1);
        int cnt = 0;
        for (auto e: token) {
            if (e == ",") {
                cnt++;
                commands.emplace_back();
                continue;
            }
            // will add suffix space but its fine
            commands[cnt].push_back(e);
        }
        // will return entire line for a command
        return commands;
    }
    static Command* object_from_command(std::vector<std::string>& command){
        if(command[0] == "int"){
            return new IntCommand(command);
        }
        else if(command[0] == "for"){
            return new ForCommand(command);
        }
        else if(command[0] == "double"){
            return new DoubleCommand(command);
        }
        else if(command[0] == "string"){
            return new StringCommand(command);
        }
        else if(command[0] == "matrix"){
            return new MatrixCommand(command);
        }
        return nullptr;
    }
};
class Tests_Manager{
private:
    std::vector<std::string>lines;
    std::vector<std::vector<Command*>>commands;
    std::string test_file_name = "d";
public:
    void reset_stored_test(){
        Command_Manager::variable_exist.clear();
        lines.clear();
        for(int i = 0;i<commands.size();i++){
            for(int j =0;j<commands[i].size();j++)
                delete commands[i][j];
        }
        commands.clear();
    }
    bool read_test(const std::string& file_name){
        reset_stored_test();
        test_file_name = file_name;
        lines = Input::read_test_script(file_name);
        for(auto line : lines){
            Test_Processor::validate(line);
        }
        if(lines.empty())
            return false;
        for(auto line : lines) {
            std::vector<std::vector<std::string>>command_per_line = Test_Processor::token_commands_from_line(line);
            std::vector<Command*>line_command;
            for(auto command : command_per_line){
                line_command.push_back(Test_Processor::object_from_command(command));
            }
            commands.push_back(line_command);
        }
        return true;
    }
    void create_test(bool reset = true){
        if(reset)
            reset_stored_test();
        UI::test_name_query();
        std::string test_name = Input::read<std::string>();
        if(!reset)
            read_test(test_name);
        int curr = 0;
        while (true) {
            UI::line_query(curr, lines);
            std::string line = Input::read_line();
            if(line == "w"){
                curr = std::max(curr-1,0ll);
            }
            else if(line == "s"){
                curr = std::min(curr+1,(int)lines.size());
            }
            else if(line == "q"){
                reset_stored_test();
                break;
            }
            else if(line == "wq"){
                Tests_Manager::test_file_name = test_name;
                Input::save_test_script(test_name,lines);
                break;
            }
            else if(curr < lines.size()){
                std::string old = lines[curr];
                Test_Processor::remove_line(old);
                if(Test_Processor::validate(line)) {
                    lines[curr] = line;
                }
                else{
                    Test_Processor::validate(old);
                }
            }
            else if (Test_Processor::validate(line)){
                curr++;
                lines.push_back(line);
            }

        }
        reset_stored_test();
        // there is a bug where i can make int n and for n and then remove the n

    }
    void generate_tests(std::ostream& fs = std::cout){
        for(auto level : commands){
            for(auto command : level){
                command->generate(fs);
            }
            fs << '\n';
        }
    }

    void run_tests(){
        if(lines.empty())
            read_test(Tests_Manager::test_file_name);
        UI::sample_test(test_file_name);
        generate_tests();
        std::string ch = Input::read<std::string>();
        if(ch == "d"){
            run_tests();
        }
        if(ch == "s"){
            Input::clear_old_files();
            GENERATED_TESTS_COUNTER = 0;
            UI::generate_test_query();
            int n = Input::read_int();
            for(int i = 0;i<n;i++){
                std::cout << "Generating test " << GENERATED_TESTS_DIR + std::to_string(++GENERATED_TESTS_COUNTER) + ".txt" << '\n';
                std::fstream fs(GENERATED_TESTS_DIR + std::to_string(GENERATED_TESTS_COUNTER) + ".txt",std::ios::trunc | std::ios::out);
                generate_tests(fs);
                fs.close();
            }
            Input::save_generated_test_count();
        }
    }
    bool custom_code_compiled = false;
    void generate_custom_test(std::string output = CUSTOM_TEST_SAMPLE + ".txt"){
        if(!custom_code_compiled) {
            Compare_Outputs::compile_file(CUSTOM_TEST_CODE, CUSTOM_TEST_CODE_EXE);
            custom_code_compiled = true;
        }
        Compare_Outputs::run_code(CUSTOM_TEST_CODE_EXE,"",output);
    }
    void run_custom_test(){
        UI::sample_test("sample");
        generate_custom_test();
        for(auto e : Input::read_file(CUSTOM_TEST_SAMPLE)){
            std::cout << e << '\n';
        }
        std::string ch = Input::read<std::string>();
        if(ch == "d"){
            run_custom_test();
        }
        if(ch == "s"){
            Input::clear_old_files();
            GENERATED_TESTS_COUNTER = 0;
            UI::generate_test_query();
            int n = Input::read_int();
            for(int i = 0;i<n;i++){
                std::cout << "Generating test " << GENERATED_TESTS_DIR + std::to_string(++GENERATED_TESTS_COUNTER) + ".txt" << '\n';
//                std::fstream fs(GENERATED_TESTS_DIR + std::to_string(GENERATED_TESTS_COUNTER) + ".txt",std::ios::trunc | std::ios::out);
                generate_custom_test(GENERATED_TESTS_DIR + std::to_string(GENERATED_TESTS_COUNTER) + ".txt");
            }
            Input::save_generated_test_count();
        }
        custom_code_compiled = false;
    }
};
#endif //MAIN_PROJECT_TEST_MANAGER_H
