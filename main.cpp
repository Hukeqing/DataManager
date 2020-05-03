//#define NOT_BREAK_ON_ERROR

#include "data_manager.h"

int main() {
    DataManager dataManager("test");
    if (!dataManager) {
        dataManager.addItem("data", String);
        dataManager.addItem("k", Int);
        dataManager.addItem("b", Int);
        dataManager.addItem("time", Double);
        dataManager.endBuild();
    }
    dataManager.setItem("data", "test_data");
    dataManager.setItem("k", 10);
//    dataManager.setItem("b", 4);
//    dataManager.clear("b");
//    dataManager.setItem("time", 1.5);
    double temp;
    map<int, double> res;
    if (dataManager.get_vary("b", res, true)) {
        for (auto &item : res) {
            cout << item.first << " " << item.second << endl;
        }
    }
    return 0;
}
