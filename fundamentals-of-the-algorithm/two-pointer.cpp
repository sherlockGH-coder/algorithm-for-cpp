#include <iostream>
#include <vector>
#include <algorithm>

/*
双指针定义：通过设置两个指针不断单向移动来解决问题的方法。

形式：
1、两个指针分别指向不同的序列
2、两个指针指向同一个序列

核心用途：
优化时间复杂度

双指针模板：
for (int i = 0, j = 0; i < n; i ++ ) {
    while (j < i && check(i, j)) j ++ ;
    // 具体问题的逻辑
}

常见类型：
1、快慢指针
2、对撞指针
3、滑动窗口

首先对于每一道题，可以先考虑暴力解法，然后再考虑题目中是否具有双指针的特点，即“单调性”（即当一个指针移动时，在希望得到答案的前提下，另外一个指针只能单向移动），如果具有这种特点，那么就可以考虑使用双指针来解决问题。

*/

// 例题：三数之和

std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
    sort(nums.begin(), nums.end());
    std::vector<std::vector<int> > res;
    for (int i = 0; i < nums.size(); i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        for (int j = i + 1, k = nums.size() - 1; j < nums.size(); j++) {
            if (j > i + 1 && nums[j] == nums[j - 1]) continue;
            while (j < k && nums[j] + nums[k] > -nums[i]) k--;
            if (j == k) break;
            if (nums[j] + nums[k] + nums[i] == 0) {
                res.push_back({nums[i], nums[j], nums[k]});
            }
        }  
    }
    return res;
}

int main() {
    std::vector<int> nums = {-1, 0, 1, 2, -1, -4, -2, -3, 3, 0, 4};
    std::vector<std::vector<int> > res = threeSum(nums);
    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[i].size(); j++) {
            std::cout << res[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}