struct SplayNode{
    SplayNode *left, *right, *parent;
    int size, value;
    //TODO
    int minimum;

    SplayNode(){
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        size = 1;
    }

    void rotate(){
        SplayNode *pp, *p, *c;
        p = this->parent;
        pp = p->parent;

        if(p->left == this){
            c = this->right;
            this->right = p;
            p->left = c;
        }
        else{
            c = this->left;
            this->left = p;
            p->right = c;
        }
        if(pp && pp->left == p)pp->left = this;
        if(pp && pp->right == p)pp->right = this;
        this->parent = pp;
        p->parent = this;
        if(c)c->parent = p;

        p->update();
        this->update();
    }

    int state(){
        if(!this->parent)return 0;
        if(this->parent->left == this)return 1;
        if(this->parent->right == this)return -1;
        return 0;
    }
    
    void splay(){
        while(this->state() != 0){
            if(this->parent->state() == 0){
                this->rotate();
            }
            else if(this->state() == this->parent->state()){
                this->parent->rotate();
                this->rotate();
            }else{
                this->rotate();
                this->rotate();
            }
        }
    }
    
    void update() {
        this->size = 1;
        //TODO
        this->minimum = value;
        if(this->left) {
            node_data_merge(this->left);
        }
        if(this->right) {
            node_data_merge(this->right);
        }
    }

    void node_data_merge(SplayNode *v) {
        this->size += v->size;
        //TODO
        chmin(this->minimum, v->minimum);
    }

    void change_value(int x) {
        this->value = x;
        this->update();
    }
};

SplayNode *get(int ind, SplayNode *root){
    SplayNode *now = root;
    while(true){
        int lsize = now->left ? now->left->size : 0;

        if(ind < lsize){
            now = now->left;
        }
        if(ind == lsize){
            now->splay();
            return now;
        }
        if(ind > lsize){
            now = now->right;
            ind = ind - lsize - 1;
        }
    }
}

typedef SplayNode Node;

Node *merge(Node *lroot, Node *rroot){
    if(!lroot)return rroot;
    if(!rroot)return lroot;
    lroot = get(lroot->size-1, lroot);
    lroot->right = rroot;
    rroot->parent = lroot;
    lroot->update();
    return lroot;
}
pair<Node*, Node*> split(int left_cnt, Node *root){
    if(left_cnt == 0)return{nullptr, root};
    if(left_cnt == root->size)return {root, nullptr};
    root = get(left_cnt, root);
    Node *lroot, *rroot;
    lroot = root->left;
    rroot = root;
    rroot->left = nullptr;
    lroot->parent = nullptr;
    rroot->update();
    return {lroot, rroot};
}
Node *insert(int ind, Node *node, Node *root){
    auto trees = split(ind, root);
    Node* lroot = trees.first;
    Node* rroot = trees.second;
    return merge( merge(lroot, node), rroot);
}
pair<Node*, Node*> erase(int ind, Node *root){
    root = get(ind, root);
    Node* lroot = root->left;
    Node* rroot = root->right;
    if(lroot)lroot->parent = nullptr;
    if(rroot)rroot->parent = nullptr;
    root->left = nullptr;
    root->right = nullptr;
    root->update();
    return {merge(lroot, rroot), root};
}

Node* shift(int l, int r, Node* root){
    auto tmp = erase(r-1, root);
    root = tmp.first;
    Node* node = tmp.second;
    return insert(l, node, root);
}

//[l, r)��len�������z�V�t�g������
// Node* circular_shift(int l, int r, int len, Node* root) {
// split, split, merge, merge??
// }

pair<Node*, int> range_fold(int l, int r, Node* root){
    Node *lroot, *croot, *rroot;
    auto tmp = split(r, root);
    rroot = tmp.second;
    tmp = split(l, tmp.first);
    lroot = tmp.first;
    croot = tmp.second;
    //TODO
    int ans = croot->minimum;
    return {merge(merge(lroot, croot), rroot), ans};
}

//undebugged
//�S�Ă̗v�f������������Ă���O��
Node* update_value(int i, int x, Node* root) {
    root = get(i, root);
    root->change_value(x);
    return root;
}
/*�������͂���Ȋ���
vector<Node> node(n);

FOR(n-1) {
    node[i].parent = &node[i+1];
    node[i+1].left = &node[i];
    node[i+1].update();
}

FOR(n) {
    node[i].change_value(a[i]);
}

SplayNode *root = &node[n-1];
*/

//root = shift, merge, �݂����ɂ���悤�ɋC��t���邱�ƁI�I





**************************************
//������Ȃ������Ƃ��p(�g���̂͏�̂��)
**************************************
struct SplayNode {
    SplayNode *left, *right, *parent;
    int size, value;
    //�C�ӂɒǉ�

