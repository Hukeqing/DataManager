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
    dataManager.setItem("b", 3);
//    dataManager.setItem("time", 1.5);
    double temp;
    if (dataManager.get(temp, true)) {
        cout << temp << endl;
    }
    return 0;
}
