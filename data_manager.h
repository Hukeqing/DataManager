//
// Created by HKQ on 4/24/2020.
//

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <bits/stdc++.h>

using namespace std;

// #define NOT_BREAK_ON_ERROR

enum DataClass {
    Int = 0, Double, String
};

struct Data {
    int *int_data;
    double *double_data;
    string *string_data;

    void init(int n) {
        int_data = new int[n];
        double_data = new double[n];
        string_data = new string[n];
    }

    bool equal(const Data &other, unsigned flag, const map<string, DataClass> &list) {
        unsigned cnt = 0;
        auto iter = list.begin();
        while (iter != list.end()) {
            if (flag & (1u << cnt))
                switch (iter->second) {
                    case Int:
                        if (int_data[cnt] != other.int_data[cnt])
                            return false;
                        break;
                    case Double:
                        if (double_data[cnt] != other.double_data[cnt])
                            return false;
                        break;
                    case String:
                        if (string_data[cnt] != other.string_data[cnt])
                            return false;
                        break;
                }
            iter++;
            cnt++;
        }
        return true;
    }
};

class DataManager {
private:
    string file_name;
    fstream file;
    bool isNew;
    map<string, DataClass> name_list;
    unsigned status;
    Data data{};

    static inline void throw_error(const string &s) {
#ifndef NOT_BREAK_ON_ERROR
        throw logic_error(s);
#endif
    }

    void unpack(const string &str, Data &d) {
        stringstream ss(str);

        int di, flag = 0;
        double dd;
        string ds;

        auto iter = name_list.begin();
        while (iter != name_list.end()) {
            switch (iter->second) {
                case Int:
                    ss >> di;
                    d.int_data[flag] = di;
                    break;
                case Double:
                    ss >> dd;
                    d.double_data[flag] = dd;
                    break;
                case String:
                    ss >> ds;
                    d.string_data[flag] = ds;
                    break;
            }
            flag++;
            iter++;
        }
    }

public:
    explicit DataManager(const string &name) {
        file_name = name + ".dm";
        file.open(file_name, ios::in);
        if (file) {
            string names;
            getline(file, names);
            stringstream ss(names);
            try {
                string tmp1;
                int tmp2;
                while (ss >> tmp1) {
                    ss >> tmp2;
                    name_list.insert({tmp1, (DataClass) tmp2});
                }
                data.init(name_list.size());
            } catch (...) {
                cerr << "File Form Error!" << endl;
                exit(-1);
            }
            status = 0;
            isNew = false;
        } else {
            isNew = true;
        }
        file.close();
    }

    bool endBuild() {
        if (isNew) {
            isNew = false;
            data.int_data = new int[name_list.size()];
            data.double_data = new double[name_list.size()];
            data.string_data = new string[name_list.size()];
            status = 0;
            file.open(file_name, ios::out);
            auto iter = name_list.begin();
            while (iter != name_list.end()) {
                file << iter->first << " " << (int) (iter->second) << " ";
                iter++;
            }
            file << endl;
            file.close();
            return true;
        } else {
            throw_error("this data manager has been built yet");
            return false;
        }
    }

    bool addItem(const string &name, DataClass dataClass) {
        if (!isNew) {
            throw_error("this data manager has been built yet");
            return false;
        }
        name_list.insert({name, dataClass});
        return true;
    }

    bool setItem(const string &item, int value) {
        auto iter = name_list.find(item);
        if (iter == name_list.end() || iter->second != Int) {
            throw_error("Do not find the tag or the type of the item is error: " + item);
            return false;
        }
        unsigned tmp = distance(name_list.begin(), iter);
        data.int_data[tmp] = value;
        status |= 1u << tmp;
        return true;
    }

    bool setItem(const string &item, double value) {
        auto iter = name_list.find(item);
        if (iter == name_list.end() || iter->second != Double) {
            throw_error("Do not find the tag or the type of the item is error: " + item);
            return false;
        }
        unsigned tmp = distance(name_list.begin(), iter);
        data.double_data[tmp] = value;
        status |= 1u << tmp;
        return true;
    }

    bool setItem(const string &item, const string &value) {
        auto iter = name_list.find(item);
        if (iter == name_list.end() || iter->second != String) {
            throw_error("Do not find the tag or the type of the item is error: " + item);
            return false;
        }
        unsigned tmp = distance(name_list.begin(), iter);
        data.string_data[tmp] = value;
        status |= 1u << tmp;
        return true;
    }

