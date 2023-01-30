template<typename T = int>
void print_vec(vector<T> vec) {
    int n = (int)vec.size();
    for(int i = 0; i < n; i++) {
        cout << vec[i];
        if(i != n - 1) cout << " ";
        else cout << endl;
    }
}