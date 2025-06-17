#ifndef MAIN_H
#define MAIN_H

#include <cstdint>
#include "ap_int.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define MAX_WIDTH 4096
#define MAX_HEIGHT 4096
#define BINS 256

typedef ap_uint<8> pixel_t;
typedef ap_uint<32> hist_t;

typedef ap_axiu<8,1,1,1> pixel_axis_t;
typedef ap_axiu<32,1,1,1> hist_axis_t;

void doHist(hls::stream<pixel_axis_t> &input_stream,
            hls::stream<hist_axis_t> &output_stream,
            uint32_t width,
            uint32_t height);

#endif
