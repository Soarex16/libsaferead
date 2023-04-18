//
// Created by Шумаф Ловпаче on 18.04.2023.
//

#ifndef SAFE_READ_LIBSAFEREAD_H
#define SAFE_READ_LIBSAFEREAD_H

#include <cstdint>
#include <optional>

std::optional<std::uint8_t> safe_read_uint8(const std::uint8_t* p);

#endif //SAFE_READ_LIBSAFEREAD_H
