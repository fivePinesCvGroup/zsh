#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;
double PI = 3.1415926535, panduan1 = 0, panduan2 = 0;
Mat channel[3];
int t1, t2, panduan4;
class light
{
public:
	RotatedRect rect;
	double slpoe = 1;
	int area = 1, rate = 1;

	int flag = 1;
	int height = 1;
	int width = 1;
	Point2f P;
};
float proc(float a, float alpha)
{
	a = pow(a, alpha);

	return a;
}
double pd(Point2f &a, Point2f &b)
{
	double res = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	return res;
}

double sl(Point2f &a, Point2f &b)
{
	double slope = (b.y - a.y) / (b.x - a.x);
	double tan = atan(slope);
	return tan / PI * 180;
}
int main()
{
	VideoCapture cap(0);
	/*int a1=0,b1=0;
	int a2=0,a3=0;
	namedWindow("bars",(800,400));
	createTrackbar("a1","bars",&a1,300);
	createTrackbar("a2","bars",&a2,300);
	createTrackbar("a3","bars",&a3,300);
	createTrackbar("b1","bars",&b1,2550);
	int ax=0,bx=0,cx=0,dx=0,ex=0,fx=0;
	namedWindow("BAR",(800,400));

	createTrackbar("a","BAR",&ax,179);
	createTrackbar("b","BAR",&bx,255);
	createTrackbar("c","BAR",&cx,255);
	createTrackbar("d","BAR",&dx,179);
	createTrackbar("e","BAR",&ex,255);
	createTrackbar("f","BAR",&fx,255);*/
	int a1 = 0, a2 = 74, a3 = 93, b1 = 19;
	int ax = 0, bx = 37, cx = 141, dx = 1, ex = 91, fx = 164;
	while (1)
	{
		Mat a, b, dst;
		cap.read(a);
		GaussianBlur(a, a, Size(5, 5), 0, 0);
		b = a.clone();
		int h = a.rows;
		int w = a.cols;
		float alpha1 = (float)a1 / 100, bias = b1;
		float alpha2 = (float)a2 / 100, alpha3 = (float)a3 / 100;
		// cvtColor(a,a,CV_BGR2HSV);
		dst = Mat::zeros(a.size(), a.type());
		cvtColor(dst, dst, CV_BGR2HSV);
		for (int row = 0; row < h; row++)
		{
			for (int col = 0; col < w; col++)
			{
				if (a.channels() == 3)
				{
					float b = a.at<Vec3b>(row, col)[0];
					float g = a.at<Vec3b>(row, col)[1];
					float r = a.at<Vec3b>(row, col)[2];

					dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(proc(b, alpha1) - bias);
					dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(proc(g, alpha2) - bias);
					dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(proc(r, alpha3) - bias);
				}
			}
		}

		Scalar lower(ax, bx, cx);
		Scalar upper(dx, ex, fx);
		inRange(dst, lower, upper, a);

		waitKey(1);
		Point pxx(-1, -1);
		Mat x = getStructuringElement(MORPH_RECT, Size(5, 5), pxx);
		dilate(a, a, x, pxx, 1);
		erode(a, a, x, pxx, 1);

		vector<vector<Point>> contours;
		vector<Vec4i> hier;
		findContours(a, contours, hier, 1, CHAIN_APPROX_NONE);
		// drawContours(b, contours, -1, Scalar(0, 255, 0), 5);
		cvtColor(a, a, COLOR_GRAY2BGR);
		for (int i = 0; i < contours.size(); i++)
		{
			light l[contours.size()];
			double area = contourArea(contours[i]);
			l[i].area = area;

			if (area < 20000 && area > 300)
			{

				// vector<light> l;
				l[i].rect = minAreaRect(contours[i]);
				Point2f p[4];

				l[i].rect.points(p);

				Point2f p1 = (p[0] + p[3]) / 2;
				Point2f p2 = (p[1] + p[2]) / 2;

				// circle(b, p1, 5, Scalar(255, 0, 0), 5, 8);
				float height = pd(p[0], p[1]), width = pd(p[0], p[3]);
				if (width > height)
				{
					float temp;
					temp = width;
					width = height;
					height = temp;
				}
				l[i].height = height;
				l[i].width = width;
				float rate = (float)height / width;
				// cout<<"a"<<rate<<endl;
				if (rate < 8 && rate > 2.5 || rate < 0.4 && rate > 0.125)
				{
					l[i].flag = 1;
					putText(b, to_string(l[i].flag), p[0], 1, 2, Scalar(0, 0, 255), 2, 8);
					// cout << rate << endl;
					string a = to_string(rate);
					l[i].slpoe = sl(p[0], p[2]);

					for (int j = 0; j < contours.size(); j++)
					{
						double area = contourArea(contours[j]);
						l[j].area = area;
						string a = to_string(rate);
						l[j].rect = minAreaRect(contours[j]);
						Point2f px[4];
						l[j].rect.points(px);
						Point2f px1 = (px[0] + px[3]) / 2;
						Point2f px2 = (px[1] + px[2]) / 2;
						l[j].slpoe = sl(px[0], px[2]);
						if (l[i].flag == 1 && l[j].flag == 1 && j != i)
						{
							// cout << "?" << endl;
							if (l[i].area != 0 && l[j].area != 0 && l[i].slpoe != 0 && l[j].slpoe != 0)
							{
								float temp1 = (double)l[i].area / l[j].area;
								float temp2 = l[i].slpoe / l[j].slpoe;
								if (temp1 < 1.5 && temp1 > 0.7 && temp2 < 2 && temp2 > 0.5)
								{

									// if(temp2 < 1.5 && temp2 > 0.66){
									// cout << temp2 << "and" << l[i].slpoe << "and" << l[j].slpoe << "and" << j - i << endl;
									// for (int j = 0; j < 4; j++)
									//{
									//	line(b, p[j], p[(j + 1) % 4], Scalar(0, 0, 255), 1);
									//}

									float height2 = pd(px[0], px[1]), width2 = pd(px[0], px[3]);
									if (width2 > height2)
									{
										float temp;
										temp = width2;
										width2 = height2;
										height2 = temp;
									}
									l[j].height = height2;

									panduan1 = pd(l[i].rect.center, l[j].rect.center);
									putText(b, to_string(panduan1), (l[i].rect.center + l[j].rect.center) / 2, 1, 2, Scalar(0, 255, 0), 2, 8);
									panduan2 = panduan1 / ((l[i].height + l[j].height) / 2);
									if (panduan2 < 6)
									{
										line(b, p1, p2, Scalar(0, 0, 255), 1);
										putText(b, to_string(l[i].slpoe), p[0], 1, 2, Scalar(0, 0, 255), 2, 8);
										// putText(b, to_string(l[i].area), p[2], 1, 2, Scalar(0, 0, 255), 2, 8);
										putText(b, to_string(l[j].slpoe), p[2], 1, 2, Scalar(0, 255, 0), 2, 8);
										line(b, l[i].rect.center, l[j].rect.center, Scalar(0, 255, 0), 3);
										circle(b, l[i].rect.center, 5, Scalar(255, 0, 0), 5, 8);
										circle(b, l[j].rect.center, 5, Scalar(255, 0, 0), 5, 8);
										if ((l[i].slpoe + l[j].slpoe) / 2 > 0)
										{
											l[i].P = p[3];
										}
										else if ((l[i].slpoe + l[j].slpoe) / 2 < 0)
										{
											l[i].P = p[2];
										}
										if (((int)l[i].P.x + l[i].width) < (b.cols - 10) && (int)l[i].P.y + l[i].height < (b.rows - 10))
										{
											Mat pandaun3_1 = b(Rect((int)l[i].P.x + 1, (int)l[i].P.y + 1, l[i].width + 1, l[i].height + 1));
											circle(b, l[i].P, 5, Scalar(255, 255, 0), 5, 8);
											split(pandaun3_1, channel);
											Mat mat_mean1, mat_mean2, mat_temp;
											meanStdDev(channel[0], mat_mean1, mat_temp);
											meanStdDev(channel[2], mat_mean2, mat_temp);
											t1 = mat_mean1.at<double>(0, 0), t2 = mat_mean2.at<double>(0, 0);
											cout << t1 << "and" << t2 << endl;
											panduan4 = t1 - t2;
										}
										// imshow("c", channel[0]);
									}
									cout << panduan2 << endl;

									// cout << "1" << endl;
								}
							}
						}
					}
				}
			}

			imshow("b", b);

			// imshow("r",a);
			// imshow("a",a);
			// imshow("dst",dst);
		}
		string str;
		if (panduan4 > 0)
		{
			str = "blue ";
		}
		else if (panduan4 < 0)
		{
			str = "red ";
		}
		if (panduan2 > 3)
		{
			cout << str << "big armor" << endl;
			panduan2 = 0;
		}
		else if (panduan2 < 3)
		{
			cout << str << "small armor" << endl;
			panduan2 = 0;
		}
	}
	//}

	return 0;
}
