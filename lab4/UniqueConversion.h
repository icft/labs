#include <memory>

template<typename Derived, typename Base>
std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base>&& p) {
    auto d = static_cast<Derived*>(p.release());
    return std::unique_ptr<Derived>(d, std::move(p.get_deleter()));
}

template<typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base>&& p) {
    if (Derived* result = dynamic_cast<Derived*>(p.get())) {
        p.release();
        return std::unique_ptr<Derived>(result, std::move(p.get_deleter()));
    }
    return std::unique_ptr<Derived>(nullptr, p.get_deleter());
}
