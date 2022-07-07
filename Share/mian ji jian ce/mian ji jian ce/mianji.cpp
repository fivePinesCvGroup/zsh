#include<iostream>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/highgui.hpp>

#include<string>
#include <opencv2/highgui.hpp>  
#include <opencv2\core\core.hpp>
#include<math.h>
using namespace std;
using namespace cv;
int i=0,j=0;
int s1 = 0, s2 = 0, s4 = 0,s5=0,s6=0;
Mat actu, actu1, actu2;
string cha;
Mat elementX = getStructuringElement(MORPH_RECT, Size(1, 1));
Mat elementY = getStructuringElement(MORPH_RECT, Size(1, 1));
Point point(-1, -1);
int main() {
	VideoCapture cap(0);
	Mat actu,img;
	
	while (true)
	{
		cap.read(img);
		imshow("a", img);
		resize(img, img, Size(400, 400), INTER_NEAREST);
		cvtColor(img, img, 7);
		for (int i = 0; i < 3; i++)
		{
			medianBlur(img, img, 3);
		}
		actu = img.clone();
		actu = img(Rect(90, 50, 270, 340));
		resize(actu, actu, Size(400, 400), 0);
		Mat roi = actu.clone(), roi_canny;
		roi = actu(Rect(275, 285, 100, 115));
		imshow("%2", roi);
		Canny(actu, actu, 50, 125, 3);
		erode(actu, actu, elementY, point, 1);
		erode(actu, actu, elementX, point, 1);
		dilate(actu, actu, elementX, point, 1);
		dilate(actu, actu, elementY, point, 2);
		vector<vector<Point>> contours;
		vector<vector<Point>> contours2;
		vector<Vec4i> hierarchy, hierarchy2;
		findContours(actu, contours, hierarchy, RETR_EXTERNAL, 1, Point(10, 10));
		findContours(roi, contours2, hierarchy2, RETR_EXTERNAL, 1, Point(1, 1));
		for (int i = 0; i < contours.size(); i++)

		{

			double g_dConArea = contourArea(contours[i], false);
			//cout << g_dConArea << endl;

			s1 += fabs(g_dConArea);                         //整个的面积
			
		}
		//if (s1 > 24000) {
		//	int j = 0;
		//	
		//	for (int i = 0; i < contours2.size(); i++)

		//	{

		//		double g_dConArea = contourArea(contours2[i], true);
		//		//cout << g_dConArea << endl;
		//		s5 += fabs(g_dConArea);                    //roi的面积

		//	}
		//	if (j < 20)
		//	{
		//		j++;
		//		s6 += s5;
		//	}
		//	if (j == 20) 
		//	{
		//		s6 /= 20;
		//		if (s6 > 3000) {
		//			cha = "正";
		//		}
		//		else if (s6 < 3000)
		//		{
		//			cha = "反";
		//		}
		//		cout << "s5=" << s6 << endl;
		//		s6 = 0,j=0;
		//	}
		//	s5 = 0;
		//}
		if (i < 20)
		{
			i++;
			s2 += s1;
		}
		if (i == 20)
		{

			int s3 = s2 / 20;
			if (s3 > 3000 && s3 < 10000)
			{
				cout << "该面为 ：空面" << endl;
			}
			if (s3 > 10000 && s3 < 24000)
			{
				cout << "该面为 ：条形码面" << endl;
			}
			if (s3 > 24000 && s3 < 60000)
			{
				cout << "该面为 ：r" << cha << "面" << endl;
			}
			cout << "面积为:" << s3 << endl;
			
			s2 = 0;
			i = 0;
		}
		//cout << s1 << endl;
		imshow("%", actu);
		s1 = 0;
		waitKey(10);
	}
	





	return 0;
}