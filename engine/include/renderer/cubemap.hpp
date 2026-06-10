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

private:
    uint32_t _id = 0;
};
