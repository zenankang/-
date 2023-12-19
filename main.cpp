#include <bits/stdc++.h>
#include<windows.h>
#include<conio.h>

using namespace std;

const int N = 6, M = 6;
const int TermX = 4, TermY = 3;
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

//颜色的代码: 0=黑色 1蓝色 2绿色 3湖蓝色 4红色 紫色 6黄色 7白色 8灰色 9淡蓝色
enum colour{black,blue,green,lakeblue,red,purple,yellow,white,gray,babyblue};
void changeColor(int forecolour,int backcolour){
    SetConsoleTextAttribute(handle, forecolour + backcolour * 0x10);
}
// 输出地图
void pMap(COORD ori, const std::string &key) {
    const auto Pstr = [](const char *s, colour fg, colour bg) {
        changeColor(fg, bg); printf("%s", s);
    };
    unordered_map<char, colour> colmap({
        {'0', gray}, {'1', blue}, {'2', blue}, {'3', red}, {'4', red}, {'5', green},
    });
    vector<int> vis(N*M);
    colour bg = babyblue;
    SetConsoleCursorPosition(handle, ori);
    for (int i = 0; i < N; i++) {
        for (int l = 0; l <= 1; l++) {
            for (int j = 0; j < M; j++) {
                colour col = colmap[key[i*N+j]];
                changeColor(col, gray);
                int p = i*N+j;
                if (key[p] == '1' || key[p] == '5') {
                    if (vis[p] == 0) {
                        if (p+N < N*M) vis[p+N] = 1;
                        if (l==0) {
                            Pstr("####", col, bg);
                        } else {
                            Pstr("#", col, bg); Pstr("  ", col, col); Pstr("#", col, bg);
                        }
                    } else {
                        if (l==0) {
                            Pstr("#", col, bg); Pstr("  ", col, col); Pstr("#", col, bg);
                        } else {
                            Pstr("####", col, bg);
                        }
                    }
                } else if (key[p] == '3') {
                    if (vis[p] == 0) {
                        vis[p+N] = 1; vis[p+2*N] = 2;
                        if (l==0) {
                            Pstr("####", col, bg);
                        } else {
                            Pstr("#", col, bg); Pstr("  ", col, col); Pstr("#", col, bg);
                        }
                    } else if (vis[p] == 1) {
                        Pstr("#", col, bg); Pstr("  ", col, col); Pstr("#", col, bg);
                    } else {
                        if (l==0) {
                            Pstr("#", col, bg); Pstr("  ", col, col); Pstr("#", col, bg);
                        } else {
                            Pstr("####", col, bg);
                        }
                    }
                } else if (key[p] == '2') {
                    if (vis[p] == 0) {
                        vis[p+1] = 1;
                        Pstr("%", col, bg); Pstr(l==0 ? "MMM" : "WWW", bg, col);
                    } else {
                        Pstr(l==0 ? "MMM" : "WWW", bg, col); Pstr("%", col, bg);
                    }
                } else if (key[p] == '4') {
                    if (vis[p] == 0) {
                        vis[p+1] = 1; vis[p+2] = 2;
                        Pstr("%", col, bg); Pstr(l==0 ? "MMM" : "WWW", bg, col);
                    } else if (vis[p] == 1) {
                        Pstr(l==0 ? "MMMM" : "WWWW", bg, col);
                    } else {
                        Pstr(l==0 ? "MMM" : "WWW", bg, col); Pstr("%", col, bg);
                    }
                } else {
                    Pstr("    ", bg, bg);
                }
            }
            Pstr(" \n", white, black);
        }
    }
}

// 是否到达终态
bool isTerm(const std::string &key) {
    int p = TermX*N + TermY;
    return key[p] == '5' && key[p+N] == '5';
}

