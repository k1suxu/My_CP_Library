const ld EPS = 1e-10;

template<typename T>
struct Matrix {
    vector<vector<T>> mat;

    Matrix() = default;
    Matrix(int h, int w) : mat(vector<vector<T>>(h, vector<T>(w))) {}
    Matrix(int h, int w, T d) : mat(vector<vector<T>>(h, vector<T>(w, d))) {}
    
    vector<T> operator[](const int i) const {return mat[i];}
    vector<T>& operator[](const int i) {return mat[i];}

    int nrow = mat.size();
    int ncol = mat[0].size();

    void fill_all(T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] = d;
            }
        }
        return;
    }

    Matrix<T>& operator+=(const Matrix<T> other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] += other[i][j];
            }
        }
        return *this;
    }
    Matrix<T>& operator-=(const Matrix<T> other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] -= other[i][j];
            }
        }
        return *this;
    }
    Matrix<T>& operator*=(const Matrix<T> other) {
        assert(ncol == other.nrow);
        Matrix<T> mat2(nrow, other.ncol);

        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < other.ncol; j++) {
                for(int k = 0; k < ncol; k++) {
                    mat2[i][j] += mat[i][k] * other[k][j];
                }
            }
        }
        *this = mat2;
        return *this;
    }

    Matrix<T> operator+(const Matrix<T> other) const {return Matrix<T>(*this) += other;}
    Matrix<T> operator-(const Matrix<T> other) const {return Matrix<T>(*this) -= other;}
    Matrix<T> operator*(const Matrix<T> other) const {return Matrix<T>(*this) *= other;}
    
    bool operator==(const Matrix<T> other) {
        if(nrow != other.nrow || ncol != other.ncol) return false;

        bool flag = true;
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                if(mat[i][j] != other[i][j]) flag = false;
            }
        }

        return flag;
    }

    Matrix<T>& operator+=(const T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] += d;
            }
        }
        return *this;
    }
    Matrix<T>& operator-=(const T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] -= d;
            }
        }
        return *this;
    }
    Matrix<T>& operator*=(const T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] *= d;
            }
        }
        return *this;
    }
    Matrix<T>& operator/=(const T d) {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] /= d;
            }
        }
        return *this;
    }

    Matrix<T> operator+(const T d) const {return Matrix<T>(*this) += d;}
    Matrix<T> operator-(const T d) const {return Matrix<T>(*this) -= d;}
    Matrix<T> operator*(const T d) const {return Matrix<T>(*this) *= d;}
    Matrix<T> operator/(const T d) const {return Matrix<T>(*this) /= d;}

    void print() {
        cout << "Size : " << ncol << " * " << nrow << endl;
        for(int i = 0; i < nrow; i++) {
            cout << "{";
            for(int j = 0; j < ncol; j++) {
                cout << mat[i][j];
                if(j != ncol - 1) cout << ", ";
                else cout << "}\n";
            }
        }
        return;
    }
};

template<typename T>
Matrix<T> Make_E(int SIZE) {
    Matrix<T> ret(SIZE, SIZE, 0);
    for(int i = 0; i < SIZE; i++) ret[i][i] = 1;
    return ret;
}

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

//(???v??????deg?????]????)
template<typename T>
Matrix<T> rot(Matrix<T> mat, long long deg) {
    deg %= 4;
    if(deg == 0) return mat;

    Matrix<T> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][mat.nrow - i - 1] = mat[i][j];
        }
    }

    return rot(mat2, deg-1);
}

template<typename T>
Matrix<T> transpose(Matrix<T> mat) {
    Matrix<T> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][i] = mat[i][j];
        }
    }
    return mat;
}

template<typename T>
T determinant(Matrix<T> mat) {
    assert(mat.nrow == mat.ncol);
    Matrix<T> mat2 = mat;
    T ret = 1;

    for(int i = 0; i < mat.ncol; i++) {
        int idx = -1;
        for(int j = i; j < mat.ncol; j++) {
            if(mat2[j][i] != 0) idx = j;
        }
        if(idx == -1) return 0;
        if(i != idx) {
            ret *= -1;
            swap(mat2[i], mat2[idx]);
        }
        ret *= mat2[i][i];
        T vv = mat2[i][i];
        for(int j = 0; j < mat.ncol; j++) {
            mat2[i][j] /= vv;
        }
        for(int j = i+1; j < mat.ncol; j++) {
            T a = mat2[j][i];
            for(int k = 0; k < mat.ncol; k++) {
                mat2[j][k] -= mat2[i][k] * a;
            }
        }
    }

    return ret;
}

template<typename T>
void mat_input(Matrix<T>& mat) {
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            cin >> mat[i][j];
        }
    }
    return;
}
template<typename T>
void mat_output(Matrix<T> mat) {
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            cout << mat[i][j];
            if(j != mat.ncol - 1) cout << " ";
            else cout << "\n";
        }
    }
    return;
}


//+, - O(N^2)
//* O(N^3)
//^ O(N^3 log k) 行列累乗
// Time Complexity : O(N^3)

//TODO : 掃き出し法による、連立一次方程式の解。
//TODO : rank(階級)の計算
//TODO : inverse(逆行列)の計算

template<class T> int GaussJordan(Matrix<T> &A, bool is_extended = false) {
    int m = A.size(), n = A[0].size();
    int rank = 0;
    for (int col = 0; col < n; ++col) {
        // 拡大係数行列の場合は最後の列は掃き出ししない
        if (is_extended && col == n-1) break;

        // ピボットを探す
        int pivot = -1;
        T ma = EPS;
        for (int row = rank; row < m; ++row) {
            if (abs(A[row][col]) > ma) {
                ma = abs(A[row][col]);
                pivot = row;
            }
        }
        // ピボットがなかったら次の列へ
        if (pivot == -1) continue;

        // まずは行を swap
        swap(A[pivot], A[rank]);

        // ピボットの値を 1 にする
        auto fac = A[rank][col];
        for (int col2 = 0; col2 < n; ++col2) A[rank][col2] /= fac;

        // ピボットのある列の値がすべて 0 になるように掃き出す
        for (int row = 0; row < m; ++row) {
            if (row != rank && abs(A[row][col]) > EPS) {
                auto fac = A[row][col];
                for (int col2 = 0; col2 < n; ++col2) {
                    A[row][col2] -= A[rank][col2] * fac;
                }
            }
        }
        ++rank;
    }
    return rank;
}
template<class T> vector<T> linear_equation(Matrix<T> A, vector<T> b) {
    // extended
    int m = A.size(), n = A[0].size();
    Matrix<T> M(m, n + 1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) M[i][j] = A[i][j];
        M[i][n] = b[i];
    }
    int rank = GaussJordan(M, true);
    
    // check if it has no solution
    vector<T> res;
    for (int row = rank; row < m; ++row) if (abs(M[row][n]) > EPS) return res;

    // answer
    res.assign(n, 0);
    for (int i = 0; i < rank; ++i) res[i] = M[i][n];
    return res;
}


//+, - O(N^2)
//* O(N^3)
//^ O(N^3 log k) 行列累乗
// Time Complexity : O(N^3)

//TODO : inverse(逆行列)の計算