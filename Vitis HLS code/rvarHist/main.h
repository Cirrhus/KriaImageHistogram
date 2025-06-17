#ifndef MAIN_H
#define MAIN_H

#include "ap_int.h"
#include <cstdint>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define MAX_WIDTH 4096
#define MAX_HEIGHT 4096
#define BINS 256

typedef ap_uint<8> pixel_t;
typedef ap_uint<32> hist_t;

// 32-bit AXI stream with 24-bit RGB data (0x00RRGGBB)
typedef ap_axiu<32,1,1,1> rgb_pixel_axis_t;
typedef ap_axiu<32,1,1,1> hist_axis_t;

void rgb_histogram(
    hls::stream<rgb_pixel_axis_t> &rgb_stream,
    hls::stream<hist_axis_t> &hist_stream,
    uint32_t width,
    uint32_t height
);
#endif
