#ifndef MAIN_PROJECT_COMMANDMANAGER_H
#define MAIN_PROJECT_COMMANDMANAGER_H
#include "string"
#include "map"
#include "set"
#include "vector"
#include "fstream"
#include "Input.h"
#include "Random.h"
class Command_Manager{
public:

    static std::map<std::string,bool>is_command;
    static std::map<std::string,std::set<std::string>>available_flag;
    static std::map<std::string,int>flag_arg_count;
    static std::set<std::string>variable_exist;
    static std::map<std::string,std::string>variables;
    static std::map<std::string,std::string>command_description;
    static std::map<std::string,std::string>flag_description;

    static void init() {
        Command_Manager::flag_arg_count = {
                {"-r",2},
                {"-unique",0},
                {"-d",0},
                {"-num",0},
                {"-b",0},
                {"-l",1},
                {"-n",1},
                {"-m",1},
                {"-p",0},
                {"-<=",1},
                {"->=",1},
                {"-dpth",1}

        };
        Command_Manager::flag_description = {
                {"-r", "Specifies a range for generated values. Requires two arguments: lower and upper bounds."},
                {"-unique", "Ensures that generated values are unique."},
                {"-d", "Indicates that the values generated should be of double (floating-point) type."},
                {"-num", "Indicates that the generated string should be an integer"},
                {"-b", "Indicates that the generated string should contain only binary characters (0 or 1)."},
                {"-l", "Specifies the length of the generated string. Requires one argument."},
                {"-n", "Specifies the number of rows in the generated matrix. Requires one argument."},
                {"-m", "Specifies the number of columns in the generated matrix. Requires one argument."},
                {"-p", "Generates a random permutation of values within the specified length."},
                {"-<=", "Specifies an upper bound for the sum of generated values. Requires one argument."},
                {"->=", "Specifies a lower bound for the sum of generated values. Requires one argument."},
                {"-dpth", "Sets the depth for recursive generation processes. Requires one argument."}
        };

        Command_Manager::is_command = {{"int",    1},
                                       {"double", 1},
                                       {"for",    1},
                                       {"string", 1},
                                       {"matrix",1}};
        Command_Manager::command_description = {
                {"int", "Generates a random integer within the specified range."},
                {"double", "Generates a random double within the specified range."},
                {"string", "Generates a random string. Can be numeric, binary, or alphabetic."},
                {"matrix", "Generates a matrix of random integers with specified dimensions and value range."},
                {"for", "Executes a sequence of random values with specific properties such as range, permutation, and sum constraints."},
        };
        Command_Manager::available_flag = {
                {"int",{"-r"}},
                {"double",{"-r"}},
                {"string",{"-l","-num","-b"}},
                {"for",{"-r","-unique","-d","-p","->=","-<=","-dpth"}},
                {"matrix",{"-n","-m","-r"}}
        };
        Command_Manager::variable_exist = {};
    }

};
class Command{
protected:
    std::string command_name;
    std::string var_name;
    std::set<std::string>flags;
    std::map<std::string,std::vector<std::string>>flag_args;
public:
    explicit Command(std::vector<std::string>str){
        command_name = str[0];
        var_name = str[1];
        int i =2;
        while (i < str.size()) {
            flags.insert(str[i]);
            int args_count = Command_Manager::flag_arg_count[str[i]];
            for(int j = i+1;j<=i+args_count;j++)
                flag_args[str[i]].push_back(str[j]);
            i += 1 + args_count;
        }
    }
    virtual void set_var_name(const std::string &_var_name){
        var_name = _var_name;
    }
    std::string get_var_name(){
        return var_name;
    }
    virtual void generate(std::ostream& fs){};
    virtual void set_command(){
    }
    virtual ~Command() = default;
};
std::map<std::string, bool> Command_Manager::is_command;
std::map<std::string, std::set<std::string>> Command_Manager::available_flag;
std::map<std::string, int> Command_Manager::flag_arg_count;
std::set<std::string> Command_Manager::variable_exist;
std::map<std::string,std::string>Command_Manager::variables;
std::map<std::string,std::string>Command_Manager::command_description;
std::map<std::string,std::string>Command_Manager::flag_description;

class IntCommand : public Command{
private:
    int l=0,r=0;
public:
    IntCommand(const std::vector<std::string>&str): Command(str){
    }

    void generate(std::ostream& fs)override{
        for(auto e : flags){
            if(e == "-r"){
                l = Input::flag_to_int(flag_args["-r"][0],Command_Manager::variables);
                r = Input::flag_to_int(flag_args["-r"][1],Command_Manager::variables);
            }
        }
        Command_Manager::variables[var_name] = std::to_string(Random::random_int(l,r));
        fs << Command_Manager::variables[var_name] << " ";
    }
};
class DoubleCommand : public Command{
private:
    double l=0,r=0;
public:
    DoubleCommand(const std::vector<std::string>&str): Command(str){
    }

