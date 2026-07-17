// Source - https://stackoverflow.com/q/70922705
// Posted by Afshin
// Retrieved 2026-07-17, License - CC BY-SA 4.0

#include <map>
#include <array>

template <typename T, size_t SZ>
class monotonic_allocator
{
public:
    using value_type = T;

    monotonic_allocator() noexcept {} 

    [[nodiscard]]
    value_type* allocate(std::size_t n)
    {
        size_t start = 0;
        for (const auto& [alloc_start, alloc_size] : alloc_list_) {
            if ((alloc_start - start) <= n) {
                alloc_list_[start] = n;
                return mem_.data() + start;
            }

            start = alloc_start + alloc_size;
        }

        throw std::bad_alloc{};
    }

    void deallocate(value_type* p, std::size_t n) noexcept
    {
        alloc_list_.erase(static_cast<size_t>(p - mem_.data()));
    }

    template <typename T1, size_t SZ1, typename T2, size_t SZ2>
    friend bool operator==(monotonic_allocator<T1, SZ1> const& x, monotonic_allocator<T2, SZ2> const& y) noexcept;

private:
    std::array<value_type, SZ> mem_;
    std::map<size_t, size_t> alloc_list_{};
};

template <typename T1, size_t SZ1, typename T2, size_t SZ2>
bool operator==(monotonic_allocator<T1, SZ1> const& x, monotonic_allocator<T2, SZ2> const& y) noexcept
{
    return SZ1 == SZ2 && x.mem_.data() == y.mem_.data();
}

template <typename T1, size_t SZ1, typename T2, size_t SZ2>
bool operator!=(monotonic_allocator<T1, SZ1> const& x, monotonic_allocator<T2, SZ2> const& y) noexcept
{
    return !(x == y);
}

int main()
{
    std::vector<int, monotonic_allocator<int, 4096>> vec = {1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,1,2,3,6};
}
