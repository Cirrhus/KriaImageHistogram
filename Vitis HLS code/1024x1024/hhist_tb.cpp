#include <iostream>
#include "hhist.h"
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<8,1,1,1> pixel_axis_t;
typedef ap_axiu<32,1,1,1> hist_axis_t;

int main() {
    hls::stream<pixel_axis_t> input_stream;
    hls::stream<hist_axis_t> output_stream;

    for (int i = 0; i < IMG_SIZE; i++) {
        pixel_axis_t pix;
        pix.data = i % 256;
        pix.keep = -1;
        pix.strb = -1;
        pix.user = (i == 0) ? 1 : 0;
        pix.last = (i == IMG_SIZE - 1) ? 1 : 0;
        pix.id = 0;
        pix.dest = 0;
        input_stream.write(pix);
    }

    doHist(input_stream, output_stream);

    for (int i = 0; i < BINS; i++) {
        hist_axis_t out_pix = output_stream.read();
        std::cout << "hist[" << i << "] = " << out_pix.data << std::endl;
        if (out_pix.last == 1 && i != BINS - 1) {
            std::cout << "Ostrze¿enie: last flag przed koñcem strumienia!" << std::endl;
            break;
        }
    }

    return 0;
}
