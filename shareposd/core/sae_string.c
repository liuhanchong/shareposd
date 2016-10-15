
/*
 * Copyright (C) hanchong liu
 */


#include "sae_config.h"
#include "sae_core.h"

sae_ulong_t sae_str_hash(sae_char_t *str)
{
    sae_int_t i = 0;
    sae_ulong_t ret = 0;
    sae_int_t l = 0;
    sae_ushort_t *s = sae_null;
    
    if (str == sae_null)
    {
        return 0;
    }
    
    l = (sae_int_t)((sae_strlen(str) + 1) / 2);
    s = (sae_ushort_t *)str;
    for (i = 0; i < l; i++)
    {
        ret ^= (s[i] << (i & 0x0f));
    }
    
    return ret;
}

sae_void_t sae_str_trim(sae_char_t *str)
{
    sae_size_t begin = 0;
    sae_size_t end = 0;
    sae_char_t *cpstr = sae_null;
    
    if ((end = sae_strlen(str)) < 1)
    {
        return;
    }
    
    /*alloc cp str*/
    cpstr = sae_alloc(end);
    
    /*cp str*/
    sae_memcpy(cpstr, str, end);
                                 
    while (sae_isspace(str[begin++]));
    begin--;
    
    while (sae_isspace(str[--end]));
    
    sae_strncpy(str, cpstr + begin, (end - begin + 1));
    
    sae_alloc_free(cpstr);
    
    str[(end - begin + 1)] = sae_str_end;
}

sae_int_t sae_space(sae_int_t c)
{
    return (c == ' '  || c == '\t' ||
            c == '\r' || c == '\n' ||
            c == '\v' || c == '\f') ? 1 : 0;
}

#if (HAVE_WIN32)
WCHAR *sae_char_to_utf8(sae_cchar_t *str)
{
    sae_uint_t n = 0;
    WCHAR *utf_str = sae_null;
    
    /*get tran Unicode len*/
    n = MultiByteToWideChar(CP_UTF8, 0, str, -1, sae_null, 0);
    
    /*alloc utf*/
    utf_str = sae_alloc_n(sae_sizeof(WCHAR), n + 1);
    
    /*tran to utf8*/
    n = MultiByteToWideChar(CP_UTF8, 0, str, -1, utf_str, n);
    
    if(n == 0)
    {
        sae_alloc_free(utf_str);
        return sae_null;
    }
    
    return utf_str;
}
#endif

#if (HAVE_WIN32)
sae_int_t sae_strncmp_parse(sae_cchar_t *s1, sae_parse_char_t *s2, sae_size_t n)
{
    sae_parse_char_t *ws1 = sae_char_to_utf8(s1);
    
    sae_int_t ret = wcscmp(ws1, s2);
    
    sae_alloc_free(ws1);
    
    return ret;
}
#endif

sae_char_t *sae_int_to_str(sae_int_t v)
{
    sae_size_t len = (v > 0) ? 1 : 2;
    sae_int_t t = (v > 0) ? v : -v;
    
    while ((t / 10) && (len++));
    
    sae_char_t *buf = sae_alloc(len);
    
    sae_snprintf(buf, len, "%d", v);
    
    return buf;
}

