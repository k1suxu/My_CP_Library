template<typename mod_int>
struct Matrix {
    vector<vector<mod_int>> mat;

    Matrix() = default;
    Matrix(int h, int w) : mat(vector<vector<mod_int>>(h, vector<mod_int>(w))) {}
    Matrix(int h, int w, mod_int d) : mat(vector<vector<mod_int>>(h, vector<mod_int>(w, d))) {}
    Matrix(vector<vector<mod_int>> vec) : mat(vec) {}
    
    vector<mod_int> get(int i) const {return mat[i];}
    vector<mod_int>& get(int i) {return mat[i];}
    mod_int get(int i, int j) const {return mat[i][j];}
    mod_int& get(int i, int j) {return mat[i][j];}

    int nrow = mat.size();
    int ncol = mat[0].size();

    void fill_all(mod_int d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] = d;
            }
        }
        return;
    }

    Matrix<mod_int>& operator+=(const Matrix<mod_int> other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] += other.get(i, j);
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator-=(const Matrix<mod_int> other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] -= other.get(i, j);
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator*=(const Matrix<mod_int> other) {
        assert(ncol == other.nrow);
        Matrix<mod_int> mat2(nrow, other.ncol);

        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < other.ncol; j++) {
                for(int k = 0; k < ncol; k++) {
                    mat2.get(i, j) += mat[i][k] * other.get(k, j);
                }
            }
        }
        *this = mat2;
        return *this;
    }

    Matrix<mod_int> operator+(const Matrix<mod_int> other) const {return Matrix<mod_int>(*this) += other;}
    Matrix<mod_int> operator-(const Matrix<mod_int> other) const {return Matrix<mod_int>(*this) -= other;}
    Matrix<mod_int> operator*(const Matrix<mod_int> other) const {return Matrix<mod_int>(*this) *= other;}
    
    bool operator==(const Matrix<mod_int> other) {
        if(nrow != other.nrow || ncol != other.ncol) return false;

        bool flag = true;
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                if(mat[i][j] != other.get(i, j)) flag = false;
            }
        }

        return flag;
    }

    Matrix<mod_int>& operator+=(const mod_int d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] += d;
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator-=(const mod_int d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] -= d;
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator*=(const mod_int d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] *= d;
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator/=(const mod_int d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] /= d;
            }
        }
        return *this;
    }

    Matrix<mod_int> operator+(const mod_int d) const {return Matrix<mod_int>(*this) += d;}
    Matrix<mod_int> operator-(const mod_int d) const {return Matrix<mod_int>(*this) -= d;}
    Matrix<mod_int> operator*(const mod_int d) const {return Matrix<mod_int>(*this) *= d;}
    Matrix<mod_int> operator/(const mod_int d) const {return Matrix<mod_int>(*this) /= d;}

    void print() {
        cout << "Size : " << ncol << " * " << nrow << endl;
        for(int i = 0; i < nrow; i++) {
            cout << "{";
            for(int j = 0; j < ncol; j++) {
                cout << mat[i][j].val();
                if(j != ncol - 1) cout << ", ";
                else cout << "}\n";
            }
        }
        return;
    }

    void input() {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                cin >> mat[i][j];
            }
        }
    }
    void output() {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                if(j != ncol - 1) cout << mat[i][j] << " ";
                else cout << mat[i][j] << "\n";
            }
        }
    }
};

template<typename mod_int>
Matrix<mod_int> Make_E(int SIZE) {
    Matrix<mod_int> ret(SIZE, SIZE, 0);
    for(int i = 0; i < SIZE; i++) ret.get(i, i) = 1;
    return ret;
}

template<typename mod_int>
Matrix<mod_int> mat_pow(Matrix<mod_int> mat, long long k) {
    assert(mat.nrow == mat.ncol);
    Matrix<mod_int> mat2 = Make_E(mat.nrow);

    while(k) {
        if(k&1) mat2 *= mat;
        mat *= mat;
        k >>= 1LL;
    }

    return mat2;
}

