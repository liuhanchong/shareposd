/**
  @file klhttp-internal.c
  @author Kevin Lynx
  @data 7.25.2008
  @brief for klhttpd internal use.
*/
#include "klhttp-in.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define NEW( type ) (type*) malloc( sizeof( type ) )

/**
  parse http request initial line.
*/
static int _parse_init_line( struct http_request *request, char *line )
{
	char *token = strtok( line, " " );
	
	/* method */
	if( strcmp( token, "GET" ) == 0 )
	{
		request->type = HTTP_GET;
	}
	else if( strcmp( token, "HEAD" ) == 0 )
	{
		request->type = HTTP_HEAD;
	}
    else if( strcmp( token, "POST" ) == 0 )
    {
        request->type = HTTP_POST;
    }
    else
	{
		request->type = HTTP_UNKNOWN;
	}

	/* uri */
	token = strtok( 0, " " );
	request->uri = (char*) malloc( strlen( token ) + 1 );
	strcpy( request->uri, token );

	/* http protocol version */
	token = strtok( 0, " " );
	if( strcmp( token, "HTTP/1.0" ) == 0 )
	{
		request->ver.major = 1;
		request->ver.minor = 0;
	}
	else if( strcmp( token, "HTTP/1.1" ) == 0 )
	{
		request->ver.major = 1;
		request->ver.minor = 1;
	}
	else
	{
		request->ver.major = 1;
		request->ver.major = 0;
	}

	return 0;
}

/**
  add response headers
*/
static void _add_headers( struct http_header_head *headers, const char *name, const char *value )
{
	struct http_header *header = NEW( struct http_header );
	header->name = (char*) malloc( strlen( name ) + 1 );
	header->value = (char*) malloc( strlen( value ) + 1 );
	strcpy( header->name, name );
	strcpy( header->value, value );

	TAILQ_INSERT_TAIL( headers, header, next );
}

/**
  add time header
*/
void add_time_header( sae_buffer_t *buf )
{
	char date[50];
#ifndef WIN32
	struct tm cur;
#endif
	struct tm *cur_p;
	time_t t = time( NULL );
#ifdef WIN32
	cur_p = gmtime( &t );
#else
	gmtime_r( &t, &cur );
	cur_p = &cur;
#endif
	strftime( date, sizeof( date ), "%a, %d %b %Y %H:%M:%S GMT", cur_p);
	sae_buffer_add_printf( buf, "Date: %s\r\n", date );
}

/**
  get an uri's mime string.
*/
const char *get_mime_type( const char *uri, char *buf )
{
	const char *t = uri + strlen( uri );
	char type[64];
	for( ; t >= uri && *t != '.'; -- t )
	{
		/* until '.'*/
		;
	}

	strcpy( type, t + 1 );

	/* append mime header */
	if( strcmp( type, "html" ) == 0 || 
		strcmp( type, "htm" ) == 0 )
	{
		sprintf( buf, "text/%s", type );
	}
	else if( strcmp( type, "gif" ) == 0 ||
		strcmp( type, "jpg" ) == 0 ||
		strcmp( type, "jpeg" ) == 0 ||
		strcmp( type, "png" ) == 0 )
	{
		sprintf( buf, "image/%s", type );
	}
	else if( strcmp( type, "/" ) == 0 )
	{
		/* the default value */
		sprintf( buf, "text/html" );
	}
	else
	{
		/* unknown error */
		sprintf( buf, "unknown" );
	}

	return buf;
}

/**
  check the whether the request is valid, if not, it will write the error response.

  @return if the request is valid return 0.
*/
int check_request_valid( sae_buffer_t *buf, const struct http_request *request )
{
    if (request == NULL)
    {
        return -1;
    }

	if( request->type == HTTP_UNKNOWN )
	{
		/* not implemented method */
		http_response_error( buf, HTTP_NOTIMPLEMENT, CODE_STR( HTTP_NOTIMPLEMENT ), "Not Implemented Method" );
		return -1;
	}

	/* HTTP 1.1 needs Host header */
	if( request->ver.major == 1 && request->ver.minor == 1 )
	{
		const char *host = http_get_header_value( request->headers, "Host" );
		if( host == 0 )
		{
			/* bad request */
			http_response_error( buf, HTTP_BADREQUEST, CODE_STR( HTTP_BADREQUEST ), "Bad Request : needs Host header" );
			return -1;
		}
	}

	return 0;
}

struct http_header_head *http_header_new()
{
	struct http_header_head *queue = NEW( struct http_header_head );
	if( queue == 0 )
	{
		return 0;
	}
	TAILQ_INIT( queue );
	return queue;
}

void http_header_free( struct http_header_head *header_queue )
{
	struct http_header *header, *prev = 0;
	TAILQ_FOREACH( header, header_queue, next )
	{
		if( prev != 0 )
		{
			free( prev );
		}
		free( header->name );
		free( header->value );
		prev = header;
	}
	free( header_queue );
}

