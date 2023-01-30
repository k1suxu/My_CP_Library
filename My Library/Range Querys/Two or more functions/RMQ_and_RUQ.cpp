struct RMQ_RUQ{
    int n;
    vector<int> dat,laz;
    auto fx = [](int x, int y) {
        return min(x, y);
    }
    const int e=0;
    RMQ_RUQ(){}
    RMQ_RUQ(int n_){init(n_);}
    void init(int n_){
        n=1;
        while(n<n_) n*=2;
        dat.clear();
        dat.resize(2*n-1,e);
        laz.clear();
        laz.resize(2*n-1,-1);
    }
    inline void eval(int len,int k){
        if(laz[k]<0) return;
        if(k*2+1<n*2-1){
        laz[k*2+1]=laz[k];
        laz[k*2+2]=laz[k];
        }
        dat[k]=laz[k];
        laz[k]=-1;
    }
    int update(int a,int b,int x,int k,int l,int r){
        eval(r-l,k);
        if(r<=a||b<=l) return dat[k];
        if(a<=l&&r<=b) return laz[k]=x;
        eval(r-l,k);
        dat[k]=f(update(a,b,x,k*2+1,l,(l+r)/2), update(a,b,x,k*2+2,(l+r)/2,r));
        return dat[k];
    }
    int query(int a,int b,int k,int l,int r){
        eval(r-l,k);
        if(r<=a||b<=l) return e;
        if(a<=l&&r<=b) return dat[k];
        int vl=query(a,b,k*2+1,l,(l+r)/2);
        int vr=query(a,b,k*2+2,(l+r)/2,r);
        return f(vl,vr);
    }
    void update(int a,int b,int x){
        update(a,b,x,0,0,n);
    }
    int query(int a,int b){
        return query(a,b,0,0,n);
    }
};