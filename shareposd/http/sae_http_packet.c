
/*
 * Copyright (C) hanchong liu
 */


#include "sae_http_packet.h"

sae_http_packet_t *sae_http_packet_create(sae_socket_fd_t fd, sae_http_packet_table_t *table)
{
    sae_http_packet_t *packet = sae_alloc(sae_sizeof(sae_http_packet_t));
    if (!packet)
    {
        return packet;
    }
    
    packet->http_client_buffer = sae_buffer_create();
    if (!packet->http_client_buffer)
    {
        sae_alloc_free(packet);
        return sae_null;
    }
        
    packet->http_client_fd = fd;
    packet->packet_table = table;
    packet->http_client_fd_str = sae_int_to_str(fd);
    
    return packet;
}

sae_void_t sae_http_packet_destroy(sae_http_packet_t *packet)
{
    sae_alloc_free(packet->http_client_fd_str);
    
    sae_alloc_free(packet->http_client_buffer);
    
    sae_alloc_free(packet);
}

sae_bool_t sae_http_packet_read(sae_http_packet_t *packet)
{
    return sae_buffer_read_socket(packet->http_client_buffer, packet->http_client_fd);
}

sae_bool_t sae_http_packet_push(sae_http_packet_t *packet)
{
    return sae_table_push(packet->packet_table->packet, packet->http_client_fd_str, packet);
}

sae_bool_t sae_http_packet_del(sae_http_packet_t *packet)
{
    return sae_table_del(packet->packet_table->packet, packet->http_client_fd_str);
}
