#ifndef LABS_POSITION_H
#define LABS_POSITION_H

#include <vector>

class Position {
private:
    int markers;
    std::vector<int> out;
public:
    Position() : markers{} {}
    explicit Position(const int tr) : markers{}, out(tr) {}
    explicit Position(std::vector<int>&& tr) : markers{}, out(std::move(tr)) {}
    [[nodiscard]] auto get_num() const { return markers; }
    [[nodiscard]] auto& get_out() const { return out; }
    void add_markers(const int n) { markers += n; }
    void remove_markers(const int n) { markers -= n; }
    void update_transition(const int ind, const int num) { out[ind] += num; }
};


#endif //LABS_POSITION_H
