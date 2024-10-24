#include <iostream>
#include <vector>
using namespace std;

// 前缀和：即数组的前n项和
// 例题：leetcode1480：一维数组的动态和

class Solution {
public:
    vector<int> runningSum(vector<int>& nums) {
        int n = nums.size();
        vector<int> arr(n);
        arr[0] = nums[0];
        for (int i = 1; i < n; i++) arr[i] = arr[i - 1] + nums[i];
        return arr;
    }
};

// 这种写法的时间复杂度是O(n)，空间复杂度是O(n)，可以进一步优化空间复杂度为O(1)
class Solution2 {
public:
    vector<int> runningSum(vector<int>& nums) {
        int n = nums.size();
        for (int i = 1; i < n; i++) nums[i] += nums[i - 1];
        return nums;
    }
};

// 差分：即数组相邻两项的差值
// 差分是一种和前缀和相对的策略，可以当作求和的逆运算


int main() {
    vector<int> nums = {1, 2, 3, 4};
    Solution S;
    vector<int> result = S.runningSum(nums);
    for (int i = 0; i < result.size(); i++) {
        if (i) cout << " ";
        cout << result[i];
    }
    cout << endl;
    Solution2 S2;
    vector<int> result2 = S2.runningSum(nums);
    for (int i = 0; i < result2.size(); i++) {
        if (i) cout << " ";
        cout << result2[i];
    }
    cout << endl;
    return 0;
}