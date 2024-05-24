vector<vector<mint>> pascal;
//calculate from 0C0 to nCn
void pascal_init(int n) {
    pascal.assign(n + 1, vector<mint>(n + 1));
    pascal[0][0] = 1;
    for(int i = 1; i < n + 1; i++) {
        pascal[i][0] = 1;
        for(int j = 1; j < n + 1; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
}

vector<vector<long long>> pascal;
//calculate from 0C0 to nCn
void pascal_init(int n) {
    pascal.assign(n + 1, vector<long long>(n + 1));
    pascal[0][0] = 1;
    for(int i = 1; i < n + 1; i++) {
        pascal[i][0] = 1;
        for(int j = 1; j < n + 1; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
}


