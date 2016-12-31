
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_STRING_H_INCLUDED_
#define _SAE_STRING_H_INCLUDED_

#include "sae_config.h"
#include "sae_core.h"

#define sae_strlen(s) strlen((sae_char_t *)s)

#define sae_memzero(buf, n) memset(buf, 0, n)

#define sae_memcpy(dst, src, n) memcpy(dst, src, n)

#define sae_cp_str(dst, src, n) \
                    sae_memcpy(dst, src, len); \
                    dst[len] = sae_str_end;

#define sae_strcmp(s1, s2)  strcmp((sae_cchar_t *)s1, (sae_cchar_t *)s2)

#define sae_strrchr(s, c)  strrchr((sae_cchar_t *)s, (sae_int_t)c)

#define sae_strchr(s, c)  strchr((sae_cchar_t *)s, (sae_int_t)c)

#define sae_isspace(c)  sae_space((sae_int_t)c)

#define sae_strncpy(dst, src, n) strncpy((sae_char_t *)dst, (sae_cchar_t *)src, (sae_size_t)n)

#define sae_strncmp(s1, s2, n)  strncmp((sae_cchar_t *)s1, (sae_cchar_t *)s2, (sae_size_t)n)

#if (HAVE_WIN32)
sae_int_t sae_strncmp_parse(sae_cchar_t *s1, sae_parse_char_t *s2, sae_size_t n);
#else
#define sae_strncmp_parse(s1, s2, n)  strncmp((sae_cchar_t *)s1, (sae_parse_char_t *)s2, (sae_size_t)n)
#endif

#if (HAVE_WIN32)
#define sae_snprintf(str, len, format, ...) sprintf_s(str, len, format, __VA_ARGS__)
#else
#define sae_snprintf(str, len, format, ...) snprintf(str, len, format, __VA_ARGS__)
#endif

#if (HAVE_WIN32)
#define sae_vsnprintf(str, len, format, arg_list) vsnprintf_s(str, len, _TRUNCATE, format, arg_list)
#else
#define sae_vsnprintf(str, len, format, arg_list) vsnprintf(str, len, format, arg_list)
#endif

sae_ulong_t sae_str_hash(sae_char_t *str);

sae_void_t sae_str_trim(sae_char_t *str);

sae_int_t sae_space(sae_int_t c);

sae_char_t *sae_int_to_str(sae_int_t v);

/*char to windows utf-8*/
#if (HAVE_WIN32)
WCHAR *sae_char_to_utf8(sae_cchar_t *str);
#endif

#endif /* _SAE_STRING_H_INCLUDED_ */
