//Graph
//Topological Sort
// Heap_Topological_Sort
//
//O((V+E) log E)
// if ans.size() != number of vertex
// unsortable -> can't sort
vector<int> topological_sort(vector<vector<int>> g) {
    int n = g.size();
    vector<int> indeg(n, 0);
    for(int i = 0; i < n; i++) {
        for(auto e : g[i]) {
            indeg[e]++;
        }
    }
    priority_queue<int, vector<int>, greater<int>> que;
    for(int i = 0; i < n; i++) {
        if(indeg[i] == 0) {
            que.push(i);
        }
    }
    vector<int> ans;
    while(!que.empty()) {
        int v = que.top();
        que.pop();
        ans.push_back(v);
        for(auto e : g[v]) {
            indeg[e]--;
            if(indeg[e] == 0) {
                que.push(e);
            }
        }
    }
    return ans;
}