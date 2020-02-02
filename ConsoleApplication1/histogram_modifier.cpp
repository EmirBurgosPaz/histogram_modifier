
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat src, dst;

    /// Load image
    src = imread("../../histogram_modifier/histogram_test.jpg", IMREAD_GRAYSCALE);

    if (!src.data)
    {
        return -1;
    }

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split(src, bgr_planes);

    /// Establish the number of bins
    int histSize = 255;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 };
    const float* histRange = { range };
    float newRange[] = { 180, 256 };
    const float* BackRange = { newRange };

    bool uniform = true; bool accumulate = false;

    Mat histogramResult;

    /// Compute the histograms:
    calcHist(&src, 1, 0, Mat(), histogramResult, 1, &histSize, &histRange, uniform, accumulate);


    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(histogramResult, histogramResult, 0, histImage.rows, NORM_MINMAX, -1, Mat());


    /// Draw for each channel
    for (int i = 1; i < histSize; i++)
    {
        line(histImage,
            Point(bin_w * (i - 1), 
            hist_h - cvRound(histogramResult.at<float>(i - 1))),
            Point(bin_w * (i), 
            hist_h - cvRound(histogramResult.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);

    }

    Mat backproj;
    calcBackProject(&src, 1, 0, histogramResult, backproj, &BackRange, 1, true);

    /// Display
    namedWindow("calcHist Demo", WINDOW_AUTOSIZE);
    imshow("calcHist Demo", histImage);
    imshow("Redraw", backproj);
    imshow(" Demo", src);
    waitKey(0);

    return 0;
}