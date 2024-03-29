/*
 * Astra Module: SoftCAM
 * http://cesbo.com/astra
 *
 * Copyright (C) 2012-2013, Andrey Dyldin <and@cesbo.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../module_cam.h"

em_packet_t * module_cam_queue_pop(module_cam_t *cam)
{
    asc_list_first(cam->packet_queue);
    if(asc_list_eol(cam->packet_queue))
        return NULL;
    em_packet_t *packet = (em_packet_t *)asc_list_data(cam->packet_queue);
    asc_list_remove_current(cam->packet_queue);
    return packet;
}

void module_cam_queue_flush(module_cam_t *cam, module_decrypt_t *decrypt)
{
    asc_list_first(cam->packet_queue);
    while(!asc_list_eol(cam->packet_queue))
    {
        em_packet_t *packet = (em_packet_t *)asc_list_data(cam->packet_queue);
        if(!decrypt || packet->decrypt == decrypt)
        {
            free(packet);
            asc_list_remove_current(cam->packet_queue);
        }
        else
            asc_list_next(cam->packet_queue);
    }
}

void module_cam_ready(module_cam_t *cam)
{
    cam->is_ready = true;

    asc_list_for(cam->decrypt_list)
    {
        module_decrypt_t *__decrypt = (module_decrypt_t *)asc_list_data(cam->decrypt_list);
        on_cam_ready(__decrypt->self);
    }
}

void module_cam_reset(module_cam_t *cam)
{
    cam->is_ready = false;

    asc_list_for(cam->decrypt_list)
    {
        module_decrypt_t *__decrypt = (module_decrypt_t *)asc_list_data(cam->decrypt_list);
        on_cam_error(__decrypt->self);
    }

    asc_list_till_empty(cam->prov_list)
    {
        asc_list_remove_current(cam->prov_list);
    }

    module_cam_queue_flush(cam, NULL);
}

void module_cam_attach_decrypt(module_cam_t *cam, module_decrypt_t *decrypt)
{
    cam->connect(cam->self);
    asc_list_insert_tail(cam->decrypt_list, decrypt);
    if(cam->is_ready)
        on_cam_ready(decrypt->self);
}

void module_cam_detach_decrypt(module_cam_t *cam, module_decrypt_t *decrypt)
{
    module_cam_queue_flush(cam, decrypt);
    asc_list_remove_item(cam->decrypt_list, decrypt);
    if(asc_list_size(cam->decrypt_list) == 0)
        cam->disconnect(cam->self);
}