    bool clear() {
        status = 0;
    }

    bool clearItem(const string &item) {
        auto iter = name_list.find(item);
        if (iter == name_list.end()) {
            throw_error("Do not find the tag: " + item);
            return false;
        }
        unsigned tmp = distance(name_list.begin(), iter);
        if ((status >> tmp) & 1u) {
            status ^= 1u << tmp;
        }
        return true;
    }

    bool update() {
        if (status + 1 != 1u << name_list.size()) {
            throw_error("data status is not complete");
            return false;
        }
        file.open(file_name, ios::app);
        auto iter = name_list.begin();
        int flag = 0;
        while (iter != name_list.end()) {
            switch (iter->second) {
                case Int:
                    file << data.int_data[flag] << " ";
                    break;
                case Double:
                    file << data.double_data[flag] << " ";
                    break;
                case String:
                    file << data.string_data[flag] << " ";
                    break;
                default:
                    break;
            }
            flag++;
            iter++;
        }
        file << endl;
        return true;
    }

    bool get(int &value, bool average = false) {
        value = 0;
        unsigned tmp = status, cnt = 0, flag = name_list.size() - 1;
        while (tmp) {
            if (tmp & 1u)
                cnt++;
            else
                flag = cnt;
            tmp >>= 1u;
        }
        if (cnt + 1 != name_list.size()) {
            throw_error("data status is not complete or there is too much tag");
            return false;
        }
        file.open(file_name, ios::in);
        string s;
        getline(file, s);
        Data cur{};
        cur.init(name_list.size());

        cnt = 0;

        while (getline(file, s) && !s.empty()) {
            unpack(s, cur);
            if (cur.equal(data, status, name_list)) {
                auto iter = name_list.begin();
                advance(iter, flag);
                if (iter->second != Int) {
                    throw_error("The ref type is not same as database");
                    return false;
                }
                cnt++;
                value = average ? value + cur.int_data[flag] : cur.int_data[flag];
            }
        }
        if (average)
            value /= (int) (cnt);
        return true;
    }

    bool get(double &value, bool average = false) {
        value = 0;
        unsigned tmp = status, cnt = 0, flag = name_list.size() - 1;
        while (tmp) {
            if (tmp & 1u)
                cnt++;
            else
                flag = cnt;
            tmp >>= 1u;
        }
        if (cnt + 1 != name_list.size()) {
            throw_error("data status is not complete or there is too much tag");
            return false;
        }
        file.open(file_name, ios::in);
        string s;
        getline(file, s);
        Data cur{};
        cur.init(name_list.size());

        cnt = 0;

        while (getline(file, s) && !s.empty()) {
            unpack(s, cur);
            if (cur.equal(data, status, name_list)) {
                auto iter = name_list.begin();
                advance(iter, flag);
                if (iter->second != Double) {
                    throw_error("The ref type is not same as database");
                    return false;
                }
                cnt++;
                value = average ? value + cur.double_data[flag] : cur.double_data[flag];
            }
        }
        if (average)
            value /= (int) (cnt);
        return true;
    }

    bool get(string &value) {
        value.clear();
        unsigned tmp = status, cnt = 0, flag = name_list.size() - 1;
        while (tmp) {
            if (tmp & 1u)
                cnt++;
            else
                flag = cnt;
            tmp >>= 1u;
        }
        if (cnt + 1 != name_list.size()) {
            throw_error("data status is not complete or there is too much tag");
            return false;
        }
        file.open(file_name, ios::in);
        string s;
        getline(file, s);
        Data cur{};
        cur.init(name_list.size());

        while (getline(file, s) && !s.empty()) {
            unpack(s, cur);
            if (cur.equal(data, status, name_list)) {
                auto iter = name_list.begin();
                advance(iter, flag);
                if (iter->second != Double) {
                    throw_error("The ref type is not same as database");
                    return false;
                }
                value = cur.string_data[flag];
            }
        }
        return true;
    }

    // NOLINTNEXTLINE
    operator void *() const {
        return isNew ? (void *) nullptr : (void *) 1;
    }
};

#endif //DATA_MANAGER_H
