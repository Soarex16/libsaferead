//
// Created by Soarex16.
//
#include <cassert>
#include <cstdint>
#include <optional>
#include <unistd.h>
#include <sys/malloc.h>
#include <sys/mman.h>

#include "libsaferead.h"

void read_existing_address()
{
    std::uint8_t value = 42;
    auto res = safe_read_uint8(&value);
    assert(res.has_value());
    assert(res.value() == 42);
}

void read_nullptr()
{
    std::uint8_t* p = nullptr;
    auto res = safe_read_uint8(p);
    assert(!res.has_value());
}

void read_protected_memory()
{
    auto page_size = sysconf(_SC_PAGESIZE);
    char* buffer;

    size_t num_of_pages = 4;

    posix_memalign((void**)&buffer, page_size, num_of_pages * page_size);

    // last page is protected
    if (mprotect(buffer + page_size * (num_of_pages - 1), page_size, PROT_NONE) == -1) {
        perror("mprotect");
    }

    for (int i = 0; i < num_of_pages - 1; ++i) {
        auto res = safe_read_uint8(reinterpret_cast<const uint8_t *>(buffer + i * page_size));
        assert(res.has_value());
    }


    auto res = safe_read_uint8(reinterpret_cast<const uint8_t *>(buffer + (num_of_pages - 1) * page_size));
    assert(!res.has_value());
}

int main() {
    read_existing_address();
    read_nullptr();
    read_protected_memory();
}