/**
 * ASIF audio encoder
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

/* static av_cold int pcm_encode_init(AVCodecContext *avctx) */
/* { */
/*     avctx->frame_size = 0; */
/*     switch (avctx->codec->id) { */
/*     case AV_CODEC_ID_PCM_ALAW: */
/*         pcm_alaw_tableinit(); */
/*         break; */
/*     case AV_CODEC_ID_PCM_MULAW: */
/*         pcm_ulaw_tableinit(); */
/*         break; */
/*     case AV_CODEC_ID_PCM_VIDC: */
/*         pcm_vidc_tableinit(); */
/*         break; */
/*     default: */
/*         break; */
/*     } */

/*     avctx->bits_per_coded_sample = av_get_bits_per_sample(avctx->codec->id); */
/*     avctx->block_align           = avctx->channels * avctx->bits_per_coded_sample / 8; */
/*     avctx->bit_rate              = avctx->block_align * 8LL * avctx->sample_rate; */

/*     return 0; */
/* } */

/**
 * Write PCM samples macro
 * @param type   Datatype of native machine format
 * @param endian bytestream_put_xxx() suffix
 * @param src    Source pointer (variable name)
 * @param dst    Destination pointer (variable name)
 * @param n      Total number of samples (variable name)
 * @param shift  Bitshift (bits)
 * @param offset Sample value offset
 */
#define ENCODE(type, endian, src, dst, n, shift, offset)                \
    samples_ ## type = (const type *) src;                              \
    for (; n > 0; n--) {                                                \
        register type v = (*samples_ ## type++ >> shift) + offset;      \
        bytestream_put_ ## endian(&dst, v);                             \
    }

#define ENCODE_PLANAR(type, endian, dst, n, shift, offset)              \
    n /= avctx->channels;                                               \
    for (c = 0; c < avctx->channels; c++) {                             \
        int i;                                                          \
        samples_ ## type = (const type *) frame->extended_data[c];      \
        for (i = n; i > 0; i--) {                                       \
            register type v = (*samples_ ## type++ >> shift) + offset;  \
            bytestream_put_ ## endian(&dst, v);                         \
        }                                                               \
    }

/* encoder */
AVCodec ff_asif_encoder = {                                        
    .name         = "asif",                                                 
    .long_name    = NULL_IF_CONFIG_SMALL("ASIF audio file (CS 3505 Spring 20202)"),                      
    .type         = AVMEDIA_TYPE_AUDIO,                         
    .id           = AV_CODEC_ID_ASIF,                                    
    //.init         = pcm_encode_init,                                        
    //.encode2      = pcm_encode_frame,                                       
    .capabilities = AV_CODEC_CAP_VARIABLE_FRAME_SIZE,                       
    //.sample_fmts  = (const enum AVSampleFormat[]){ sample_fmt_,             
    //AV_SAMPLE_FMT_NONE },    
};

