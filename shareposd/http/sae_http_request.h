
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_HTTP_REQUEST_H_INCLUDED_
#define _SAE_HTTP_REQUEST_H_INCLUDED_

#include "sae_http.h"

sae_void_t *sae_http_request_accept(sae_event_t *event, sae_void_t *arg);

sae_void_t *sae_http_request_read(sae_event_t *event, sae_void_t *arg);

#endif /* _SAE_HTTP_REQUEST_H_INCLUDED_ */
