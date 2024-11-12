#ifndef MAIN_PROJECT_INPUT_H
#define MAIN_PROJECT_INPUT_H
#include "iostream"
#include "limits"
#include "string"
#include "fstream"
#include "filesystem"
#include "vector"
#ifndef int
#define int long long
#endif

const std::string COMPILER_PATH = "C:/mingw64/bin/g++.exe";
const std::string CURRENT_DIR = "..";

const std::string GENERATED_TESTS_SCRIPTS_DIR = CURRENT_DIR + "/Test_Scripts/";
const std::string GENERATED_TESTS_DIR = CURRENT_DIR + "/Generated_Tests/";

const std::string MY_CODE_DIR = CURRENT_DIR + "/MyCode.cpp";
const std::string MY_CODE_EXE_DIR = CURRENT_DIR + "/bin/MyCode.exe";
const std::string MY_CODE_OUTPUT_DIR = CURRENT_DIR + "/My_Code_Output/";

const std::string CORRECT_CODE_DIR = CURRENT_DIR + "/CorrectCode.cpp";
const std::string CORRECT_CODE_EXE_DIR =CURRENT_DIR + "/bin/CorrectCode.exe";
const std::string CORRECT_CODE_OUTPUT_DIR = CURRENT_DIR + "/Correct_Code_Output/";

const std::string CUSTOM_TEST_SAMPLE = CURRENT_DIR + "/Custom_Test/sample";
const std::string CUSTOM_TEST_CODE = CURRENT_DIR + "/Custom_Test/CustomTest.cpp";
const std::string CUSTOM_TEST_CODE_EXE =  CURRENT_DIR + "/Custom_Test/CustomTest.exe";


static int GENERATED_TESTS_COUNTER = 1;
class Input{
public:
    static std::string Compiler_path(){return COMPILER_PATH;}
    static int read_int(){
        int num;std::cin>>num;
        while (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input please input an Int or 0 to terminate. \n";
            std::cin>>num;
        }
        return num;
    }
    template<class tp>static tp read(){
        tp num;std::cin>>num;
        while (std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input please input an Int or 0 to terminate. \n";
            std::cin>>num;
        }
        return num;
    }
    static std::string read_line(){
        if(std::cin.peek() == '\n'){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        std::string str;
        std::getline(std::cin,str);
        return str;
    }
    static void write_to_file(const std::string& dir,const std::vector<std::string>&lines){
        std::ofstream fs(dir + ".txt",std::ios::trunc | std::ios::out);
        // handle file not found
        for(auto e : lines){
            fs << e << "\n";
        }
    }
    static std::vector<std::string> read_file(const std::string& dir){
        std::ifstream fs( dir + ".txt");
        std::string tem;
        std::vector<std::string>lines;
        while (std::getline(fs, tem)) {
            lines.push_back(tem);
        }
        fs.close();
        return lines;
    }
    static std::vector<std::string> read_test_script(const std::string& name = "default"){
        //handel file not found
        if(name == "d")
            return read_file(GENERATED_TESTS_SCRIPTS_DIR + "default");
        return read_file(GENERATED_TESTS_SCRIPTS_DIR + name);

    }
    static std::vector<std::string> read_generated_test(const std::string& name = "default"){
        return read_file(GENERATED_TESTS_DIR + name);

    }
    static void save_test_script(std::string& name,const std::vector<std::string>&lines){
        if(name == "d")
            name = "default";
        write_to_file(GENERATED_TESTS_SCRIPTS_DIR + name,lines);
    }
    static void save_generated_test_count(){
        std::vector<std::string>lines = {std::to_string(GENERATED_TESTS_COUNTER)};
        write_to_file(GENERATED_TESTS_DIR + "Test_Count",lines);
    }
    static int read_test_count(){
        return std::stoi(read_file(GENERATED_TESTS_DIR + "Test_Count")[0]);
    }

    static void delete_txt_files_in_dir(const std::string &dir){
        for(const auto& entry : std::filesystem::directory_iterator(dir)){
            const std::string file_path = entry.path().string();
            if(entry.is_regular_file() && entry.path().extension() == ".txt"){
                try {
                    std::filesystem::remove(entry.path());
                }
                catch (const std::filesystem::filesystem_error& e){
                    std::cout << "Error deleting old test " << e.what() << '\n';
                }
            }
        }
    }
    static void clear_old_files(){
        delete_txt_files_in_dir(GENERATED_TESTS_DIR);
        delete_txt_files_in_dir(MY_CODE_OUTPUT_DIR);
        delete_txt_files_in_dir(CORRECT_CODE_OUTPUT_DIR);
    }
    static bool is_int(const std::string &str){
        try{
            size_t pos;
            std::stoll(str,&pos);
            return pos == str.size();
        }
        catch(std::invalid_argument&){
            return false;
        }
    }
    static bool is_double(const std::string &str){
        try{
            size_t pos;
            std::stod(str,&pos);
            return pos == str.size();
        }
        catch(std::invalid_argument&){
            return false;
        }
    }

    static int flag_to_int(const std::string& str,const std::map<std::string,std::string>&mp){
        if(is_int(str)){
            return std::stoll(str);
        }
        else{
            return std::stoll(mp.at(str));
        }
    }
    static double flag_to_double(const std::string& str,const std::map<std::string,std::string>&mp){
        if(is_double(str)){
            return std::stod(str);
        }
        else{
            return std::stod(mp.at(str));
        }
    }
};

#endif //MAIN_PROJECT_INPUT_H
