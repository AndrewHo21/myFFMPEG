/*
 * RAW PCM muxers
 * Copyright (c) 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "avformat.h"
#include "rawenc.h"

/* Writes a packet using the packet data that was given via asif_receive_packet from the encoder file
 */
static int asif_write_packet(AVFormatContext *s, AVPacket *pkt){
  avio_write(s->pb , pkt->data, pkt->size);
  return 0;
}

AVOutputFormat ff_asif_muxer = {               
    .name         = "asif",                                 
    .long_name    = NULL_IF_CONFIG_SMALL("ASIF audio file (CS 3505 Spring 20202)"),
    .extensions   = "asif",                                    
    .audio_codec  = AV_CODEC_ID_ASIF,                                                            
    .write_packet = asif_write_packet,                    
    .flags        = AVFMT_NOTIMESTAMPS,                     
};
