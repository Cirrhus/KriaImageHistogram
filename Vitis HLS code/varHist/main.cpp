#include "main.h"

void doHist(hls::stream<pixel_axis_t> &input_stream,
            hls::stream<hist_axis_t> &output_stream,
            uint32_t width,
            uint32_t height) {
#pragma HLS INTERFACE axis port=input_stream
#pragma HLS INTERFACE axis port=output_stream
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=width bundle=control
#pragma HLS INTERFACE s_axilite port=height bundle=control

    if (width > MAX_WIDTH) width = MAX_WIDTH;
    if (height > MAX_HEIGHT) height = MAX_HEIGHT;

    hist_t local_hist[BINS];
#pragma HLS ARRAY_PARTITION variable=local_hist complete dim=1

    for (int i = 0; i < BINS; i++) {
#pragma HLS UNROLL
        local_hist[i] = 0;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
#pragma HLS PIPELINE II=1
            pixel_axis_t pix = input_stream.read();
            if (pix.data < BINS) {
                local_hist[pix.data]++;
            }
        }
    }

    for (int i = 0; i < BINS; i++) {
#pragma HLS PIPELINE II=1
        hist_axis_t out_pkt;
        out_pkt.data = local_hist[i];
        out_pkt.keep = -1;
        out_pkt.strb = -1;
        out_pkt.user = (i == 0) ? 1 : 0;
        out_pkt.last = (i == BINS - 1) ? 1 : 0;
        out_pkt.id = 0;
        out_pkt.dest = 0;
        output_stream.write(out_pkt);
    }
}
