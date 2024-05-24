template<typename T>
struct Matrix : vector<vector<T>> {
    int nrow, ncol;

    Matrix() = default;
    Matrix(int h, int w, T e = 0) {
        (*this).resize(h, vector<T>(w, e));
        nrow = (*this).size();
        ncol = (nrow == 0? 0 : (*this)[0].size());
    }

    static Matrix<T> make_I(int n) {
        Matrix<T> mat(n, n);
        for (int i = 0; i < n; i++) mat[i][i] = 1;
        return mat;
    }

    void fill_all(T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] = d;
            }
        }
        return;
    }

    // O(d(M) log K): d(M)=O(H^3) with assertion `H = W`
    Matrix<T>& operator^=(long long k) {
        Matrix<T> bin_mat = (*this);
        assert(nrow == ncol);
        Matrix<T> mat = make_I(nrow);

        while(k) {
            if(k&1) mat *= bin_mat;
            bin_mat *= bin_mat;
            k >>= 1LL;
        }

        return (*this) = mat;
    }
    Matrix<T> operator^(const long long &d) const {return Matrix<T>(*this) ^= d;}
    Matrix<T> pow(const long long &d) const {return Matrix<T>(*this) ^= d;}

    // O(HW)
    Matrix<T>& operator+=(const Matrix<T> &other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] += other[i][j];
            }
        }
        return *this;
    }
    // O(HW)
    Matrix<T>& operator-=(const Matrix<T> &other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] -= other[i][j];
            }
        }
        return *this;
    }
    Matrix<T>& operator*=(const Matrix<T> &other) {
        assert(ncol == other.nrow);
        Matrix<T> mat(nrow, other.ncol);

        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < other.ncol; j++) {
                for(int k = 0; k < ncol; k++) {
                    mat[i][j] += (*this)[i][k] * other[k][j];
                }
            }
        }
        *this = mat;
        return *this;
    }

    Matrix<T> operator+(const Matrix<T> &other) const {return Matrix<T>(*this) += other;}
    Matrix<T> operator-(const Matrix<T> &other) const {return Matrix<T>(*this) -= other;}
    Matrix<T> operator*(const Matrix<T> &other) const {return Matrix<T>(*this) *= other;}

    bool operator==(const Matrix<T> &other) {
        if(nrow != other.nrow || ncol != other.ncol) return false;

        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                if((*this)[i][j] != other[i][j]) return false;
            }
        }
        
        return true;
    }

    Matrix<T>& operator+=(const T &d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] += d;
            }
        }
        return *this;
    }
    Matrix<T>& operator-=(const T &d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] -= d;
            }
        }
        return *this;
    }
    Matrix<T>& operator*=(const T &d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] *= d;
            }
        }
        return *this;
    }
    Matrix<T>& operator/=(const T &d) {
        assert(d != 0);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] /= d;
            }
        }
        return *this;
    }

    Matrix<T> operator+(const T &d) const {return Matrix<T>(*this) += d;}
    Matrix<T> operator-(const T &d) const {return Matrix<T>(*this) -= d;}
    Matrix<T> operator*(const T &d) const {return Matrix<T>(*this) *= d;}
    Matrix<T> operator/(const T &d) const {return Matrix<T>(*this) /= d;}

    void self_slice_range(const int &u, const int &d, const int &l, const int &r) {
        assert(u < d && l < r && 0 <= u && d <= nrow && 0 <= l && r <= ncol);
        Matrix<T> mat(d-u, r-l);
        for(int i = u; i < d; i++) for(int j = l; j < r; j++) mat[i-u][j-l] = (*this)[i][j];
        (*this) = mat;
        return;
    }
    Matrix<T> get_slice_range(const int &u, const int &d, const int &l, const int &r) {
        assert(u < d && l < r && 0 <= u && d <= nrow && 0 <= l && r <= ncol);
        Matrix<T> mat(d-u, r-l);
        for(int i = u; i < d; i++) for(int j = l; j < r; j++) mat[i-u][j-l] = (*this)[i][j];
        return mat;
    }

    void rot_clockwise(long long deg) {
        deg = (deg % 4 + 4) % 4;

        for(int D = 0; D < deg; D++) {
            Matrix<T> mat(ncol, nrow);
            for(int i = 0; i < nrow; i++) {
                for(int j = 0; j < ncol; j++) {
                    mat[j][nrow-i-1] = (*this)[i][j];
                }
            }
            (*this) = mat;
        }

        return;
    }
    void rot_counter_clockwise(long long deg) {
        deg = (4-(deg%4+4)%4)%4;
        return rot_clockwise(deg);
    }

    void transpose() {
        Matrix<T> mat(ncol, nrow);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[j][i] = (*this)[i][j];
            }
        }
        (*this) = mat;
        return;
    }
    
    // 上三角行列にして対角成分の総積
    T determinant() {
        assert(nrow == ncol);
        T ret = 1;

        for(int i = 0; i < ncol; i++) {
            int idx = -1;
            for(int j = i; j < ncol; j++) {
                if((*this)[j][i] != 0) idx = j;
            }
            if(idx == -1) return 0;
            if(i != idx) {
                ret *= -1;
                swap((*this)[i], (*this)[idx]);
            }
            ret *= (*this)[i][i];
            T vv = (*this)[i][i];
            for(int j = 0; j < ncol; j++) {
                (*this)[i][j] /= vv;
            }
            for(int j = i+1; j < ncol; j++) {
                T a = (*this)[j][i];
                // for(int k = 0; k < ncol; k++) {
                for(int k = i; k < ncol; k++) {
                    (*this)[j][k] -= (*this)[i][k] * a;
                }
            }
        }

        return ret;
    }

    void print() {
        cout << "Size : " << nrow << " * " << ncol << endl;
        for(int i = 0; i < nrow; i++) {
            cout << "{";
            for(int j = 0; j < ncol; j++) {
                cout << (*this)[i][j];
                if(j != ncol - 1) cout << ", ";
                else cout << "}\n";
            }
        }
        return;
    }

    void input() {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                cin >> (*this)[i][j];
            }
        }
    }
    void output() {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                if(j != ncol - 1) cout << (*this)[i][j] << " ";
                else cout << (*this)[i][j] << "\n";
            }
        }
    }
};

