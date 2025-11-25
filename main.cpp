#include <print>
#include <vector>
#include <span>

using namespace std;

struct Item {
    int64_t weight{};
    int64_t value{};
};

class KnapsackProblem {
public:
    explicit KnapsackProblem(const span<const Item> items_) : items{items_.begin(), items_.end()} {}

    [[nodiscard]] int64_t solveNaive(const int64_t total_capacity) const {
        if (total_capacity < 0)
            throw invalid_argument{"total_capacity < 0"};

        return solveNaiveRecursive(items.size(), total_capacity);
    }

    [[nodiscard]] int64_t solveTopDown(const int64_t total_capacity) const {
        if (total_capacity < 0)
            throw invalid_argument{"total_capacity < 0"};

        // memo[number_of_items][capacity] = -1
        vector<vector<int64_t>> memo(items.size() + 1, vector<int64_t>(total_capacity + 1, -1));
        const int64_t result = solveTopDownRecursive(memo, items.size(), total_capacity);
        println("memo: {}", memo);
        return result;
    }

    [[nodiscard]] int64_t solveBottomUp(const int64_t total_capacity) const {
        if (total_capacity < 0)
            throw invalid_argument{"total_capacity < 0"};

        // dp[number_of_items][capacity] = 0
        vector<vector<int64_t>> dp(items.size() + 1, vector<int64_t>(total_capacity + 1));

        for (size_t number_of_items = 1; number_of_items <= items.size(); ++number_of_items) {
            const auto &item = items[number_of_items - 1];

            for (size_t capacity = 0; capacity <= total_capacity; ++capacity) {
                int64_t result;
                if (capacity < item.weight)
                    result = dp[number_of_items - 1][capacity];
                else {
                    const int64_t picked_nth_item = dp[number_of_items - 1][capacity - item.weight] + item.value;
                    const int64_t not_picked_nth_item = dp[number_of_items - 1][capacity];

                    result = max(picked_nth_item, not_picked_nth_item);
                }

                dp[number_of_items][capacity] = result;
            }
        }

        println("dp: {}", dp);
        return dp[items.size()][total_capacity];
    }

    [[nodiscard]] int64_t solveBottomUpOptimized(const int64_t total_capacity) const {
        if (total_capacity < 0)
            throw invalid_argument{"total_capacity < 0"};

        // dp[c] = best achievable value using items seen so far with capacity c
        // dp[capacity] = 0
        vector<int64_t> dp(total_capacity + 1);

        for (size_t number_of_items = 1; number_of_items <= items.size(); ++number_of_items) {
            const auto &item = items[number_of_items - 1];

            for (ptrdiff_t capacity = total_capacity; capacity >= item.weight; --capacity) {
                const int64_t picked_nth_item = dp[capacity - item.weight] + item.value;
                const int64_t not_picked_nth_item = dp[capacity];

                const int64_t result = max(picked_nth_item, not_picked_nth_item);
                dp[capacity] = result;
            }
        }

        println("dp: {}", dp);
        return dp[total_capacity];
    }

private:
    vector<Item> items;

    [[nodiscard]] int64_t solveNaiveRecursive(const size_t number_of_items, const int64_t capacity) const {
        if (number_of_items == 0)
            return 0;

        const auto &item = items[number_of_items - 1];

        if (capacity < item.weight)
            return solveNaiveRecursive(number_of_items - 1, capacity);

        const int64_t picked_nth_item = solveNaiveRecursive(number_of_items - 1, capacity - item.weight) + item.value;
        const int64_t not_picked_nth_item = solveNaiveRecursive(number_of_items - 1, capacity);

        return max(picked_nth_item, not_picked_nth_item);
    }

    [[nodiscard]] int64_t
    solveTopDownRecursive(vector<vector<int64_t>> &memo, const size_t number_of_items, const int64_t capacity) const {
        const int64_t stored = memo[number_of_items][capacity];
        if (stored != -1)
            return stored;

        if (number_of_items == 0) {
            memo[number_of_items][capacity] = 0;
            return 0;
        }

        const auto &item = items[number_of_items - 1];

        if (capacity < item.weight)
            return memo[number_of_items][capacity] = solveTopDownRecursive(memo, number_of_items - 1, capacity);

        const int64_t picked_nth_item =
                solveTopDownRecursive(memo, number_of_items - 1, capacity - item.weight) + item.value;
        const int64_t not_picked_nth_item = solveTopDownRecursive(memo, number_of_items - 1, capacity);

        const int64_t result = max(picked_nth_item, not_picked_nth_item);
        memo[number_of_items][capacity] = result;
        return result;
    }
};

int main() {
    const vector<Item> items = {
            {4, 1},
            {5, 2},
            {1, 3}
    };
    const KnapsackProblem knapsackProblem{items};
    const int64_t capacity = 4;

    println("Naive: {}", knapsackProblem.solveNaive(capacity));
    println("Top Down: {}", knapsackProblem.solveTopDown(capacity));
    println("Bottom Up: {}", knapsackProblem.solveBottomUp(capacity));
    println("Bottom Up Optimized: {}", knapsackProblem.solveBottomUpOptimized(capacity));
    return 0;
}
