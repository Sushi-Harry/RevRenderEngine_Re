#pragma once
#include <cstdint>
#include <vector>
#include <string>

class Cubemap{
public:
    Cubemap(const std::vector<std::string>& faces);
    ~Cubemap();

    void bind(uint32_t slot) const;
    uint32_t get_id() const;

    Cubemap(const Cubemap&) = delete;
    Cubemap& operator=(const Cubemap&) = delete;

    Cubemap(Cubemap&& other) noexcept : _id(other._id) { other._id = 0; }
    Cubemap& operator=(Cubemap&& other) noexcept;

private:
    uint32_t _id = 0;
};
