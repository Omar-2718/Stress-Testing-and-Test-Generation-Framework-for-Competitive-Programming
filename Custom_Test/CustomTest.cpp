#include "iostream"
#include "random"
#include "chrono"
using namespace std;
static int random_int(int low,int high){
    static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::mt19937 gen(seed);  // Seed the Mersenne Twister engine with a high-resolution clock
    std::uniform_int_distribution<int> dist(low, high);
    return dist(gen);
}
// Create your test and output to std::cout and the stress test will generate your tests automaticallty
signed main(){
}