#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>

using namespace std;

// #define SHOW_item
// #define SHOW_path
#define SHOW_travel

const int N = 40010, M = 350010;

struct CITY
{
    int x;
    int y;
    vector<int> item_list;
}city[N];

struct ITEM
{
    int id;
    int val;
    int weight;
    int pos;
    double score;
    bool operator < (const ITEM &t) const {
        return score > t.score;
    }
}items[M], tmp_items[M];

int n, m;
double C;
double maxv, minv, rent_r;
bool vis[N], take_item[M];
vector<int> path;
map<CITY, double> dis_table;

double get_dis(CITY &a,CITY &b)
{
    return sqrt((double)(a.x - b.x) * (a.x - b.x) + (double)(a.y - b.y) * (a.y - b.y));
}

void dfs(int u)
{
    if (path.size() == n) return ;
    int min_len = 0x3f3f3f3f, id = -1;
    for (int i = 1; i <= n; i ++)
    {
        if (vis[i]) continue;
        int distance = get_dis(city[u], city[i]); 
        if (min_len > distance) {   // 这里可以随机选择
            min_len = distance;
            id = i;
        }
    }
    path.push_back(id);
    vis[id] = 1;
    dfs(id);
}

double cal_val()
{
    double cur_v = maxv;
    double cur_weight = 0;
    double cur_cost = 0, cur_val = 0;
    int cur_city_id = 1, next_city_id;
    for (int i = path.size()-1; i >= 0; i --) 
    {
        next_city_id = path[i];
        CITY &next_city = city[next_city_id];
        CITY &cur_city = city[cur_city_id];
        
        for (auto item_id : cur_city.item_list) {
            if (take_item[item_id] == true) {
                ITEM &t = items[item_id];
                cur_weight += t.weight;
                cur_val += t.val;
            }
        }
        

        double weight_rate = cur_weight / C;
        cur_v = maxv - ((maxv - minv) * weight_rate);
        double distance = get_dis(cur_city, next_city);
        cur_cost += distance / cur_v;

#ifdef SHOW_travel
        fprintf(stderr, "%d->%d[%lf]: \tcur_val: %lf, cur_weight: %lf, weight_rate: %lf, cur_v: %lf, cost_time: %lf, sum_time: %lf\n", 
                        cur_city_id, next_city_id, distance, cur_val, cur_weight, weight_rate, cur_v, distance / cur_v, cur_cost);
#endif


        cur_city_id = next_city_id;
    }
    return cur_val - cur_cost * rent_r;
}

int main(int argc, char *argv[])
{
    string file, loadfile, itemfile, savefile;
    if (argc > 2) {
        file = argv[1];
    }else {
        // cin >> file;
        file = "a280_n279";
        // file = "a280_n1395";
    }
    loadfile = "./data/" + file;
    savefile = "./results/" + file;
    freopen(loadfile.c_str(), "r", stdin);

    cin >> C;
    cin >> minv;
    cin >> maxv;
    cin >> rent_r;

    cin >> n;
    for (int i = 1; i <= n; i ++) {
        int id, a, b;
        cin >> id >> a >> b;
        city[id] = {a, b};
    }

#ifdef SHOW_dis
    freopen("./distance.out", "w", stdout);
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            fprintf(stdout, "%lf\n", get_dis(city[i], city[j]));
        }
    }
#endif

    cin >> m;
    for (int i = 1; i <= m; i ++) {
        int id, val, weight, pos;
        cin >> id >> val >> weight >> pos;
        
        double a, b, c;
        a = ((double)rand() / (INT_MAX));
        b = ((double)rand() / (INT_MAX));
        a = a / (a + b);
        b = b / (a + b);
        double score = pow(val, a) / pow(weight, b);
        // double score = val / weight;

        items[id] = {id, val, weight, pos, score};

        city[pos].item_list.push_back(id);
    }

    memcpy(tmp_items, items, sizeof items);
    sort(tmp_items + 1, tmp_items + m + 1);

#ifdef SHOW_item
    for (int i = 1; i <= 30; i ++) {
        ITEM &t = tmp_items[i];
        fprintf(stdout, "id: %d,  \t position: %d,\t val: %d,\t weight: %d,\t score: %lf\n", t.id, t.pos, t.val, t.weight, t.score);
    }
#endif

    path.clear();
    memset(vis, 0, sizeof vis);
    memset(take_item, 0, sizeof take_item);
    
    path.push_back(1);
    vis[1] = true;


    int sum = 0;
    for (int i = 1; i <= m; i ++) {
        ITEM &t =  tmp_items[i];
        sum += t.weight;
        if (sum > C) break;
        take_item[t.id] = true;     // 在这个位置拿东西
        if (i == 1 && t.pos == 1) {
            continue;
        }
        path.push_back(t.pos);
        vis[t.pos] = true;
    }

    dfs(path[path.size() - 1]);


#ifdef SHOW_path
    for (auto v : path)
    {
        fprintf(stdout, "%d ", v);
    }
    puts("");
    for (int v : path) {
        CITY &t = city[v];
        printf("city: %d: ", v);
        for (auto it : t.item_list) {
            printf("%d -- %d", it, take_item[it]);
        }
        puts("");
        // fprintf(stdout, "%d ", );
    }
    puts("");
#endif


    double ans = cal_val();
    printf("ans: %lf\n", ans);

    puts("");
}

/*
g++ ttp.cpp -o ttp && ./ttp
*/