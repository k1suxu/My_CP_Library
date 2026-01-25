template<typename DF, typename DS>
int sign(const pair<DF, DS> &p) {
    if (p.first == 0 && p.second == 0) return 0;
    else if (p.second < 0 || (p.second <= 0 && p.first > 0)) return -1;
    return 1;
}
template<typename DF, typename DS>
bool cmp(const pair<DF, DS> &p, const pair<DF, DS> &q) {
    return (sign(p) != sign(q) ? sign(p) < sign(q) : p.first * q.second - p.second * q.first > 0);
}
// (-pi,pi]
// sort(v.begin(), v.end(), cmp);