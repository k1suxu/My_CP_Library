template<typename mod_int>
struct Matrix {
    vector<vector<mod_int>> mat;

    Matrix() = default;
    Matrix(int h, int w) : mat(vector<vector<mod_int>>(h, vector<mod_int>(w))) {}
    Matrix(int h, int w, mod_int d) : mat(vector<vector<mod_int>>(h, vector<mod_int>(w, d))) {}
    Matrix(vector<vector<mod_int>> vec) : mat(vec) {}
    
    vector<mod_int> operator[](const int i) const {return mat[i];}
    vector<mod_int>& operator[](const int i) {return mat[i];}

    int nrow = mat.size();
    int ncol = mat[0].size();

    void input() {
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                // mat[i][j].input();
                // cin >> mat[i][j]
            }
        }
        return;
    }

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
                mat[i][j] += other[i][j];
            }
        }
        return *this;
    }
    Matrix<mod_int>& operator-=(const Matrix<mod_int> other) {
        assert(nrow == other.nrow && ncol == other.ncol);
        for(int i = 0; i < nrow; i++) {
            for(int j = 0; j < ncol; j++) {
                mat[i][j] -= other[i][j];
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
                    mat2[i][j] += mat[i][k] * other[k][j];
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
                if(mat[i][j] != other[i][j]) flag = false;
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
};

template<typename mod_int>
Matrix<mod_int> Make_E(int SIZE) {
    Matrix<mod_int> ret(SIZE, SIZE, 0);
    for(int i = 0; i < SIZE; i++) ret[i][i] = 1;
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

//(???v??????deg?????]????)
template<typename mod_int>
Matrix<mod_int> rot(Matrix<mod_int> mat, long long deg) {
    deg %= 4;
    if(deg == 0) return mat;

    Matrix<mod_int> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][mat.nrow - i - 1] = mat[i][j];
        }
    }

    return rot(mat2, deg-1);
}

template<typename mod_int>
Matrix<mod_int> transpose(Matrix<mod_int> mat) {
    Matrix<mod_int> mat2(mat.ncol, mat.nrow);
    for(int i = 0; i < mat.nrow; i++) {
        for(int j = 0; j < mat.ncol; j++) {
            mat2[j][i] = mat[i][j];
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
            if(mat2[j][i] != 0) idx = j;
        }
        if(idx == -1) return 0;
        if(i != idx) {
            ret *= -1;
            swap(mat2[i], mat2[idx]);
        }
        ret *= mat2[i][i];
        mod_int vv = mat2[i][i];
        for(int j = 0; j < mat.ncol; j++) {
            mat2[i][j] /= vv;
        }
        for(int j = i+1; j < mat.ncol; j++) {
            mod_int a = mat2[j][i];
            for(int k = 0; k < mat.ncol; k++) {
                mat2[j][k] -= mat2[i][k] * a;
            }
        }
    }

    return ret;
}