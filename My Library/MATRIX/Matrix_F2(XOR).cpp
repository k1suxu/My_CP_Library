//+ O(HW/64)
//* O(HW^2)
//bitset -> O(HW^2/64)
//F_2Matrix

const int MAX_ROW = 510;
const int MAX_COL = 510;

struct Bit_Matrix {
    int H, W;
    bitset<MAX_COL> val[MAX_ROW];
    Bit_Matrix(int m, int n) : H(m), W(n) {}

    inline bitset<MAX_COL> operator[](int i) const {return val[i];}
    inline bitset<MAX_COL>& operator[](int i) {return val[i];}

    Bit_Matrix& operator+=(const Bit_Matrix other) {
        assert((*this).H == other.H);
        assert((*this).W == other.W);
        for(int i = 0; i < (*this).H; i++) (*this)[i] ^= other[i];
        return *this;
    }
    Bit_Matrix& operator*=(const Bit_Matrix other) {
        assert((*this).H == other.W);
        Bit_Matrix R((*this).H, other.W);
        Bit_Matrix tB(other.W, other.H);
        for(int i = 0; i < tB.H; i++) for(int j = 0; j < tB.W; j++) tB[i][j] = other[j][i];
        for(int i = 0; i < R.H; i++) for(int j = 0; j < R.W; j++) R[i][j] = (((*this)[i] & tB[j]).count() & 1);
        
        *this = R;
        return *this;
    }
    Bit_Matrix operator+(const Bit_Matrix other) const {return Bit_Matrix(*this) += other;}
    Bit_Matrix operator*(const Bit_Matrix other) const {return Bit_Matrix(*this) *= other;}
};

//rank(A)‚ğ•Ô‚µAA‚ğ‘|‚«o‚µŒã‚É•ÏŒ`‚·‚é
int Gauss_Jordan(Bit_Matrix &A, bool is_extended = false) {
    int rank = 0;

    for(int col = 0; col < A.W; col++) {
        if(is_extended && col == A.W - 1) break;
        int pivot = -1;
        for(int row = rank; row < A.H; row++) {
            if(A[row][col]) {
                pivot = row;
                break;
            }
        }

        if(pivot == -1) continue;

        swap(A[pivot], A[rank]);

        for(int row = 0; row < A.H; row++) {
            if(row != rank && A[row][col]) A[row] ^= A[rank];
        }
        ++rank;
    }

    return rank;
}

//rank(A+b)‚ğ•Ô‚µAres‚É•û’ö®‚Ì‰ğ‚ğ“ü‚ê‚é
int linear_equatin(Bit_Matrix A, vector<int> b, vector<int>& res) {
    int m = A.H, n = A.W;
    //Šg‘åŒW”s—ñ
    Bit_Matrix M(m, n+1);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) M[i][j] = A[i][j];
        M[i][n] = b[i];
    }
    int rank = Gauss_Jordan(M, true);

    //‰ğ‚È‚µ
    for(int row = rank; row < m; row++) if(M[row][n]) return -1;
    
    //‰ğ‚ ‚è
    res.assign(n, 0);
    for(int i = 0; i < rank; i++) res[i] = M[i][n];
    return rank;
}