template<typename T>
int Gauss_Jordan(Matrix<T> &mat, bool is_extended = false) {
    int m = mat.nrow, n = mat.ncol;
    int rank = 0;

    for(int col = 0; col < n; col++) {
        if(is_extended && col == n-1) break;

        int pivot = -1;

        for(int row = rank; row < m; row++) {
            if(mat[row][col] != 0) {
                pivot = row;
                break;
            }
        }

        if(pivot == -1) continue;
        swap(mat[pivot], mat[rank]);
        auto inv = T(1)/mat[rank][col];
        for(int col2 = 0; col2 < n; col2++) {
            mat[rank][col2] = mat[rank][col2] * inv;
        }
        for(int row = 0; row < m; row++) {
            if(row != rank && mat[row][col] != 0) {
                auto fac = mat[row][col];
                for(int col2 = 0; col2 < n; col2++) {
                    mat[row][col2] -= mat[rank][col2] * fac;
                }
            }
        }
        ++rank;
    }

    return rank;
}

// 戻り値は[A|b]拡大係数行列のrank
template<typename T>
int linear_equation(Matrix<T> A, vector<T> b, vector<T> &res) {
    assert(A.nrow == (int)b.size());
    int m = A.nrow, n = A.ncol;

    Matrix<T> M(m, n+1);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) M[i][j] = A[i][j];
        M[i][n] = b[i];
    }

    int rank = Gauss_Jordan(M, true);
    for(int row = rank; row < m; row++) if(M[row][n] != 0) return -1;

    res.assign(n, 0);
    for(int i = 0; i < rank; i++) res[i] = M[i][n];
    return rank;
}