    SplayNode() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        size = 1;
    }

    void rotate() {
        SplayNode *pp, *p, *c;
        p = this->parent;
        pp = p->parent;

        if(p->left == this) {
            c = this->right;
            this->right = p;
            p->left = c;
        }else {
            c = this->left;
            this->left = p;
            p->right = c;
        }

        if(pp && pp->left == p) pp->left = this;
        if(pp && pp->right == p) pp->right = this;
        this->parent = pp;
        p->parent = this;
        if(c) c->parent = p;

        p->update();
        this->update();
    }

    int state() {
        if(!this->parent) return 0; //�e����
        if(this->parent->left == this) return 1; //�e�̍���������
        if(this->parent->right == this) return -1; //�e�̉E��������
        return 0;
    }

    void splay() {
        //���������ɂȂ�܂ŏ�ɍs��
        while(this->state() != 0) {
            if(this->parent->state() == 0) {
                this->rotate();
            }else if(this->state() == this->parent->state()) {
                //���������ɘA�Ȃ��Ă���Ƃ�(�e������)
                this->parent->rotate();
                this->rotate();
            }else {
                //���Z���Ⴄ�Ƃ�(����������)
                this->rotate();
                this->rotate();
            }
        }
    }

    void update() {
        //�����o�ϐ��ɂ��낢�뎝�����Ă�Ƃ��͂����ōX�V
        this->size = 1;
        if(this->left) {
            this->size += this->left->size;
        }
        if(this->right) {
            this->size += this->left->size;
        }
    }

    // void update() {
    //     //�����o�ϐ��ɂ��낢�뎝�����Ă�Ƃ��͂����ōX�V
    //     this->size = 1;
    //     if(this->left) node_merge(this->left);
    //     if(this->right) node_merge(this->right);
    // }

    // void node_merge(SplayNode *v) {
    //     this->size += v->size;
    //     //�C�ӂɒǉ�
    // }
};

typedef SplayNode Node;

//0-indexed
//�I�������i�����ɗ���
Node *get(int i, Node *root) {
    Node *now = root;
    while(true) {
        int lsize = now->left? now->left->size : 0;
        
        if(i < lsize) {
            now = now->left;
        }
        if(i == lsize) {
            now-> splay();
            return now;
        }
        if(i > lsize) {
            now = now->right;
            i = i-lsize-1;
        }
    }
}

//merge��̍���Ԃ�
Node *merge(Node *lroot, Node *rroot) {
    if(!lroot) return rroot;
    if(!rroot) return lroot;
    lroot = get(lroot->size-1, lroot);
    lroot->right = rroot;
    rroot->parent = lroot;
    lroot->update();
    return lroot;
}

//split��̍��̖؂̃T�C�Y��left_size�ɂȂ�悤��split
//�߂�l�͍��؂̍��ƉE�؂̍�
pair<Node*, Node*> split(int left_size, Node *root) {
    if(left_size == 0) return make_pair(nullptr, root);
    if(left_size == root->size) return {root, nullptr};

    root = get(left_size, root);
    Node *lroot, *rroot;
    lroot = root->left;
    rroot = root;
    rroot-> left = nullptr;
    lroot->parent = nullptr;
    rroot->update();
    return make_pair(lroot, rroot);
}

//0-indexed��i�Ԗڂ̌��Ԃɓ����
Node* insert(int i, Node *node, Node *root) {
    pair<Node*, Node*> trees = split(i, root);
    Node *lroot = trees.first;
    Node *rroot = trees.second;
    return merge(merge(lroot, node), rroot);
}

//i�Ԗڂ̗v�f���폜����
//�߂�l�͍폜��̖؂̍� + �폜���ꂽ�v�f�̍s����(�؂̍�)
pair<Node*, Node*> remove(int i, Node *root) {
    root = get(i, root);
    Node *lroot = root->left;
    Node *rroot = root->right;
    if(lroot) lroot->parent = nullptr;
    if(rroot) rroot->parent = nullptr;
    root->update();
    return make_pair(merge(lroot, rroot), root);
}

//0-indexed[l, r](���)��left_circular_shift�����
Node* shift(int l, int r, Node* root) {
    pair<Node*, Node*> tmp = remove(r, root);
    root = tmp.first;
    Node* node = tmp.second;
    return insert(l, node, root);
}

//rmq�̂���{�ЂȌ`
//0-indexed[l,r)(���J���)�̃N�G���̓�����Ԃ�
//����I�����̍��Ɠ�����Ԃ�
pair<Node*, int> rmq(int l, int r, Node *root) {
    Node *lroot, *croot, *rroot;
    pair<Node*, Node*> tmp = split(r, root);
    rroot = tmp.second;
    tmp = split(l, tmp.first);
    lroot = tmp.first;
    croot = tmp.second;
    int ans = croot->min_val;
    return make_pair(merge(merge(lroot, croot), rroot), ans);
}