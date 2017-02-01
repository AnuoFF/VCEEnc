﻿// -----------------------------------------------------------------------------------------
//     VCEEnc by rigaya
// -----------------------------------------------------------------------------------------
// The MIT License
//
// Copyright (c) 2014-2017 rigaya
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// IABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// ------------------------------------------------------------------------------------------

#pragma once

#define VER_FILEVERSION             0,3,3,0
#define VER_STR_FILEVERSION          "3.03"
#define VER_STR_FILEVERSION_TCHAR _T("3.03")

#define VCE_AMD_APP_SDK "3.0"

#define ENABLE_OPENCL 1

#define ENABLE_AVCODEC_OUT_THREAD 1
#define ENABLE_AVCODEC_AUDPROCESS_THREAD 0

#ifdef _M_IX86
#define BUILD_ARCH_STR _T("x86")
#else
#define BUILD_ARCH_STR _T("x64")
#endif

#define ENABLE_LIBASS_SUBBURN 0

#if defined(VCE_AUO)
#define ENABLE_VAPOURSYNTH_READER 0
#define ENABLE_AVISYNTH_READER 0
#define ENABLE_AVCODEC_VCE_READER 0
#else
#define ENABLE_VAPOURSYNTH_READER 1
#define ENABLE_AVISYNTH_READER 1
#define ENABLE_AVCODEC_VCE_READER 1
#endif
