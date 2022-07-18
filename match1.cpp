#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

double sum1 = 0, t = 0, temp1, temp2, PI = 3.1415926535;
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
float proc(float a, float alpha)
{
	a = pow(a, alpha);

	return a;
}
class shape
{
public:
	RotatedRect rect;
	double rate, rate2, slope, area, leng, height, width;
	Point2f p;
	vector<Point> contours;
	// vector<Vec4f>lines;
};
int main()
{
	VideoCapture cap(0);
	Mat a, b, mask, r1;

	Point p(-1, -1);
	int a1 = 13, b1 = 89, c = 176, d = 50, e = 255, f = 255, test = 0, point;
	int ax1 = 115, bx1 = 115;
	int ax2 = 115, ax3 = 120;
	while (1)
	{
		point = 0;
		cap.read(a);
		Mat c1 = a.clone();
		// b=a.clone();
		Mat temp = a.clone();
		for (int i = 0; i < 3; i++)
		{
			GaussianBlur(a, a, Size(5, 5), 0, 0);
		}
		int h = a.rows;
		int w = a.cols;
		float alpha1 = (float)ax1 / 100, bias = bx1;
		float alpha2 = (float)ax2 / 100, alpha3 = (float)ax3 / 100;
		Mat dst = Mat::zeros(a.size(), a.type());
		//
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
		b = dst.clone();
		cvtColor(dst, dst, CV_BGR2HSV);
		Scalar lower(a1, b1, c);
		Scalar upper(d, e, f);
		// Scalar lower(18, 25, 137);
		// Scalar upper(32, 247, 255);
		inRange(dst, lower, upper, a);
		Mat x = getStructuringElement(MORPH_RECT, Size(5, 1), p);
		Mat y = getStructuringElement(MORPH_RECT, Size(8, 1), p);
		Mat z = getStructuringElement(MORPH_RECT, Size(3, 3), p);
		b = a.clone();
		dilate(a, a, x, p, 1);
		dilate(a, a, y, p, 1);
		erode(a, a, x, p, 1);
		erode(a, a, y, p, 3);

		erode(a, a, z, p, 3);
		vector<vector<Point>> contours;
		vector<Vec4i> hier;
		vector<vector<Point>> contours2;
		vector<Vec4i> hier2;
		findContours(a, contours, hier, 1, 1);

		for (int i = 0; i < contours.size(); i++)
		{

			double area = contourArea(contours[i]);
			vector<vector<Point>> conPoly(contours.size());
			vector<Rect> boundRect(contours.size());
			string objectType;

			if (area > 8000)
			{
				double peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

				// cout << conPoly[i].size() << endl;
				boundRect[i] = boundingRect(conPoly[i]);

				// cout<<area<<endl;

				rectangle(a, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
				// cout<<"2"<<endl;
				double port;
				if (boundRect[i].height != 0 && boundRect[i].width != 0)
				// cout<<"3"<<boundRect[i].height<<"and"<<boundRect[i].width<<endl;

				{
					port = (double)boundRect[i].height / boundRect[i].width;
					//cout << port << endl;

					if (port < 1.2 && port > 0.6)
					{
						Mat r;
						// cout<<"1"<<endl;
						r = a(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height));
						r1 = c1(Rect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height));
						if ((r.empty()) /*&&*/)
						{
							// cout<<"error"<<endl;
						}
						else
						{
							resize(r, r, Size(400, 400), 1);
							// cout<<2<<endl;
							//  cvtColor(r, r, CV_BGR2GRAY);
							//   inRange(r,lowerFinal,upperFinal,mask);
							threshold(r, r, 150, 255, 1);

							Mat mat_mean, mat_stddev;
							meanStdDev(r, mat_mean, mat_stddev);
							t = mat_mean.at<double>(0, 0);
							double s = mat_stddev.at<double>(0, 0);
							// cout << t << endl;

							if (t < 80)
							{
								point = 1;
							}
							else
							{
								// Canny(r, r, 100, 300);
								findContours(a, contours2, hier2, 1, 1);
								for (int j = 0; j < contours2.size(); j++)
								{
									int area = contourArea(contours2[j]);
									// cout << area << endl;
									if (area > 3000)
									{
										for (int j = 0; j < contours2.size(); j++)
										{
											shape mine[contours2.size()];
											mine[j].rect = minAreaRect(contours2[j]);
											Point2f p[4];

											mine[j].rect.points(p);
											for (int i = 0; i < 4; i++)
											{
												line(r1, p[i], p[(i + 1) % 4], Scalar(0, 0, 255), 1);
											}

											double temp1 = pd(p[0], p[1]), temp2 = pd(p[0], p[3]);
											if (temp1 > temp2)
											{
												mine[j].height = temp1;
												mine[j].width = temp2;
											}
											if (temp2 > temp1)
											{
												mine[j].height = temp2;
												mine[j].width = temp1;
											}
											mine[j].rate = mine[j].height / mine[j].width;
											putText(r1, to_string(mine[j].rate), p[1], 1, 2, Scalar(255, 0, 0), 2, 8);
											mine[j].area = mine[j].height * mine[j].width;
											if (mine[j].rate > 1.4 && mine[j].rate < 1.8)
											{
												point = 10;
												//cout << mine[j].rate << endl;
											}
											else if (mine[j].rate < 1.1 && mine[j].area > 20000 && mine[j].area < 120000)
											{
												point = 100;
											}
											imshow("r", r);
											imshow("r1", r1);
										}
									}
								}
							}
							
						}
					}
					for (int j = 0; j < 500; j++)
					{
//cout<<j<<endl;//
//cout << sum1 << endl;
						sum1 += point;
						if (j == 499)
						{
							
							sum1 /= 500;
							cout << sum1 << endl;
							if ( sum1 < 15)

								cout << "kong" << endl;
							if (sum1 > 15.0)

								cout << "tiaoxingma" << endl;
							if (sum1 > 50)

								cout << "r" << endl;
							//cout << sum1 << "r" << endl;
							sum1 = 0;
						}
					}
				}
			}

			// drawContours(a,contours,-1,Scalar(30,255,255),5);
			imshow("a", a);
			waitKey(1);
		}
	}
	return 0;
}
