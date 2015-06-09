
// Coherence Line Drawing standalone executable
//
// Source code, modified from the paper 'Coherent Line Drawing' by Kang et al, Proc. NPAR 2007,
//   to be a standalone program. The project website for that paper is here:
//   http://cg.postech.ac.kr/research/coherent_line/
//
// Install OpenCV and compile with: g++ *.cpp -o main `pkg-config --libs opencv`

#include "opencv2/opencv.hpp"

#include "imatrix.h"
#include "ETF.h"
#include "fdog.h"
#include "myvec.h"

using cv::Mat;

// Code from: https://github.com/kylemcdonald/ofxCv/blob/master/libs/ofxCv/include/ofxCv/Wrappers.h
// and the paper website: http://cg.postech.ac.kr/research/coherent_line/
//
// coherent line drawing: good values for halfw are between 1 and 8,
// smoothPasses 1, and 4, sigma1 between .01 and 2, sigma2 between .01 and 10,
// tau between .8 and 1.0
// this could be rewritten into a class so we're not doing an allocate and copy each time
void CLD(Mat src, Mat &dst, int halfw = 4, int smoothPasses = 2, double sigma1 = .4, double sigma2 = 3, double tau = .99, int black = 0) {
    src.copyTo(dst);
    int width = src.cols, height = src.rows;
    imatrix img;
    img.init(height, width);
    if(black != 0) {
        add(dst, cv::Scalar(black), dst);
    }
    // copy from dst (unsigned char) to img (int)
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            img[y][x] = dst.at<unsigned char>(y, x);
        }
    }
    ETF etf;
    etf.init(height, width);
    etf.set(img);
    etf.Smooth(halfw, smoothPasses);
    GetFDoG(img, etf, sigma1, sigma2, tau);
    // copy result from img (int) to dst (unsigned char)
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            dst.at<unsigned char>(y, x) = img[y][x];
        }
    }
}

int main(int argc, char *argv[]) {
    argc--;
    argv++;
    if (argc < 2) {
        fprintf(stderr, "main in.png out.png\n"); exit(1);
    }
    Mat image;
    image = cv::imread(argv[0], CV_LOAD_IMAGE_GRAYSCALE);

    Mat out_image;
    CLD(image, out_image);

    cv::imwrite(argv[1], out_image);
    return 0;
}
