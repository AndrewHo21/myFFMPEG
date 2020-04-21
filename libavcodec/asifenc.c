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

/* Struct used to hold private vairables so that data can be accessed in the functions
 */
typedef struct asif_encoder_data{
	
	int num_of_samples;
	int num_of_channels;
	int last_frame_received; //Last frame flag
	uint8_t *arr;
	int x;
	double y;
	
	//sif_encoder_data *other;
}asif_encoder_data;

/*Initializes the data that is stored in the asif_encoder_data struct
 *and initializes any private variable that is being used in AVCodec and AVCodecContext
 */
static av_cold int asif_encode_init(AVCodecContext *avctx)
{
    asif_encoder_data *my_data;

    my_data = avctx->priv_data;
    my_data->x = 42;
    my_data->y = 3.7;
    //my_data->other = NULL;
	my_data-> last_frame_received = 0;
  
    av_log(avctx, AV_LOG_INFO, "executing inside of asif_encode_int\n");

    avctx->frame_size = 1000000;
    
    return 0;
}

/* Uses frames that are taken from an audio file and data to a private data structure so that it can be accessed by asif_recieve_frames
 */
static int asif_send_frame(AVCodecContext *avctx, const AVFrame *frame){
	
	asif_encoder_data *my_data;
	my_data = avctx->priv_data;
	
	//Those indexes in the priv_data will be filled with buffer
	
    //creates buf to hold all frames? 
	uint8_t *buf = av_malloc(sizeof(asif_encoder_data));
	
	//allocate space to hold *bufs
    avctx->priv_data = av_malloc(sizeof(asif_encoder_data));

	//When receive a NULL frame from frame, throw flag
	if(frame == NULL)
		my_data->last_frame_received = 1; //Sets flag to true;
		
	//Add buf to memory location so that receive_packets can access it
	avctx->priv_data = buf;
	
	return 0;
	
}

/* Will access the private data structure that was allocated in asif_send_frames, 
 * and will take each frame and add it to a packet the correct order that each frame was add.
 */
static int asif_receive_packet(AVCodecContext *avctx, AVPacket *avpkt){
	asif_encoder_data *my_data;
	my_data = avctx->priv_data;
	
	//Sends AVERROR(EAGAIN) if there are still frames missing from the data structure, or if the frames received aren't completed
	if(!my_data->last_frame_received)
		return AVERROR(EAGAIN);
	
	//prep packet using frames from send_frames
	
	return 0;
}

/* Ends the process of encoding data
 */
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
    .send_frame     = asif_send_frame,
    .receive_packet = asif_receive_packet,
    .close          = asif_encode_close,
    .capabilities   = AV_CODEC_CAP_SMALL_LAST_FRAME,
    .sample_fmts    = (const enum AVSampleFormat[]){ AV_SAMPLE_FMT_U8P,
                                                     AV_SAMPLE_FMT_NONE },
};


