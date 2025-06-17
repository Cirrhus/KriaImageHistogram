#include "hhist.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<8,1,1,1> pixel_axis_t;
typedef ap_axiu<32,1,1,1> hist_axis_t;

void doHist(hls::stream<pixel_axis_t> &input_stream,
            hls::stream<hist_axis_t> &output_stream) {
#pragma HLS INTERFACE axis port=input_stream
#pragma HLS INTERFACE axis port=output_stream
#pragma HLS INTERFACE s_axilite port=return bundle=control

    hist_t local_hist[BINS];
#pragma HLS ARRAY_PARTITION variable=local_hist complete dim=1

    for (int i = 0; i < BINS; i++) {
#pragma HLS UNROLL
        local_hist[i] = 0;
    }

    for (int i = 0; i < IMG_SIZE; i++) {
#pragma HLS PIPELINE II=1
        pixel_axis_t pix = input_stream.read();
        local_hist[pix.data]++;
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
