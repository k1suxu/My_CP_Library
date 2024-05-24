//mapを使った実装: https://satanic0258.github.io/snippets/data-structure/SegmentMap.html
//の方が定数倍が早い

// Description: 区間をsetで管理するデータ構造(なお実装はmap)．各クエリO(log区間数)． 
// #### attention! : [l, r] ( include r, not [l, r) )
class SegmentMap : public std::map<signed, signed> {
private:
    bool flagToMergeAdjacentSegment;
public:
    // if merge [l, c] and [c+1, r], set flagToMergeAdjacentSegment to true
    SegmentMap(bool flagToMergeAdjacentSegment) :
        flagToMergeAdjacentSegment(flagToMergeAdjacentSegment) {}
    // __exist -> iterator pair(l, r) (contain p)
    // noexist -> map.end()
    auto get(signed p) const {
        auto it = upper_bound(p);
        if (it == begin() || (--it)->second < p) return end();
        return it;
    }
    // insert segment [l, r]
    void insert(signed l, signed r) {
        auto itl = upper_bound(l), itr = upper_bound(r + flagToMergeAdjacentSegment);
        if (itl != begin()) {
            if ((--itl)->second < l - flagToMergeAdjacentSegment) ++itl;
        }
        if (itl != itr) {
            l = std::min(l, itl->first);
            r = std::max(r, std::prev(itr)->second);
            erase(itl, itr);
        }
        (*this)[l] = r;
    }
    // remove segment [l, r]
    void remove(signed l, signed r) {
        auto itl = upper_bound(l), itr = upper_bound(r);
        if (itl != begin()) {
            if ((--itl)->second < l) ++itl;
        }
        if (itl == itr) return;
        int tl = std::min(l, itl->first), tr = std::max(r, std::prev(itr)->second);
        erase(itl, itr);
        if (tl < l) (*this)[tl] = l - 1;
        if (r < tr) (*this)[r + 1] = tr;
    }
    // Is p and q in same segment?
    bool same(signed p, signed q) const {
        const auto&& it = get(p);
        return it != end() && it->first <= q && q <= it->second;
    }
};










struct Segment_Map {
    set<pair<long long, long long>> seg;

    Segment_Map() = default;
    Segment_Map(vector<pair<long long, long long>> init_segs) {
        for(auto e : init_segs) insert(e);
    }

    auto get(long long i) {
        return seg.lower_bound(make_pair(i, -INF));
    }
    pair<long long, long long> get_seg_lr(long long i) {
        return *get(i);
    }

    void insert(long long l, long long r) {
        auto itl = get(l), itr = get(r);
        
        if(itl != seg.begin() && (--it)->second < l) itl++;
        if(itr->first == r) itr++;

        l = min(l, itl->first);
        if(itr != seg.begin()) r = max(r, prev(itr)->second);

        if(itl != itr) seg.erase(itl, itr);
        seg.emplace(l, r);
    }

    //実装未完了
    void remove(long long l, long long r) {
        auto itl = get(l);

        if(itl != seg.begin() && (--itl)->second < l) itl++;

        seg.erase(itl);
        seg.insert(itl->first, min(itl->second, l));

        itl = get(l);
        auto itr = get(r);

        if(itr != seg.begin() && (--itr)->second >= r) itr++;

        if(itl != itr) seg.erase(itl, itr);
    }

    bool same(long long i, long long j) {
        return get(i) == get(j);
    }
};