    void generate(std::ostream& fs)override{
        for(auto e : flags){
            if(e == "-r"){
                l = Input::flag_to_double(flag_args["-r"][0],Command_Manager::variables);
                r = Input::flag_to_double(flag_args["-r"][1],Command_Manager::variables);
            }
        }
        Command_Manager::variables[var_name] = std::to_string(Random::random_double(l,r));
        fs << Command_Manager::variables[var_name] << " ";
    }
};
class StringCommand : public Command{
private:
    int length;
    bool is_num = false;
    bool is_binary = false;
public:
    StringCommand(const std::vector<std::string>&str): Command(str){
    }
    void generate(std::ostream& fs)override{
        for(auto e : flags){
            if(e == "-l"){
                length = Input::flag_to_int(flag_args["-l"][0],Command_Manager::variables);
            }
            if(e == "-num"){
                is_num = true;
            }
            if(e == "-b"){
                is_binary = true;
            }
        }
        if(is_num)
            Command_Manager::variables[var_name] = Random::random_string_of_numbers(length,true);
        else if(is_binary)
            Command_Manager::variables[var_name] = Random::random_binary_string(length);
        else
            Command_Manager::variables[var_name] = Random::random_string_letters(length);
        fs << Command_Manager::variables[var_name] << " ";
    }
};
class MatrixCommand : public Command{
private:
    int rows,columns,l,r;
public:
    MatrixCommand(const std::vector<std::string>&str): Command(str){
    }
    void generate(std::ostream& fs)override{
        for(auto e : flags){
            if(e == "-n"){
                rows = Input::flag_to_int(flag_args["-n"][0],Command_Manager::variables);
            }
            else if(e == "-m"){
                columns = Input::flag_to_int(flag_args["-m"][0],Command_Manager::variables);
            }
            else if(e == "-r"){
                l = Input::flag_to_int(flag_args["-r"][0],Command_Manager::variables);
                r = Input::flag_to_int(flag_args["-r"][1],Command_Manager::variables);
            }
        }
        std::vector<std::vector<int>>matrix = Random::random_matrix(rows,columns,l,r);
        for(int i = 0;i<rows;i++){
            for(int j=0;j<columns;j++){
                fs << matrix[i][j] << " ";
            }
            fs << "\n";
        }
    }
};
class ForCommand : public Command{
private:
    // todo handle huge numbers
    double l,r;
    double sum_l = INT_MIN,sum_r = INT_MAX;
    int length;
    int depth = 10;
    bool numbers_are_double = false;
    bool perm = false;
    bool sum_condition = false;
public:
    ForCommand(const std::vector<std::string>&str): Command(str){
    }
    void generate(std::ostream& fs)override{

        for(auto e : flags){
            if(e == "-r"){
                l = Input::flag_to_double(flag_args["-r"][0],Command_Manager::variables);
                r = Input::flag_to_double(flag_args["-r"][1],Command_Manager::variables);
            }
            if(e == "-d"){
                numbers_are_double = true;
            }
            if(e == "-p"){
                perm = true;
            }
            if(e == "-<="){
                sum_condition = 1;
                sum_r = Input::flag_to_double(flag_args["-<="][0],Command_Manager::variables);
            }
            if(e == "->="){
                sum_condition = 1;
                sum_l = Input::flag_to_double(flag_args["->="][0],Command_Manager::variables);
            }
            if(e == "-dpth"){
                depth = Input::flag_to_int(flag_args["-dpth"][0],Command_Manager::variables);
            }
        }
        length = Input::flag_to_int(var_name,Command_Manager::variables);
        if(perm){
            for(auto e : Random::random_permutation(length)){
                fs << e << " ";
            }
            return;
        }
        if(sum_condition){
            if(numbers_are_double){
                for(auto e : Random::better_random_numbers_sum_range_double(sum_l,sum_r,l,r,length,depth))
                    fs << e << " ";
                return;
            }
            for(auto e : Random::better_random_numbers_sum_range(sum_l,sum_r,l,r,length,depth))
                fs << e << " ";
            return;
        }
        for(int i = 0;i<length;i++){
            if(numbers_are_double){
                fs << Random::random_double(l,r) << " ";
            }
            else {
                fs << Random::random_int(l, r) << " ";
            }
        }
    }
};
#endif //MAIN_PROJECT_COMMANDMANAGER_H
