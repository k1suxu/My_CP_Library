// two demention 1_indexed
// 0_indexed -> delete the "+1"s in the making function
template<typename T>
pair<vector<T>, vector<T>> compress(vector<T> &a, vector<T> &b) {
    int n = a.size();
    vector<T> val_x = a, val_y = b;
    sort(val_x.begin(), val_x.end());
    sort(val_y.begin(), val_y.end());
    val_x.erase(unique(val_x.begin(), val_x.end()), val_x.end());
    val_y.erase(unique(val_y.begin(), val_y.end()), val_y.end());
    // making function
    for(int i = 0; i < n; i++) {
        a[i] = lower_bound(val_x.begin(), val_x.end(), a[i]) - val_x.begin();
        b[i] = lower_bound(val_y.begin(), val_y.end(), b[i]) - val_y.begin();
    }
    //vals[X[i]] = original X[i]
    return make_pair(val_x, val_y);
}