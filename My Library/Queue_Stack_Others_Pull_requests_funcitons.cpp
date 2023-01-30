template<typename T>
T pull(queue<T>& que) {
    T temp = que.front();
    que.pop();
    return temp;
}
template<typename T>
T pull(stack<T>& que) {
    T temp = que.top();
    que.pop();
    return temp;
}
template<typename T>
T pull_front(deque<T>& deq) {
    T temp = deq.front();
    deq.pop_front();
    return temp;
}
template<typename T>
T pull_back(deque<T>& deq) {
    T temp = deq.back();
    deq.pop_back();
    return temp;
}
template<typename T>
T pull(priority_queue<T, vector<T>, greater<T>>& pq) {
    T temp = pq.top();
    pq.pop();
    return temp;
}
template<typename T>
T pull(priority_queue<T, vector<T>, less<T>>& pq) {
    T temp = pq.top();
    pq.pop();
    return temp;
}