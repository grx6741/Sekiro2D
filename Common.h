#pragma once

#include <cstdint>

// The local port used to communicate with the steam servers.
#define CLIENT_LOCAL_PORT 27015

// The port that clients will connect to for gameplay.
#define SERVER_PORT 27016

#define CLIENT_VERSION "1.0.0.0"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef double f64;
typedef float  f32;
