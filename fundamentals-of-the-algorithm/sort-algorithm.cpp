#include <iostream>
#include <vector>

// 插入排序
void insert_sort(std::vector<int>& num) {
    for (int i = 1; i < num.size(); i++) {
        for (int j = i; j > 0 && num[j] < num[j - 1]; j--) std::swap(num[j], num[j - 1]);
    }
    return ;
}

// 冒泡排序
void bubble_sort(std::vector<int>& num) {
    int flag = 1;
    for (int i = 1; i < num.size() - 1 && flag; i++) {
        flag = 0;
        for (int j = 0; j < num.size() - i; j++) {
            if (num[j] <= num[j + 1]) continue;
            std::swap(num[j], num[j + 1]);
            flag ++;
        }
    }
    return ;
}


int main() {
    std::vector<int> num = {2,1,90,72,19};
    insert_sort(num);
    for (auto x : num) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    return 0;
}