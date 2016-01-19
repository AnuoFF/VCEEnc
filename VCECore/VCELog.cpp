﻿// -----------------------------------------------------------------------------------------
//     VCEEnc by rigaya
// -----------------------------------------------------------------------------------------
// The MIT License
//
// Copyright (c) 2014-2016 rigaya
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

#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <math.h>
#include <tchar.h>
#include <process.h>

#include "VCEParam.h"
#include "VCEVersion.h"
#include "VCELog.h"

const char *VCELog::HTML_FOOTER = "</body>\n</html>\n";

void VCELog::init(const TCHAR *pLogFile, int log_level) {
    m_pStrLog = pLogFile;
    m_nLogLevel = log_level;
    if (pLogFile != nullptr) {
        FILE *fp = NULL;
        if (_tfopen_s(&fp, pLogFile, _T("a+")) || fp == NULL) {
            fprintf(stderr, "failed to open log file, log writing disabled.\n");
            pLogFile = nullptr;
        } else {
            if (check_ext(pLogFile, { ".html", ".htm" })) {
                _fseeki64(fp, 0, SEEK_SET);
                char buffer[1024] ={ 0 };
                size_t file_read = fread(buffer, 1, sizeof(buffer)-1, fp);
                if (file_read == 0) {
                    m_bHtml = true;
                    writeHtmlHeader();
                } else {
                    std::transform(buffer, buffer + file_read, buffer, [](char in) -> char {return (char)tolower(in); });
                    if (strstr(buffer, "doctype") && strstr(buffer, "html")) {
                        m_bHtml = true;
                    }
                }
            }
            fclose(fp);
        }
    }
}

void VCELog::writeFileHeader(const TCHAR *pDstFilename) {
    tstring fileHeader;
    int dstFilenameLen = (int)_tcslen(pDstFilename);
    static const TCHAR *const SEP5 = _T("-----");
    int sep_count = max(16, dstFilenameLen / 5 + 1);
    if (m_bHtml) {
        fileHeader += _T("<hr>");
    } else {
        for (int i = 0; i < sep_count; i++)
            fileHeader += SEP5;
    }
    fileHeader += _T("\n") + tstring(pDstFilename) + _T("\n");
    if (m_bHtml) {
        fileHeader += _T("<hr>");
    } else {
        for (int i = 0; i < sep_count; i++)
            fileHeader += SEP5;
    }
    fileHeader += _T("\n");
    (*this)(VCE_LOG_INFO, fileHeader.c_str());
}

void VCELog::writeHtmlHeader() {
    FILE *fp = NULL;
    if (!_tfopen_s(&fp, m_pStrLog, _T("wb"))) {
        std::wstring header =
            L"<!DOCTYPE html>\n"
            L"<html lang = \"ja\">\n"
            L"<head>\n"
            L"<meta charset = \"UTF-8\">\n"
            L"<title>QSVEncC Log</title>\n"
            L"<style type=text/css>\n"
            L"   body   { \n"
            L"       background-color: #303030;\n"
            L"       line-height:1.0; font-family: \"MeiryoKe_Gothic\",\"遊ゴシック\",\"ＭＳ ゴシック\",sans-serif;\n"
            L"       margin: 10px;\n"
            L"       padding: 0px;\n"
            L"   }\n"
            L"   div {\n"
            L"       white-space: pre;\n"
            L"   }\n"
            L"   .error { color: #FA5858 }\n"
            L"   .warn  { color: #F7D358 }\n"
            L"   .more  { color: #CEF6F5 }\n"
            L"   .info  { color: #CEF6F5 }\n"
            L"   .debug { color: #ACFA58 }\n"
            L"   .trace { color: #ACFA58 }\n"
            L"</style>\n"
            L"</head>\n"
            L"<body>\n";
        fprintf(fp, wstring_to_string(header, CP_UTF8).c_str());
        fprintf(fp, HTML_FOOTER);
        fclose(fp);
    }
}

void VCELog::writeFileFooter() {
    (*this)(VCE_LOG_INFO, _T("\n\n"));
}

void VCELog::operator()(int log_level, const TCHAR *format, ...) {
    if (log_level < m_nLogLevel) {
        return;
    }

    auto convert_to_html = [log_level](std::string str) {
        //str = str_replace(str, "<", "&lt;");
        //str = str_replace(str, ">", "&gt;");
        //str = str_replace(str, "&", "&amp;");
        //str = str_replace(str, "\"", "&quot;");

        auto strLines = split(str, "\n");

        std::string strHtml;
        for (uint32_t i = 0; i < strLines.size() - 1; i++) {
            strHtml += strsprintf("<div class=\"%s\">", tchar_to_string(list_log_level[log_level - VCE_LOG_TRACE].desc).c_str());
            strHtml += strLines[i];
            strHtml += "</div>\n";
        }
        return strHtml;
    };

    va_list args;
    va_start(args, format);

    int len = _vsctprintf(format, args) + 1; // _vscprintf doesn't count terminating '\0'
    tstring buffer(len, 0);
    if (buffer.data() != nullptr) {

        _vstprintf_s(&buffer[0], len, format, args); // C4996

        HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
        std::string buffer_char;
#ifdef UNICODE
        char *buffer_ptr = NULL;
        DWORD mode = 0;
        bool stderr_write_to_console = 0 != GetConsoleMode(hStdErr, &mode); //stderrの出力先がコンソールかどうか
        if (m_pStrLog || !stderr_write_to_console) {
            buffer_char = tchar_to_string(buffer, (m_bHtml) ? CP_UTF8 : CP_THREAD_ACP);
            if (m_bHtml) {
                buffer_char = convert_to_html(buffer_char);
            }
            buffer_ptr = &buffer_char[0];
        }
#else
        char *buffer_ptr = &buffer[0];
        if (m_bHtml) {
            buffer_char = wstring_to_string(char_to_wstring(buffer_ptr), CP_UTF8);
            if (m_bHtml) {
                buffer_char = convert_to_html(buffer_char);
            }
            buffer_ptr = &buffer_char[0];
        }
#endif
        EnterCriticalSection(&cs);
        if (m_pStrLog) {
            FILE *fp_log = NULL;
            //logはANSI(まあようはShift-JIS)で保存する
            if (0 == _tfopen_s(&fp_log, m_pStrLog, (m_bHtml) ? _T("rb+") : _T("a")) && fp_log) {
                if (m_bHtml) {
                    _fseeki64(fp_log, 0, SEEK_END);
                    __int64 pos = _ftelli64(fp_log);
                    _fseeki64(fp_log, 0, SEEK_SET);
                    _fseeki64(fp_log, pos -1 * strlen(HTML_FOOTER), SEEK_CUR);
                }
                fwrite(buffer_ptr, 1, strlen(buffer_ptr), fp_log);
                if (m_bHtml) {
                    fwrite(HTML_FOOTER, 1, strlen(HTML_FOOTER), fp_log);
                }
                fclose(fp_log);
            }
        }
#ifdef UNICODE
        if (!stderr_write_to_console) //出力先がリダイレクトされるならANSIで
            fprintf(stderr, buffer_ptr);
        if (stderr_write_to_console) //出力先がコンソールならWCHARで
#endif
            vce_print_stderr(log_level, buffer.data(), hStdErr);
        LeaveCriticalSection(&cs);
    }
    va_end(args);
}
