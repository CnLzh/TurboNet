// Copyright (C) 2023
//
// File:     PublicDefine.h
// Brief:    Generic Macro Definition Module.
// Author:   CnLzh

#ifndef TURBONET_SRC_BASE_PUBLIC_DEFINE_H_
#define TURBONET_SRC_BASE_PUBLIC_DEFINE_H_

// Disable Copy and Assignment Constructors
#define DISALLOW_COPY_AND_ASSIGN(ClassName)  \
    ClassName (const ClassName&) = delete;   \
    ClassName operator=(const ClassName&) = delete;

// Primitive data types
using tb_s8 = char;
using tb_u8 = unsigned char;

using tb_s16 = short;
using tb_u16 = unsigned short;

using tb_s32 = int;
using tb_u32 = unsigned int;

using tb_s64 = long long;
using tb_u64 = unsigned long long;

using tb_f32 = float;
using tb_f64 = double;

#endif //TURBONET_SRC_BASE_PUBLIC_DEFINE_H_