// 戻り値:基底の個数(=解空間の次元), ans:解の一つ, basis:解空間の基底ベクトル
template<typename T>
int extended_linear_equation(Matrix<T> coef, vector<T> b, vector<T> &ans, vector<vector<T>> &basis_vector) {
    int m = coef.size();
    int n = coef[0].size()+1;
    Matrix<T> A(m, n);
    assert(m == (int)b.size());
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n-1; j++) A[i][j] = coef[i][j];
        A[i][n-1] = b[i];
    }

    vector<int> is_used_ctr(m, false); // i行目が主成分を含むかどうか
    vector<int> basis;
    vector<int> var2ctr(n-1, -1);

    for(int i = 0; i < n-1; i++) {
        int row = -1;
        for(int j = 0; j < m; j++) {
            if(!is_used_ctr[j] && A[j][i] != 0) {
                is_used_ctr[j] = true;
                row = j;
                break;
            }
        }

        if(row == -1) {
            basis.push_back(i);
        }else {
            var2ctr[i] = row;
            T inv = T(1)/A[row][i];
            for(int j = 0; j < n; j++) A[row][j] *= inv;
            for(int j = 0; j < m; j++) {
                if(j != row && A[j][i] != 0) {
                    T fac = A[j][i];
                    for(int k = 0; k < n; k++) {
                        A[j][k] -= A[row][k] * fac;
                    }
                }
            }
        }
    }

    for(int j = 0; j < m; j++) {
        if(!is_used_ctr[j] && A[j].back() != 0) {
            return -1;
        }
    }

    auto calc_basis = [&](vector<T> &solution, int idx_row1) -> void {
        for(int i = 0; i < n-1; i++) {
            if(var2ctr[i] != -1) {
                int row = var2ctr[i];
                if(idx_row1 != -1) {
                    assert(idx_row1 < n);
                    solution[i] = - A[row][idx_row1];
                }else {
                    solution[i] = A[row].back();
                }
            }
        }
        if(idx_row1 != -1) {
            solution[idx_row1] = 1;
        }
    };

    ans.resize(n-1);
    calc_basis(ans, -1);
    basis_vector.resize((int)basis.size(), vector<T>(n-1));
    for(int i = 0; i < (int)basis.size(); i++) calc_basis(basis_vector[i], basis[i]);

    return (int)basis.size();
}

// 戻り値:逆行列が存在するかのboolean変数
template<typename T>
bool cal_mat_inv(const Matrix<T> mat, Matrix<T>& inversed_mat) {
    assert(mat.nrow == mat.ncol);
    Matrix<T> mat_E(mat.nrow, 2*mat.ncol, 0);
    for(int i = 0; i < mat.nrow; i++) for(int j = 0; j < mat.ncol; j++) mat_E[i][j] = mat[i][j];
    for(int i = 0; i < mat.nrow; i++) mat_E[i][i+mat.ncol] = 1;
    Gauss_Jordan(mat_E, false);
    if(mat_E.get_slice_range(0, mat.nrow, 0, mat.ncol) != Matrix<T>::make_I(mat.nrow)) return false;
    inversed_mat = mat_E.get_slice_range(0, mat.nrow, mat.ncol, 2*mat.ncol);
    return true;
}

// 行列積の高速化、二次元配列を一次元にすることによる配列アクセス処理の高速化など
// https://qiita.com/nocturnality/items/92b3cf7d7fc5dab4a3a7
// 逆元が必ずしも存在するとは限らない環における行列式
// https://noshi91.hatenablog.com/entry/2020/11/28/115621

// [verifier]
// Product(operator*) : https://judge.yosupo.jp/submission/168166
// Determinant        : https://judge.yosupo.jp/submission/168357
// Linear Equations   : https://judge.yosupo.jp/submission/168356
// Inverse            : https://judge.yosupo.jp/submission/168355