// 搜索步进状态
vector<string> searchNext(const string &key) {
    auto tryMove = [&](const int& p) -> vector<int> {
        if (key[p] == '0') return {};
        vector<int> aims;
        int x = p/N, y = p%N;
        if (key[p]&1) {   // 上下
            if (x > 0) {
                int np = (x-1)*N + y;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p-N);
            }
            if ((key[p] == '1' || key[p] == '5') && x + 2 < N) {
                int np = (x+2)*N + y;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p+N);
            } else if (key[p] == '3' && x + 3 < N) {
                int np = (x+3)*N + y;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p+N);
            }
        } else {     // 左右
            if (y > 0) {
                int np = x*N + y-1;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p-1);
            }
            if (key[p] == '2' && y + 2 < M) {
                int np = x*N + y+2;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p+1);
            } else if (key[p] == '4' && y + 3 < M) {
                int np = x*N + y+3;
                if (np >= 0 && np < N*M && key[np] == '0')
                    aims.emplace_back(p+1);
            }
        }
        return move(aims);
    };
    vector<string> next_keys;
    vector<int> vis(N*M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int p = i*N+j;
            if ((!vis[p]) && (key[p] != '0')) {
                /// 空白 0 短横2 长横4    短竖1 长竖3 开关5
                char ch = key[p];
                auto aims = tryMove(p);
                if (key[p] == '1' || key[p] == '5') {
                    vis[p] = vis[p+N] = 1;
                    for (auto np : aims) {
                        next_keys.push_back(key);
                        string& next_key = next_keys.back();
                        next_key[p] = next_key[p+N] = '0';
                        next_key[np] = next_key[np+N] = ch;
                    }
                } else if (key[p] == '3') {
                    vis[p] = vis[p+N] = vis[p+2*N] = 1;
                    for (auto np : aims) {
                        next_keys.push_back(key);
                        string& next_key = next_keys.back();
                        next_key[p] = next_key[p+N] = next_key[p+2*N] = '0';
                        next_key[np] = next_key[np+N] = next_key[np+2*N] = ch;
                    }
                } else if (key[p] == '2') {
                    vis[p] = vis[p+1] = 1;
                    for (auto np : aims) {
                        next_keys.push_back(key);
                        string& next_key = next_keys.back();
                        next_key[p] = next_key[p+1] = '0';
                        next_key[np] = next_key[np+1] = ch;
                    }
                } else if (key[p] == '4') {
                    vis[p] = vis[p+1] = vis[p+2] = 1;
                    for (auto np : aims) {
                        next_keys.push_back(key);
                        string& next_key = next_keys.back();
                        next_key[p] = next_key[p+1] = next_key[p+2] = '0';
                        next_key[np] = next_key[np+1] = next_key[np+2] = ch;
                    }
                }
            }
        }
    }
    return move(next_keys);
}

vector<string> BFS(const string& s) {
    unordered_map<string, string> pref; // 路径回溯
    unordered_set<string> vis;
    queue<string> que;
    que.emplace(s);
    vis.insert(s);
    string term_key = "";
    while( que.size() && !term_key.size() ) {
        string dkey = que.front(); que.pop();
//        pMap({0,10}, dkey);
        for (const auto& nkey : searchNext(dkey)) {
            if (vis.count(nkey) == 0) {
//                pMap({0,18}, nkey);
                pref[nkey] = dkey;
                que.emplace(nkey);
                vis.insert(nkey);
                if (isTerm(nkey)) {
                    term_key = nkey;
                    break;
                }
            }
        }
    }
    if (term_key == "") return {};  // 无解
    vector<string> res;
    string tmp_key = term_key;
    term_key[4*N+3] = '0';  // 真终态补充
    res.emplace_back(term_key);
    while( tmp_key != s) {
        res.emplace_back(tmp_key);
        tmp_key = pref[tmp_key];
    }
    res.emplace_back(s);    // 初态
    reverse(res.begin(), res.end());
    return move(res);
}

int main() {
/* 测试用例
001522
221501
111001
111001
114441
110000

223500
003522
333001
334441
330000
002222

100000
104440
220220
122522
100501
220221
*/
    while(1) {
        puts("请输入地图：\n空白[0]  短横[2]  长横[4]  短竖[1]  长竖[3]  开关[5]");
        string start, tmp;
        bool invalid = false;
        for (int i = 0; i < N && !invalid; i++) {
            cin>>tmp;
            start += string(tmp);
            invalid |= tmp.size() != 6;
            for (auto ch : tmp) invalid |= !(ch >= '0' && ch <= '5');
        }
        vector<string> res = BFS(start);
        if (invalid || res.size() == 0) {
            puts("************************************");
            puts("输入数据有误!!!请仔细核对并重新输入!");
            puts("************************************");
            continue;
        }
        puts("* 解法已生成 *");
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        GetConsoleScreenBufferInfo(handle, &cbsi);
        COORD ori = cbsi.dwCursorPosition;
        int i = -1, n = res.size(), ch = 'd';
        do {
            if (ch == 'a') {
                i = max(0, i-1);
            } else if (ch == 'd'){
                i = min(n-1, i+1);
            } else if (ch == 'r') {
                ori.X = ori.Y = 0;
                SetConsoleCursorPosition(handle, ori);
                system("cls");
                break;
            } else continue;
            pMap(ori, res[i]);
            printf("[D]下一步  [A]上一步  [R]重新输入  当前步骤[%d/%d]    \n", i+1, n);
        } while((ch = getch()) != EOF);
    }
    return 0;
}
