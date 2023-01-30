template<typename T = int>
void print_2dvec(vector<vector<T>> a) {
    int n = (int)a.size();
    int m = (int)a[0].size();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cout << a[i][j];
            if(j != m - 1) cout << " ";
            else cout << endl;
        }
    }
}