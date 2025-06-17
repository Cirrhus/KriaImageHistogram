#include "main.h"

void rgb_histogram(
    hls::stream<rgb_pixel_axis_t> &rgb_stream,
    hls::stream<hist_axis_t> &hist_stream,
    uint32_t width,
    uint32_t height)
{
#pragma HLS INTERFACE axis port=rgb_stream
#pragma HLS INTERFACE axis port=hist_stream
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=width bundle=control
#pragma HLS INTERFACE s_axilite port=height bundle=control

    width = (width > 4096) ? 4096 : width;
    height = (height > 4096) ? 4096 : height;

    hist_t hist_r[BINS];
    hist_t hist_g[BINS];
    hist_t hist_b[BINS];
#pragma HLS ARRAY_PARTITION variable=hist_r cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=hist_g cyclic factor=8 dim=1
#pragma HLS ARRAY_PARTITION variable=hist_b cyclic factor=8 dim=1
#pragma HLS RESOURCE variable=hist_r core=RAM_T2P_BRAM
#pragma HLS RESOURCE variable=hist_g core=RAM_T2P_BRAM
#pragma HLS RESOURCE variable=hist_b core=RAM_T2P_BRAM

    INIT_LOOP:
    for (int i = 0; i < BINS; i++) {
#pragma HLS UNROLL factor=8
        hist_r[i] = 0;
        hist_g[i] = 0;
        hist_b[i] = 0;
    }

    PIXEL_LOOP:
    for (int y = 0; y < height; y++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=4096
        for (int x = 0; x < width; x++) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=4096
#pragma HLS PIPELINE II=2
            rgb_pixel_axis_t pix = rgb_stream.read();

            ap_uint<8> r = pix.data(23,16);
            ap_uint<8> g = pix.data(15,8);
            ap_uint<8> b = pix.data(7,0);

            hist_r[r] += 1;
            hist_g[g] += 1;
            hist_b[b] += 1;
        }
    }

    OUTPUT_R_LOOP:
    for (int i = 0; i < BINS; i++) {
#pragma HLS PIPELINE II=1
        hist_axis_t pkt;
        pkt.data = hist_r[i];
        pkt.last = 0;
        pkt.keep = -1;
        pkt.strb = -1;
        hist_stream.write(pkt);
    }

    OUTPUT_G_LOOP:
    for (int i = 0; i < BINS; i++) {
#pragma HLS PIPELINE II=1
        hist_axis_t pkt;
        pkt.data = hist_g[i];
        pkt.last = 0;
        pkt.keep = -1;
        pkt.strb = -1;
        hist_stream.write(pkt);
    }

    OUTPUT_B_LOOP:
    for (int i = 0; i < BINS; i++) {
#pragma HLS PIPELINE II=1
        hist_axis_t pkt;
        pkt.data = hist_b[i];
        pkt.last = (i == BINS-1);
        pkt.keep = -1;
        pkt.strb = -1;
        hist_stream.write(pkt);
    }
}
