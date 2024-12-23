template <int char_size>
struct TrieNode {
    int nxt[char_size];

    // [TODO]: 追加で持たせたい情報
    // int nearest_leaf_dist;

    int exist;
    vector<int> accept;

    TrieNode() : exist(0) {
        memset(nxt, -1, sizeof(nxt));
        // [TODO]: 新しいノード作るときの初期値
        // nearest_leaf_dist = INF;
    }
};

template <int char_size, int margin>
struct Trie {
private:
    using Node = TrieNode<char_size>;

    vector<Node> nodes;
    int root;

    void update_direct(int node, int id) {
        // [TODO]: acceptリスト要らないときは使わないようにする (https://atcoder.jp/contests/abc377/submissions/59448755 でa連打みたいなのされて困った)
        // nodes[node].accept.push_back(id);
        // [TODO]: 最後(文字列の読み込みおわり)まで見たときの挙動
        // nodes[node].nearest_leaf_dist = 0;
    }

    void update_child(int node, int child, int id) {
        ++nodes[node].exist;
        // [TODO]: ノードの接続関係の変更
        // chmin(nodes[node].nearest_leaf_dist, nodes[child].nearest_leaf_dist + 1);
    }

    void add(const string &str, int str_index, int node_index, int id) {
        if (str_index == str.size()) {
            update_direct(node_index, id);
        } else {
            const int c = str[str_index] - margin;
            if (nodes[node_index].nxt[c] == -1) {
                nodes[node_index].nxt[c] = (int)nodes.size();
                nodes.push_back(Node());
            }
            add(str, str_index + 1, nodes[node_index].nxt[c], id);
            update_child(node_index, nodes[node_index].nxt[c], id);
        }
    }

    void add(const string &str, int id) { add(str, 0, 0, id); }
public:
    Trie() : root(0) { nodes.push_back(Node()); }

    void add(const string &str) { add(str, nodes[0].exist); }

    void query(const string &str, const function<void(int)> &f) {
        query(str, f, 0, 0);
    }

    // こっちでは読み込む文字の開始位置とノードの開始位置を指定できる
    void query(const string &str, const function<void(int)> &f, int str_index, int node_index) {
        for (auto &idx : nodes[node_index].accept) f(idx);
        if (str_index == str.size()) {
            return;
        } else {
            const int c = str[str_index] - margin;
            if (nodes[node_index].nxt[c] == -1) return;
            query(str, f, str_index + 1, nodes[node_index].nxt[c]);
        }
    }

    // 各文字が読み込まれるごとにfを呼ぶ(読み込んだ後の場所で) + lcpを返す
    int query_prefix(const string &str, function<void(int, Node)> f, int str_idx = 0, int node_idx = 0) {
        while(str_idx < (int)str.size() && nodes[node_idx].nxt[(int)(str[str_idx] - margin)] != -1) {
            node_idx = nodes[node_idx].nxt[(int)(str[str_idx] - margin)];
            f(str_idx, nodes[node_idx]);
            str_idx++;
        }
        return str_idx;
    }

    int count() const { return (nodes[0].exist); } // 根の個数

    int size() const { return ((int)nodes.size()); }
};

// テンプレート引数のchar_sizeは、使用する文字の種類数
// ref: https://ei1333.github.io/library/structure/trie/trie.hpp
// query(文字列Sのプレフィックスに一致する文字列を検索する)のfは、prefixの文字が一致する度に呼ばれる(引数は何番目に挿入した文字列がLCP全一致するか(insert AAA, insert AAB, query AABBとかってやるとAABのindex1がfに一度だけ渡される))。　// ref: https://ei1333.github.io/luzhiled/snippets/structure/trie.html
// marginは'A'など、文字を0-indexedにする際に引くもの

// vector<T> = T_stringにも対応したいね

// 各ノードに対して部分木全体の○○(冪等性あり)とかが持たせられそう？

// [TODO]あり