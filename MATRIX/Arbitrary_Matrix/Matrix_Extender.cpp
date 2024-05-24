template<typename T>
Matrix<T> mat_make_I(const int SIZE) {
    Matrix<T> ret(SIZE, SIZE, 0);
    for(int i = 0; i < SIZE; i++) ret[i][i] = 1;
    return ret;
}

// O(d(M) log K): d(M)=O(H^3) with assertion `H = W`
template<typename T>
Matrix<T> mat_pow(Matrix<T> mat, long long k) {
    assert(mat.nrow == mat.ncol);
    Matrix<T> mat2 = Make_E(mat.nrow);

    while(k) {
        if(k&1) mat2 *= mat;
        mat *= mat;
        k >>= 1LL;
    }

    return mat2;
}

// mat自体は不変
template<typename T>
Matrix<T> rot_clockwise(Matrix<T> mat, long long deg) {
    deg = (deg % 4 + 4) % 4;
    if(deg == 0) return mat;

    Matrix<T> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][mat.nrow-i-1] = mat[i][j];
        }
    }

    return rot(mat2, deg-1);
}
// mat自体は不変
template<typename T>
Matrix<T> rot_counter_clockwise(Matrix<T> mat, long long deg) {
    deg = (4-(deg%4+4)%4)%4;
    return rot_clockwise(mat, deg);
}

template<typename T>
Matrix<T> transpose(Matrix<T> mat) {
    Matrix<T> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][i] = mat[i][j];
        }
    }
    return mat2;
}

// 上三角行列にして対角成分の総積
template<typename T>
T determinant(Matrix<T> mat) {
    assert(mat.nrow == mat.ncol);
    T ret = 1;

    for(int i = 0; i < mat.ncol; i++) {
        int idx = -1;
        for(int j = i; j < mat.ncol; j++) {
            if(mat[j][i] != 0) idx = j;
        }
        if(idx == -1) return 0;
        if(i != idx) {
            ret *= -1;
            swap(mat[i], mat[idx]);
        }
        ret *= mat[i][i];
        T vv = mat[i][i];
        for(int j = 0; j < mat.ncol; j++) {
            mat[i][j] /= vv;
        }
        for(int j = i+1; j < mat.ncol; j++) {
            T a = mat[j][i];
            // for(int k = 0; k < mat.ncol; k++) {
            for(int k = i; k < mat.ncol; k++) {
                mat[j][k] -= mat[i][k] * a;
            }
        }
    }

    return ret;
}

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
    if(mat_E.get_slice_range(0, mat.nrow, 0, mat.ncol) != mat_make_I<T>(mat.nrow)) return false;
    inversed_mat = mat_E.get_slice_range(0, mat.nrow, mat.ncol, 2*mat.ncol);
    return true;
}

// 行列積の高速化、二次元配列を一次元にすることによる配列アクセス処理の高速化など
// https://qiita.com/nocturnality/items/92b3cf7d7fc5dab4a3a7
// 逆元が必ずしも存在するとは限らない環における行列式
// https://noshi91.hatenablog.com/entry/2020/11/28/115621

// [verifier]
// Product(operator*) : https://judge.yosupo.jp/submission/168166
// Determinant        : https://judge.yosupo.jp/submission/168167
// Linear Equations   : https://judge.yosupo.jp/submission/168168
// Inverse            : https://judge.yosupo.jp/submission/168169