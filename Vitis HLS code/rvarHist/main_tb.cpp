#include <iostream>
#include "main.h"

int main() {
    const uint32_t WIDTH = 16;
    const uint32_t HEIGHT = 16;
    const uint32_t TOTAL_PIXELS = WIDTH * HEIGHT;

    hls::stream<rgb_pixel_axis_t> rgb_stream;
    hls::stream<hist_axis_t> hist_stream;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            rgb_pixel_axis_t pixel;
            pixel.data = (x << 16) | (y << 8) | ((x + y) % 256);
            pixel.keep = -1;
            pixel.strb = -1;
            pixel.last = (x == WIDTH-1 && y == HEIGHT-1);
            rgb_stream.write(pixel);
        }
    }

    rgb_histogram(rgb_stream, hist_stream, WIDTH, HEIGHT);

    std::cout << "Red Channel Histogram:\n";
    for (int i = 0; i < BINS; i++) {
        hist_axis_t val = hist_stream.read();
        if (val.data > 0) {
            std::cout << "Bin " << i << ": " << val.data << "\n";
        }
    }

    std::cout << "\nGreen Channel Histogram:\n";
    for (int i = 0; i < BINS; i++) {
        hist_axis_t val = hist_stream.read();
        if (val.data > 0) {
            std::cout << "Bin " << i << ": " << val.data << "\n";
        }
    }

    std::cout << "\nBlue Channel Histogram:\n";
    for (int i = 0; i < BINS; i++) {
        hist_axis_t val = hist_stream.read();
        if (val.data > 0) {
            std::cout << "Bin " << i << ": " << val.data;
            if (i == BINS-1) std::cout << " (LAST)";
            std::cout << "\n";
        }
    }

    std::cout << "Test completed!\n";
    return 0;
}
