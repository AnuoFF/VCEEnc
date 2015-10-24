﻿//  -----------------------------------------------------------------------------------------
//    VCEEnc by rigaya
//  -----------------------------------------------------------------------------------------
//   ソースコードについて
//   ・無保証です。
//   ・本ソースコードを使用したことによるいかなる損害・トラブルについてrigayaは責任を負いません。
//   以上に了解して頂ける場合、本ソースコードの使用、複製、改変、再頒布を行って頂いて構いません。
//  -----------------------------------------------------------------------------------------

#pragma once

#define VER_FILEVERSION             0,1,3,0
#define VER_STR_FILEVERSION          "1.03"
#define VER_STR_FILEVERSION_TCHAR _T("1.03")

#define VCE_AMD_APP_SDK "3.0"

#define ENABLE_OPENCL 1

#define CHECK_PERFORMANCE 1

#ifdef _M_IX86
#define BUILD_ARCH_STR _T("x86")
#else
#define BUILD_ARCH_STR _T("x64")
#endif

#if defined(VCE_AUO) && defined(NDEBUG)
#define ENABLE_VAPOURSYNTH_READER 0
#define ENABLE_AVISYNTH_READER 0
#else
#define ENABLE_VAPOURSYNTH_READER 1
#define ENABLE_AVISYNTH_READER 1
#endif
