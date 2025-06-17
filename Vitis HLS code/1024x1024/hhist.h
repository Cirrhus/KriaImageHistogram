#ifndef HHIST_H
#define HHIST_H

#include "ap_int.h"

#define IMG_WIDTH 1024
#define IMG_HEIGHT 1024
#define IMG_SIZE (IMG_WIDTH * IMG_HEIGHT)
#define BINS 256

typedef ap_uint<8> pixel_t;
typedef ap_uint<32> hist_t;

#include <hls_stream.h>
#include <ap_axi_sdata.h>
typedef ap_axiu<8,1,1,1> pixel_axis_t;
typedef ap_axiu<32,1,1,1> hist_axis_t;

void doHist(hls::stream<pixel_axis_t> &input_stream,
            hls::stream<hist_axis_t> &output_stream);

#endif