int http_add_header( struct http_header_head *header_queue, char *line )
{
	char *value ;
	char *name = strchr( line, ':' );
	struct http_header *header = NEW( struct http_header );
	header->name = (char*) malloc( name - line + 1 );
	strncpy( header->name, line, name - line );
	header->name[name-line] = '\0';

	for( value = name + 1; *value == ' '; ++ value )
	{
		/* until a no-space character. */
		;
	}

	header->value = (char*) malloc( strlen( line ) - ( value - line ) + 1 );
	strcpy( header->value, value );
	TAILQ_INSERT_TAIL( header_queue, header, next );

	return 0;
}

int http_add_body( struct http_header_head *header_queue,  sae_buffer_t *buf )
{
    int conlen = atoi(http_get_header_value(header_queue, "Content-Length"));
    if (conlen <= 0)
    {
        _add_headers(header_queue, "body", "");
        _add_headers(header_queue, "bodylen", "0");
        return 1;
    }
    
    if (buf->buffer_off < conlen)
    {
        _add_headers(header_queue, "body", "");
        _add_headers(header_queue, "bodylen", "0");
        return 1;
    }
    
    char *data = (char *)malloc(conlen);
    if (data == NULL)
    {
        _add_headers(header_queue, "body", "");
        _add_headers(header_queue, "bodylen", "0");
        return 1;
    }
    memset(data, 0, conlen);
    
    strncpy(data, (const char *)buf->buffer, conlen);
    
    _add_headers(header_queue, "body", data);
    _add_headers(header_queue, "bodylen", http_get_header_value(header_queue, "Content-Length"));
    
    free(data);
                 
    return 0;
}

const char *http_get_header_value( struct http_header_head *header_queue, const char *header_name )
{
	struct http_header *header;
	TAILQ_FOREACH( header, header_queue, next )
	{
		if( strcmp( header->name, header_name ) == 0 )
		{
			return header->value;
		}
	}
	return 0;
}

struct http_request *http_request_parse( sae_buffer_t *buf )
{
	struct http_request *request = NEW( struct http_request );
	if( request == 0 )
	{
		return 0;
	}

	/* parse initial line */
	{
		char *line = sae_buffer_read_line( buf );
        if ( line == NULL )
        {
            return 0;
        }
		int ret = _parse_init_line( request, line );
		free( line );
		if( ret != 0 )
		{
			free( request );
			return 0;
		}
	}

	/* create the header queue */
	request->headers = http_header_new();
	/* parse headers */
	{
		char *line = sae_buffer_read_line( buf ) ;
		int ret = 0;
		while( line != 0 && *line != '\0' ) /* there is one null line between header and body */
		{
			ret = http_add_header( request->headers, line );

			free( line );
			if( ret != 0 )
			{
				break;
			}
			line = sae_buffer_read_line( buf );
		}
	}

	/* ignore the bodies because it does not support now */
	return request;
}

/**
 parse img headers
 */
struct http_header_head *http_img_header( sae_buffer_t *buf )
{
    /* create the header queue */
    struct http_header_head *headers = http_header_new();
    /* parse headers */
    {
        char *line = sae_buffer_read_line( buf ) ;
        int ret = 0;
        while( line != 0 && *line != '\0' ) /* there is one null line between header and body */
        {
            ret = http_add_header( headers, line );
            
            free( line );
            if( ret != 0 )
            {
                break;
            }
            line = sae_buffer_read_line( buf );
        }
    }
    
    /* ignore the bodies because it does not support now */
    return headers;
}

void http_request_free( struct http_request *request )
{
	free( request->uri );
	http_header_free( request->headers );
	free( request );
}

void http_response_error( sae_buffer_t *buf, int status, const char *status_str, const char *more_info )
{
#define ERR_FORMAT "<HTML><HEAD>\n" \
	    "<TITLE>%d %s</TITLE>\n" \
	    "</HEAD><BODY>\n" \
	    "<H1>%d %s</H1>\n" \
	    "%s<P>\n" \
	    "</BODY></HTML>\n"
	char content[512];
	size_t len;

	sprintf( content, ERR_FORMAT, status, status_str, status, status_str, more_info );
	len = strlen( content );

	/* initial line */
	sae_buffer_add_printf( buf, "HTTP/1.1 %d %s\r\n", status, status_str );
	/* add server info header */
	sae_buffer_add_printf( buf, "Server: klhttpd/0.1.0\r\n" );
	/* add time header */
	add_time_header( buf );
	/* content type */
	sae_buffer_add_printf( buf, "Content-Type: text/html\r\n" );
	/* content length */
	sae_buffer_add_printf( buf, "Content-Length: %d\r\n", (int)len );
	/* end of headers */
	sae_buffer_add( buf, "\r\n", 2 );
	/* body */
	sae_buffer_add( buf, content, len );
	
#undef ERR_FORMAT
}
