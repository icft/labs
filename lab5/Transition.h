#ifndef LABS_TRANSITION_H
#define LABS_TRANSITION_H

#include <vector>

class Transition {
private:
    std::vector<int> inp;
    std::vector<int> out;
    static void update(std::vector<int>& vec, const int ind, const int num) { vec[ind] += num; }
public:
    Transition() = default;
    Transition(std::vector<int>&& i, std::vector<int>&& o) : inp(std::move(i)), out(std::move(o)) {}
    [[nodiscard]] const auto& get_inp() const { return inp; }
    [[nodiscard]] const auto& get_out() const { return out; }
    void update_inp(const int ind, const int num) { update(inp, ind, num); }
    void update_out(const int ind, const ind num) { update(out, ind, num);
};


#endif //LABS_TRANSITION_H
