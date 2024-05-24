struct LinkCutTree {

    struct Node {
        Node* l = 0, * r = 0, * p = 0;
        int index;
        int sumz = 0;
        // lazy & 1 : reverse
        // lazy & 2 : deta
        int lazy = 0;
        
        void prepareDown() {
            if (lazy & 1) {
                ::std::swap(l, r);
                if (l) { l->lazy ^= 1; }
                if (r) { r->lazy ^= 1; }
            }
            lazy = 0;
        }
        void prepareUp() {
            sumz = 1;
            if(l) sumz += l->sumz;
            if(r) sumz += r->sumz;
        }
        Node** p_parentchild() {
            if(!p) return nullptr;
            if(p->l == this) return &p->l;
            else if(p->r == this) return &p->r;
            return nullptr;
        }
        void rotL() {
            Node* t = p;
            Node** parentchild_p = t->p_parentchild();
            if(parentchild_p){ *parentchild_p = this; }
            p = t->p;
            t->p = this;
            t->r = l;
            if(l) l->p = t;
            l = t;
        }
        void rotR() {
            Node* t = p;
            Node** parentchild_p = t->p_parentchild();
            if(parentchild_p){ *parentchild_p = this; }
            p = t->p;
            t->p = this;
            t->l = r;
            if(r) r->p = t;
            r = t;
        }
    };

    ::std::vector<Node> A;
    LinkCutTree(int n = 0) {
        A.resize(n);
        // TODO: BUILDER
        for(int i=0; i<n; i++) A[i].index = i;
        for(int i=0; i<n; i++) A[i].sumz = 1;
    }
    Node* at(int idx) {
        return &A[idx];
    }
    void splay(Node* c) {
        c->prepareDown();
        while(true) {
            Node* p = c->p;
            if(!p) break;
            Node* pp = p->p;
            if(pp) pp->prepareDown();
            p->prepareDown();
            c->prepareDown();
            if(p->l == c){
                if(!pp){ c->rotR(); }
                else if(pp->l == p){ p->rotR(); c->rotR(); }
                else if(pp->r == p){ c->rotR(); c->rotL(); }
                else{ c->rotR(); }
            }
            else if(p->r == c){
                if(!pp){ c->rotL(); }
                else if(pp->r == p){ p->rotL(); c->rotL(); }
                else if(pp->l == p){ c->rotL(); c->rotR(); }
                else{ c->rotL(); }
            }
            else break;
            if(pp) pp->prepareUp();
            p->prepareUp();
            c->prepareUp();
        }
        c->prepareUp();
    }
    void expose(int c) {
        auto Hc = at(c);
        auto p = Hc;
        while(p){ splay(p); p = p->p; }
        p = Hc;
        while(p->p){ p->p->r = p; p = p->p; }
        splay(Hc);
        Hc->r = nullptr;
        Hc->prepareUp();
    }
    void evert(int c) {
        auto Hc = at(c);
        expose(c);
        Hc->lazy ^= 1;
        Hc->prepareDown();
    }
    void link(int c, int r) {
        if(c == r) return;
        auto Hc = at(c);
        auto Hr = at(r);
        evert(c);
        expose(r);
        if(Hc->p) return;
        Hc->p = Hr;
    }
    // from - cを切り離す
    bool cut(int from, int c) {
        if(is_connected(from, c)) return false;
        evert(from);
        auto Hc = at(c);
        expose(c);
        if(!Hc->l) return false;
        Hc->l->p = nullptr;
        Hc->l = nullptr;
        Hc->prepareUp();
        return true;
    }
    int get_root_of(int c){
        expose(c);
        auto v = at(c);
        v->prepareDown();
        while(v->l){
            v = v->l;
            v->prepareDown();
        }
        splay(v);
        return v->index;
    }
    bool is_connected(int c, int d) {
        return get_root_of(c) == get_root_of(d);
    }
    int get_depth_of(int c){
        expose(c);
        auto v = at(c);
        v->prepareDown();
        if(!(v->l)) return 0;
        return v->l->sumz;
    }
    int la(int c, int d){
        if(d == 0) return c;
        expose(c);
        auto v = at(c);
        while(true){
            v->prepareDown();
            if(v->l){
                if(d < v->l->sumz){ v = v->l; continue; }
                d -= v->l->sumz;
            }
            if(d == 0){
                splay(v);
                return v->index;
            }
            d--;
            if(v->r){
                if(d < v->r->sumz){ v = v->r; continue; }
                d -= v->r->sumz;
            }
            splay(v);
            return -1;
        }
        return -1;
    }
    // c -> dのdist番目
    int jump(int c, int d, int dist) {
        if(!is_connected(c, d)) return -1;
        evert(d);
        return la(c, dist);
    }
    // unverified;
    int dist(int c, int d) {
        if(!is_connected(c, d)) return -1;
        evert(c);
        return get_depth_of(d);
    }
};

// expose -> connect the vertex to root and make the vertex root
// evert -> the vertex go up to root
// cut -> 切り離し
// is_connected -> 連結判定
// get_root_of -> 根を取得
// get_depth_of -> 深さを取得
// la, jump, dist -> いつも通り