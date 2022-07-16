#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;
Mat r,ra;
double PI = 3.1415926535, rate2, panduan;
double sl(Point2f &a, Point2f &b);
double pd(Point2f &a, Point2f &b);
double rate(double a, double b);
double height, width;
class shape
{
public:
    RotatedRect rect;
    double rate, rate2, slope, area, leng, height, width;
    Point2f p;
    vector<Point> contours;
};

int main()
{ /*
     int a1 = 0, b1 = 0, c = 0, d = 0, e = 0, f = 0;

     namedWindow("BAR");
     createTrackbar("a1", "BAR", &a1, 179);
     createTrackbar("b", "BAR", &b1, 255);
     createTrackbar("c", "BAR", &c, 255);
     createTrackbar("d", "BAR", &d, 179);
     createTrackbar("e", "BAR", &e, 255);
     createTrackbar("f", "BAR", &f, 255);*/
    VideoCapture cap(0);
    Mat a, b, mask, rect_j, matrix;

    Point p(-1, -1);
    while (1)
    {
        cap.read(a);
        b = a.clone();
        Scalar lower(98, 51, 119);
        Scalar upper(179, 255, 255); /*
        Scalar lower(a1, b1, c);
        Scalar upper(d, e, f);*/

        // GaussianBlur(a, a, Size(5, 5), 0, 0);
        cvtColor(a, a, CV_BGR2HSV);
        inRange(a, lower, upper, mask);

        Mat x = getStructuringElement(MORPH_RECT, Size(3, 3), p);
        dilate(mask, a, x, p, 1);
        erode(a, a, x, p, 1);
        vector<vector<Point>> contours;
        vector<Vec4i> hier;
        vector<vector<Point>> contours2;
        vector<Vec4i> hier2;
        findContours(a, contours, hier, 1, CHAIN_APPROX_NONE);

        for (int i = 0; i < contours.size(); i++)
        {

            double area = contourArea(contours[i]);
            vector<vector<Point>> conPoly(contours.size());
            vector<Rect> boundRect(contours.size());
            string objectType;

            if (area > 5000)
            {
                double peri = arcLength(contours[i], true);
                approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

                // cout << conPoly[i].size() << endl;
                boundRect[i] = boundingRect(conPoly[i]);

                // cout<<area<<endl;

                //rectangle(a, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
                
                r = a(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height));
                //ra = b(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height));

                double port;
                if (boundRect[i].height != 0 && boundRect[i].width != 0)
                    ;
                {
                    port = boundRect[i].height / boundRect[i].width;
                }

                if ((r.empty()) /*&&*/)
                {
                    // cout<<"error"<<endl;
                }
                else if (port < 1.10 || port > 0.9)
                {
                    resize(r, r, Size(400, 400), 1);
                }
            }
        }

        findContours(r, contours2, hier2, 1, CHAIN_APPROX_NONE);

        for (int j = 0; j < contours2.size(); j++)
        {

            shape right[contours2.size()];
            right[j].rect = minAreaRect(contours2[j]);
            Point2f px[4];
            right[j].rect.points(px);
             for (int j = 0; j < 4; j++)
                    {
                        line(r, px[j], px[(j + 1) % 4], Scalar(0, 0, 255), 1);
                    }
            right[j].area = contourArea(contours2[j]);

            if (right[j].area > 3000&&right[j].area<80000)
            {

                right[j].rate = rate(right[j].height, right[j].width);
                //cout << right[j].rate << endl;
                if (right[j].rate > 1.2)
                {
                   
                    right[j].rect.points(px);
                    Point2f p1 = (px[0] + px[3]) / 2, p2 = (px[2] + px[1]) / 2;
                    
                    double temp1 = pd(px[0], px[1]), temp2 = pd(px[0], px[3]);
                    if (temp1 > temp2)
                    {
                        right[j].slope = sl(px[0], px[1]);
                        line(a, px[0], px[3], Scalar(0, 255, 0), 1, 8);
                        // right[j].height=temp1,right[j].width=temp2;
                    }
                    else if (temp2 > temp1)
                    {
 
                        right[j].slope = sl(px[0], px[3]);
                        // right[j].height=temp2,right[j].width=temp1;
                    }

                    putText(r, to_string(right[j].rateg), px[1], 1, 2, Scalar(0, 255, 0), 2, 8);
                    cout << right[j].slope << endl;
                }
            }
        }
        if (r.empty())
        {
            cout << "error" << endl;
        }
        else
            imshow("r", r);

        imshow("a", b);
        // imshow("b", b);

        waitKey(1);
    }

    return 0;
}
double sl(Point2f &a, Point2f &b)
{
    double slope = (b.y - a.y) / (b.x - a.x);
    double tan = atan(slope);
    return tan / PI * 180;
}
double pd(Point2f &a, Point2f &b)
{
    double res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    return res;
}
double rate(double a, double b)
{
    double rate;

    if (a > b)
        rate = a / b;
    else if (b < a)
        rate = b / a;
    return rate;
}