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

#endif //TURBONET_SRC_BASE_PUBLIC_DEFINE_H_