template<typename mod_int>
Matrix<mod_int> rot(Matrix<mod_int> mat, long long deg) {
    deg %= 4;
    if(deg == 0) return mat;

    Matrix<mod_int> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2.get(j, mat.nrow - i - 1) = mat.get(i, j);
        }
    }

    return rot(mat2, deg-1);
}

template<typename mod_int>
Matrix<mod_int> transpose(Matrix<mod_int> mat) {
    Matrix<mod_int> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2.get(j, i) = mat.get(i, j);
        }
    }
    return mat;
}

template<typename mod_int>
mod_int determinant(Matrix<mod_int> mat) {
    assert(mat.nrow == mat.ncol);
    Matrix<mod_int> mat2 = mat;
    mod_int ret = 1;

    for(int i = 0; i < mat.ncol; i++) {
        int idx = -1;
        for(int j = i; j < mat.ncol; j++) {
            if(mat2.get(j, i) != 0) idx = j;
        }
        if(idx == -1) return 0;
        if(i != idx) {
            ret *= -1;
            swap(mat2.get(i), mat2.get(idx));
        }
        ret *= mat2.get(i, i);
        mod_int vv = mat2.get(i, i);
        for(int j = 0; j < mat.ncol; j++) {
            mat2.get(i, j) /= vv;
        }
        for(int j = i+1; j < mat.ncol; j++) {
            mod_int a = mat2.get(j, i);
            for(int k = 0; k < mat.ncol; k++) {
                mat2.get(j, k) -= mat2.get(i, k) * a;
            }
        }
    }

    return ret;
}

//ランクを返す
template<typename mod_int>
int Gauss_Jordan(Matrix<mod_int> &mat, bool is_extended = false) {
    int m = (int)mat.size(), n = (int)(mat.get(0)).size();
    int rank = 0;

    for(int col = 0; col < n; col++) {
        //拡大係数行列の定数ベクトルは掃きださない
        if(is_extended && col == n-1) break;

        //ピボットを探す
        int pivot = -1;

        for(int row = rank; row < m; row++) {
            if(mat.get(row, col) != 0) {
                pivot = row;
                break;
            }
        }

        if(pivot == -1) continue;
        swap(mat.get(pivot), mat.get(rank));
        auto inv = mat.get(rank, col).inv();
        for(int col2 = 0; col2 < n; col2++) {
            mat.get(rank, col2) = mat.get(rank, col2) * inv;
        }
        for(int row = 0; row < m; row++) {
            if(row != rank && mat.get(row, col)) {
                auto fac = mat.get(row, col);
                for(int col2 = 0; col2 < n; col2++) {
                    mat.get(row, col2) -= mat.get(row, col2) * fac;
                }
            }
        }
        ++rank;
    }

    return rank;
}

//rankを返す
//A:係数行列, B:定数ベクトル, res:解を入れるベクトル
template<typename mod_int>
int linear_equation(Matrix<mod_int> A, vector<mod_int> b, vector<mod_int> &res) {
    int m = (int)A.size(), n = (int)(A.get(0)).size();
    //拡大係数行列Mを作る
    Matrix<mod_int> M(m, n+1);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) M.get(i, j) = A.get(i, j);
        M.get(i, n) = b[i];
    }

    int rank = Gauss_Jordan(M, true);
    //解なしのチェック（掃き出した後rankよりも大きな行の定数ベクトルが0になってるかどうか）
    for(int row = rank; row < m; row++) if(M.get(row, n)) return -1;

    res.assign(n, 0);
    for(int i = 0; i < rank; i++) res[i] = M.get(i, n);
    return rank;
}

//解空間の次元Rを返す
//A:係数行列, B:定数ベクトル, ans:解の一つ, basis:解空間を定めるR個の独立な基底ベクトル
template<typename mod_int>
int extended_linear_equation(Matrix<mod_int> A, vector<mod_int> b, vector<mod_int> &ans, vector<vector<mod_int>> &basis) {
}

//+=を+で定義するようにする(-, *, /)も同じ