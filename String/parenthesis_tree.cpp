bool valid_parenthesis(string s) {
    int cur = 0;
    for (char c : s) {
        if (c == '(') cur++;
        else if (c == ')') cur--;
        else return false;
        if (cur < 0) return false;
    }
    return cur == 0;
}

vector<vector<int>> parenthesis_tree(string s) {
    assert(valid_parenthesis(s));
    stack<int> st;
    st.push(0);
    vector<vector<int>> tree;
    tree.emplace_back();
    int t = 1;
    for (char c : s) {
        if (c == '(') {
            st.push(t++);
            tree.emplace_back();
        } else {
            int i = st.top();
            st.pop();
            tree[st.top()].emplace_back(i);
        }
    }
    return tree;
}