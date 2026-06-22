// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud {

//  void convertToUTF8(MGC_PCSZ text, std::string * utf8Result) {
//      if (text == NULL) {
//          return;
//      }
//#if MGC_SIZEOFWCHAR == 4 // sizeof(wchar_t) == 4
//      //... treat wchar_t string as UTF-32
//#else // sizeof(wchar_t) == 2
//      //... treat wchar_t string as UTF-16
//      std::vector<unsigned char> buf;
//      utf8::unchecked::utf16to8(text, text + wcslen(text), back_inserter(buf));
//      utf8Result->clear();
//      for(std::vector<unsigned char>::iterator it = buf.begin(); it != buf.end(); ++it) {
//          utf8Result->push_back(*it);
//      }
//#endif
//  }
    
    bool StringUtil::IsOptionalParameterValid(const char * param)
    {
        if (param == 0 || *param == '\0')
        {
            return false;
        }
        return true;
    }
    
    bool StringUtil::IsOptionalParameterValid(std::string param)
    {
        if (param.length() <= 0)
        {
            return false;
        }
        return true;
    }

    std::string StringUtil::Base64Encode(const uint8_t* data, size_t length)
    {
        static const char alphabet[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string result;
        result.reserve(((length + 2) / 3) * 4);

        for (size_t i = 0; i < length; i += 3)
        {
            uint32_t group = (uint32_t)data[i] << 16;
            if (i + 1 < length) group |= (uint32_t)data[i + 1] << 8;
            if (i + 2 < length) group |= (uint32_t)data[i + 2];

            result += alphabet[(group >> 18) & 0x3F];
            result += alphabet[(group >> 12) & 0x3F];
            result += (i + 1 < length) ? alphabet[(group >> 6) & 0x3F] : '=';
            result += (i + 2 < length) ? alphabet[group & 0x3F] : '=';
        }

        return result;
    }

    std::string StringUtil::Base64Encode(const std::string & input)
    {
        return Base64Encode(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    }
}
