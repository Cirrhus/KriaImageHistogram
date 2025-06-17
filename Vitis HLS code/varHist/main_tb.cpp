#include "main.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void generate_input(hls::stream<pixel_axis_t> &input_stream, uint32_t width, uint32_t height) {
    srand(time(0));
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            pixel_axis_t pix;
            pix.data = rand() % 256; // Random 8-bit grayscale pixel
            pix.keep = -1;
            pix.strb = -1;
            pix.user = (y == 0 && x == 0) ? 1 : 0; // Start of stream
            pix.last = (y == height - 1 && x == width - 1) ? 1 : 0; // End of stream
            pix.id = 0;
            pix.dest = 0;
            input_stream.write(pix);
        }
    }
}

void print_histogram(hls::stream<hist_axis_t> &output_stream) {
    for (int i = 0; i < BINS; ++i) {
        if (!output_stream.empty()) {
            hist_axis_t out_pkt = output_stream.read();
            std::cout << "Bin[" << i << "] = " << out_pkt.data.to_uint() << std::endl;
        } else {
            std::cout << "Bin[" << i << "] = (missing output)" << std::endl;
        }
    }
}

int main() {
    const uint32_t width = 2048;
    const uint32_t height = 2048;

    hls::stream<pixel_axis_t> input_stream;
    hls::stream<hist_axis_t> output_stream;

    generate_input(input_stream, width, height);

    doHist(input_stream, output_stream, width, height);

    print_histogram(output_stream);

    return 0;
}
