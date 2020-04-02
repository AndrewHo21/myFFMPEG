/**
 * ASIF audio decoder
 */

/*
 * PCM codecs
 * Copyright (c) 2001 Fabrice Bellard
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

/**
 * @file
 * PCM codecs
 */

#include "libavutil/attributes.h"
#include "libavutil/float_dsp.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"
#include "mathops.h"
#include "pcm_tablegen.h"

/**
 * Read PCM samples macro
 * @param size   Data size of native machine format
 * @param endian bytestream_get_xxx() endian suffix
 * @param src    Source pointer (variable name)
 * @param dst    Destination pointer (variable name)
 * @param n      Total number of samples (variable name)
 * @param shift  Bitshift (bits)
 * @param offset Sample value offset
 */
#define DECODE(size, endian, src, dst, n, shift, offset)                       \
    for (; n > 0; n--) {                                                       \
        uint ## size ## _t v = bytestream_get_ ## endian(&src);                \
        AV_WN ## size ## A(dst, (uint ## size ## _t)(v - offset) << shift);    \
        dst += size / 8;                                                       \
    }

#define DECODE_PLANAR(size, endian, src, dst, n, shift, offset)                \
    n /= avctx->channels;                                                      \
    for (c = 0; c < avctx->channels; c++) {                                    \
        int i;                                                                 \
        dst = frame->extended_data[c];                                         \
        for (i = n; i > 0; i--) {                                              \
            uint ## size ## _t v = bytestream_get_ ## endian(&src);            \
            AV_WN ## size ## A(dst, (uint ## size ##_t)(v - offset) << shift); \
            dst += size / 8;                                                   \
        }                                                                      \
    }

/* decoder */
AVCodec ff_asif_decoder = {                                        
    .name           = "asif",                                               
    .long_name      = NULL_IF_CONFIG_SMALL("ASIF audio file (CS 3505 Spring 20202)"),                     
    .type           = AVMEDIA_TYPE_AUDIO,                                   
    .id             = AV_CODEC_ID_ASIF,                                  
    //.priv_data_size = sizeof(PCMDecode),                                    
    //.init           = pcm_decode_init,                                      
    //.close          = pcm_decode_close,                                     
    //.decode         = pcm_decode_frame,                                     
    .capabilities   = AV_CODEC_CAP_DR1,                                     
    //.sample_fmts    = (const enum AVSampleFormat[]){ sample_fmt_,           
    // AV_SAMPLE_FMT_NONE },  
};
