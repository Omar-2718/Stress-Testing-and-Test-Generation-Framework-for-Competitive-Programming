#include <bits/stdc++.h>
#include <windows.h>
#include "CompareOutputs.h"
#include "Input.h"
#include "UI.h"
#include "CommandManager.h"
#include "Test_Manager.h"
#include "sstream"
#ifndef int
#define int long long
#endif
using namespace std;

// 1 - create test
// 2 - add custom rules
// 3 - run test
// 4 - restart

// 3 - run test
// 4 -restart

// first line : number t
// first line : t loop
// second line : number n
// third line n loop
// third line : --unique <=1e9



static bool ON = true;
Tests_Manager test_manager;
class Program{
public:
    static void run(){
        bool couldnt_open = false;
        Command_Manager::init();
        while (ON) {
            UI::start_menu();
            if(couldnt_open) {
                UI::couldntOpenFile();
                couldnt_open = false;
            }
            int ch = Input::read_int();
            if(ch == 1){
                test_manager.create_test();
            }
            else if(ch == 3){
                UI::test_name_query();
                string test_name = Input::read_line();
                if(test_manager.read_test(test_name))
                test_manager.run_tests();
                else
                    couldnt_open = true;
            }
            else if(ch == 4){
                test_manager.create_test(0);
            }
            else if(ch == 5){
                test_manager.run_tests();
            }
            else if(ch == 6){
                Compare_Outputs::run_tests(Input::read_test_count());
            }
            else if(ch == 2){
                test_manager.run_custom_test();
            }
            else{
                ON = false;
            }
        }
    }
};
signed main(){
//    while (true) {
//        int l, r, ai_min, ai_max;
//        cin >> l >> r >> ai_min >> ai_max;
//        for (auto e: Random::better_random_numbers_sum_range(l, r, ai_min, ai_max, 10)) {
//            cout << e << " ";
//        }
//    }
//    int t;cin>>t;
    Program::run();
    return 0;
}