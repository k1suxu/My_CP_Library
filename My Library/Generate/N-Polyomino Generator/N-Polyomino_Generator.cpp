struct PolyominoState {
  std::vector<std::vector<int>> number, field;
  PolyominoState(int n, int m):number(n, std::vector<int>(m,-1)),field(n, std::vector<int>(m,0)){}

  void minimize(){
    if(field.size() == 0) return;
    if(field[0].size() == 0) return;
    std::vector<int> w(field[0].size(), 0), h(field.size(), 0);
    
    for(int i=0; i<field.size(); i++){
      for(int j=0; j<field[i].size(); j++){
        if(field[i][j] == 1){
          w[j] = 1;
          h[i] = 1;
        }
      }
    }

    int width = 0, height = 0;
    for(int i=0; i<w.size(); i++) if(w[i]==1) width++;
    for(int i=0; i<h.size(); i++) if(h[i]==1) height++;
    
    std::vector<std::vector<int>> new_field(height,std::vector<int>(width,0));
    for(int i=0,ii=0; i<field.size(); i++){
      if(h[i] == 0) continue;
      for(int j=0,jj=0; j<field[i].size(); j++){
        if(w[j] == 0) continue;
        new_field[ii][jj] = field[i][j];
        jj++;
      }
      ii++;
    }

    number.clear();
    field = new_field;
  }
};
class GeneratePolyomino {
  int N;
  
  //設置可能かど�?かを確�?
  bool check(const PolyominoState& s, int y, int x){
    int n = s.number.size();
    int m = s.number[0].size();
    if(y<0 || n<=y || x<0 || m<=x) return false;
    if(y==0){
      if(m/2 <= x && s.number[y][x] == -1) return true;
      return false;
    }else{
      if(s.number[y][x] == -1) return true;
      return false;
    }
  }
public:
  GeneratePolyomino(int N):N(N){}
  
  std::vector<PolyominoState> generate(){
    std::vector<PolyominoState> ret;

    PolyominoState st(N, 2*N-1);
    st.number[0][N-1] = 1;
    if(N>1) st.number[0][N] = 2;
    if(N>1) st.number[1][N-1] = 3;
    st.field[0][N-1] = 1;

    if(N == 1){
      ret.push_back(st);
      return ret;
    }
    
    std::queue<PolyominoState> que;
    que.push(st);

    while(!que.empty()){
      st = que.front(); que.pop();
      int n_last = -1;
      int f_last = -1;
      int f_num = 0;
      for(int i=0; i<st.number.size(); i++){
        for(int j=0; j<st.number[i].size(); j++){
          if(st.field[i][j] == 0) n_last = std::max(n_last, st.number[i][j]);
          if(st.field[i][j] == 1){
            f_last = std::max(f_last, st.number[i][j]);
            f_num++;
          }
        }
      }

      if(f_num==N){
        st.minimize();
        ret.push_back(st);
        continue;
      }
      
      for(int i=0; i<st.number.size(); i++){
        for(int j=0; j<st.number[i].size(); j++){
          if(st.number[i][j] == -1) continue;
          if(st.field[i][j] == 1) continue;
          if(st.number[i][j] < f_last) continue;
          
          PolyominoState tmp = st;
          tmp.field[i][j] = 1;

          int number = n_last+1;
          if(check(tmp, i-1, j)) tmp.number[i-1][j] = number++;
          if(check(tmp, i, j-1)) tmp.number[i][j-1] = number++;
          if(check(tmp, i, j+1)) tmp.number[i][j+1] = number++;
          if(check(tmp, i+1, j)) tmp.number[i+1][j] = number++;

          que.push(tmp);
        }
      }
    }
    return ret;
  }
};

//referrence
/*  

****generato N-Polyominos****
    GeneratePolyomino poly(N);
    vector<PolyominoState> ps = poly.generate();

*/