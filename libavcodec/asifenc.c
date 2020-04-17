/**
 * ASIF audio encoder
 */

/*
 * WavPack lossless audio encoder
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

#define BITSTREAM_WRITER_LE

#include "libavutil/intreadwrite.h"
#include "libavutil/opt.h"
#include "avcodec.h"
#include "internal.h"
#include "put_bits.h"
#include "bytestream.h"

typedef struct asif_encoder_data{
	
	int num_of_samples;
	int num_of_channels;
	int last_frame_received; //Last frame flag
	uint8_t *arr;
	int x;
	double y;
	
	asif_encoder_data *other;
}asif_encoder_data;

static av_cold int asif_encode_init(AVCodecContext *avctx)
{
    asif_encoder_data *my_data;

    my_data = avctx->priv_data;
    my_data->x = 42;
    my_data->y = 3.7;
    my_data->other = NULL;
	my_data-> last_frame_received = 0;
  
    av_log(avctx, AV_LOG_INFO, "executing inside of asif_encode_int\n");

    avctx->frame_size = 1000000;
    
    return 0;
}

static int asif_send_frame(AVCodecContext *avctx, const AVFrame *frame){
	
	asif_encoder_data *my_data;
	my_data = avctx->priv_data;
	
	//Those indexes in the priv_data will be filled with buffer
	
    //creates buf to hold all frames? 
	uint8_t *buf = av_malloc(priv_data_size);
	
	//allocate space to hold *bufs
    avctx->priv_data = av_malloc(priv_data_size);

	//When receive a NULL frame from frame, throw flag
	if(frame == NULL)
		my_data->last_frame_received = 1; //Sets flag to true;
		
	//Add buf to memory location so that receive_packets can access it
	avctx->priv_data = buf;
	
	return 0;
	
}

static int asif_receive_packet(AVCodecContext *avctx, AVPacket *avpkt){
	asif_encoder_data *my_data;
	my_data = avctx->priv_data;
	
	if(!my_data->last_frame_received)
		return AVERROR(EAGAIN);
	
	//prep packet using frames from send_frames
	
	return 0;
}

static int asif_encode_frame(AVCodecContext *avctx, AVPacket *avpkt,
                                const AVFrame *frame, int *got_packet_ptr)
{
    int ret;
    uint8_t *buf;
    
    av_log(avctx, AV_LOG_INFO, "executing inside of asif_encode_frame\n");
    
    if ((ret = ff_alloc_packet2(avctx, avpkt, 16, 16)) < 0)
     return ret;

    buf = avpkt->data;

    buf[0] = 'Y';
    buf[1] = 'e';
    buf[2] = 'e';
    buf[3] = 't';

    for (int i = 4; i < 16; i++)
      buf[i] = ' ';
    
    *got_packet_ptr = 1;
    return 0;
}

static av_cold int asif_encode_close(AVCodecContext *avctx)
{

  av_log(avctx, AV_LOG_INFO, "executing inside of asif_encode_close\n");
  
    return 0;
}

AVCodec ff_asif_encoder = {
    .name           = "asif",
    .priv_data_size = sizeof(asif_encoder_data), //Allocates 20 bytes of memory
    .long_name      = NULL_IF_CONFIG_SMALL("ASIF audio file (CS 3505 Spring 20202)"),
    .type           = AVMEDIA_TYPE_AUDIO,
    .id             = AV_CODEC_ID_ASIF,
    .init           = asif_encode_init,
    //.encode2        = asif_encode_frame,
    .send_frame     = asif_send_frame,
    .receive_packet = asif_receive_packet,
    .close          = asif_encode_close,
    .capabilities   = AV_CODEC_CAP_SMALL_LAST_FRAME,
    .sample_fmts    = (const enum AVSampleFormat[]){ AV_SAMPLE_FMT_U8P,
                                                     AV_SAMPLE_FMT_NONE },
};


