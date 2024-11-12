#ifndef MAIN_PROJECT_RANDOM_H
#define MAIN_PROJECT_RANDOM_H
#include "random"
#ifndef int
#define int long long
#endif
class Random{
public:
    static int random_int(int low,int high){
        static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        static std::mt19937 gen(seed);  // Seed the Mersenne Twister engine with a high-resolution clock
        std::uniform_int_distribution<int> dist(std::min(low,high), std::max(low,high));
        return dist(gen);
    }
    static double random_double(double low,double high){
        static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        static std::mt19937 gen(seed);
        std::uniform_real_distribution<double>dist(std::min(low,high),std::max(low,high));
        return dist(gen);
    }
    static std::string random_string_letters(int n){
        std::string str(n,' ');
        for(int i = 0;i<n;i++){
            str[i] = 'a' + Random::random_int(0,25);
        }
        return str;
    }
    static std::string random_string_of_numbers(int n,bool ignore_leading_zeros = false){
        std::string str(n,' ');
        if(n <= 0)
            return "";
        for(int i = 0;i<n;i++){
            str[i] = '0' + Random::random_int(0,9);
        }
        // std::string ans;
        if(ignore_leading_zeros){
            // for(int i = 0;i<n;i++){
            //     if(str[i] != '0'){
            //         // 0 1 2 3 4
            //         ans = str.substr(i,n-i);
            //     }
            // }
            if(str[0] == '0')
                return random_string_of_numbers(n,ignore_leading_zeros);
        }
        return str;
    }
//    static std::vector<std::pair<int,int>> random_tree(int size=-1,int root=-1,int height=-1){
//        if(size == -1)
//            size = random_int(1,100000);
//
//    }
    static std::string random_binary_string(int n){
        std::string str(n,' ');
        for(int i = 0;i<n;i++)
            str[i] = '0' + random_int(0,1);
        return str;
    }
    static std::vector<int> random_permutation(int length){
        std::vector<int>permutations(length);
        std::iota(permutations.begin(),permutations.end(),1);
        static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        static std::mt19937 gen(seed);
        std::shuffle(permutations.begin(), permutations.end(),gen);
        return permutations;
    }
    static std::vector<std::vector<int>>random_matrix(int n,int m,int low,int high){
        std::vector<std::vector<int>>matrix(n,std::vector<int>(m));
        for(int i =0;i<n;i++){
            for(int j = 0;j<m;j++){
                matrix[i][j] = random_int(low,high);
            }
        }
        return matrix;
    }

    static std::vector<int>random_numbers_sum_range(int l ,int r,int ai_min,int ai_max,int n,int depth=100){
        std::vector<int>array(n);
        int curr_sum = 0;

        if(depth == 0)
            return std::vector<int>();
        for(int i = 0;i<n;i++){
            if(i == n-1){
                if(std::max(l - curr_sum,ai_min) < ai_min || r - curr_sum > ai_max || r-curr_sum < l-curr_sum){
                    return random_numbers_sum_range(l,r,ai_min,ai_max,n,depth-1);
                }
                array[i] = random_int(std::max(l - curr_sum,ai_min),r-curr_sum);
                static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                static std::mt19937 gen(seed);
                std::shuffle(array.begin(), array.end(),gen);
                return array;
            }
            if(std::min(r-curr_sum,ai_max) < ai_min){
                return random_numbers_sum_range(l,r,ai_min,ai_max,n,depth-1);
            }
            array[i] = random_int(ai_min, std::min(r-curr_sum,ai_max));
            curr_sum += array[i];
        }
        return std::vector<int>();
    }
    static std::vector<int>better_random_numbers_sum_range(int l ,int r,int ai_min,int ai_max,int n,int depth=100,int depth2 = 10){
        //10 10 0 10
        //this test
        if(depth2 == 0){
            return std::vector<int>();
        }
        std::vector<int> ret = random_numbers_sum_range(l,random_int(l,r),ai_min, ai_max,n,depth);
        if(ret.empty()){
            return better_random_numbers_sum_range(l,r,ai_min, ai_max,n,depth,depth2-1);
        }
        return ret;
    }
    static std::vector<double>random_numbers_sum_range_double(double l ,double r,double ai_min,double ai_max,int n,int depth=1){
        std::vector<double>array(n);
        double curr_sum = 0;

        if(depth == 0)
            return std::vector<double>();
        for(int i = 0;i<n;i++){
            if(i == n-1){
                if(std::max(l - curr_sum,ai_min) < ai_min || r - curr_sum > ai_max || r-curr_sum < l-curr_sum){
                    return random_numbers_sum_range_double(l,r,ai_min,ai_max,n,depth-1);
                }
                array[i] = random_double(std::max(l - curr_sum,ai_min),r-curr_sum);
                static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
                static std::mt19937 gen(seed);
                std::shuffle(array.begin(), array.end(),gen);
                return array;
            }
            if(std::min(r-curr_sum,ai_max) < ai_min){
                return random_numbers_sum_range_double(l,r,ai_min,ai_max,n,depth-1);
            }
            array[i] = random_double(ai_min, std::min(r-curr_sum,ai_max));
            curr_sum += array[i];
        }
        return std::vector<double>();
    }
    static std::vector<double>better_random_numbers_sum_range_double(double l ,double r,double ai_min,double ai_max,int n,int depth=100,int depth2 = 10){
        //10 10 0 10
        //this test
        if(depth2 == 0){
            return std::vector<double>();
        }
        std::vector<double> ret = random_numbers_sum_range_double(l,random_double(l,r),ai_min, ai_max,n,depth);
        if(ret.empty()){
            return better_random_numbers_sum_range_double(l,r,ai_min, ai_max,n,depth,depth2-1);
        }
        return ret;
    }

};
#endif //MAIN_PROJECT_RANDOM_H
// int n -r 100 200 , int r -r n 3000