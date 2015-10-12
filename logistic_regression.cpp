/***************************************************************
 > File Name: logistic_regression.cpp
 > Author: Bluuuuer
 **************************************************************/
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <cmath>
#include <time.h>

using namespace std;
using namespace cv;

void plot(Mat & image){
	Point o_point,x_point,y_point;
	o_point.x = 50;
	o_point.y = 550;
	x_point.x = 550;
	x_point.y = 550;
	y_point.x = 50;
	y_point.y = 50;

	line(image,o_point,x_point,255,1,LINE_AA);
	line(image,o_point,y_point,255,1,LINE_AA);

	Point t_0_0;
	t_0_0.x = 30;
	t_0_0.y = 570;

	putText(image,"O",t_0_0,1,1,255);

	Point t_50_0,t_100_0,t_150_0,t_200_0,t_250_0;
	t_50_0.x = 150;
	t_50_0.y = t_100_0.y = t_150_0.y = t_200_0.y = t_250_0.y = 570;
	t_100_0.x = 250;
	t_150_0.x = 350;
	t_200_0.x = 450;
	t_250_0.x = 550;
	putText(image,"2O",t_50_0,1,1,255);
	putText(image,"4O",t_100_0,1,1,255);
	putText(image,"6O",t_150_0,1,1,255);
	putText(image,"8O",t_200_0,1,1,255);
	putText(image,"10O",t_250_0,1,1,255);
	line(image,Point(150,550),Point(150,545),255,1,LINE_AA);
	line(image,Point(250,550),Point(250,545),255,1,LINE_AA);
	line(image,Point(350,550),Point(350,545),255,1,LINE_AA);
	line(image,Point(450,550),Point(450,545),255,1,LINE_AA);
	line(image,Point(550,550),Point(550,545),255,1,LINE_AA);

	Point t_0_50,t_0_100,t_0_150,t_0_200,t_0_250;
	t_0_50.y = 450;
	t_0_50.x = t_0_100.x = t_0_150.x = t_0_200.x = t_0_250.x = 20;
	t_0_100.y = 350;
	t_0_150.y = 250;
	t_0_200.y = 150;
	t_0_250.y = 50;
	putText(image,"20",t_0_50,1,1,255);
	putText(image,"4O",t_0_100,1,1,255);
	putText(image,"6O",t_0_150,1,1,255);
	putText(image,"8O",t_0_200,1,1,255);
	putText(image,"10O",t_0_250,1,1,255);
	line(image,Point(50,450),Point(60,450),255,1,LINE_AA);
	line(image,Point(50,350),Point(60,350),255,1,LINE_AA);
	line(image,Point(50,250),Point(60,250),255,1,LINE_AA);
	line(image,Point(50,150),Point(60,150),255,1,LINE_AA);
	line(image,Point(50,50),Point(60,50),255,1,LINE_AA);
}

void DrawPoint(Mat& image,int x,int y){
	Point center;
	center.x = x * 5 + 50;
	center.y = 550 - y * 5;
	circle(image,center,1,255,2,LINE_AA);
}

void DrawX(Mat& image,int _x,int _y){
    int x = _x * 5 + 50;
    int y = 550 - _y * 5;
    image.at<uchar>(y-1,x-1) = 255;
    image.at<uchar>(y-1,x+1) = 255;
    image.at<uchar>(y+1,x-1) = 255;
    image.at<uchar>(y+1,x+1) = 255;

    image.at<uchar>(y-2,x-2) = 255;
    image.at<uchar>(y-2,x+2) = 255;
    image.at<uchar>(y+2,x-2) = 255;
    image.at<uchar>(y+2,x+2) = 255;

    image.at<uchar>(y-2,x-2) = 255;
    image.at<uchar>(y-2,x+2) = 255;
    image.at<uchar>(y+2,x-2) = 255;
    image.at<uchar>(y+2,x+2) = 255;

    image.at<uchar>(y,x) = 255;
}

float h_x(float* theta,int* x1,int* x2,int i){
    float theta_x = theta[0] + theta[1] * x1[i] + theta[2] * x2[i];
    float s = 1 / (1 + exp(-theta_x));
    return s;
}

float J_theta(float* theta,int* x1,int* x2,int* y,int num){
    float J = 0.0;
    for(int i = 0;i < num ;i ++){
        J += y[i] * log(h_x(theta,x1,x2,i)) + (1-y[i]) * log(1 - h_x(theta,x1,x2,i));
    }
    return J;
}

void DrawLine(Mat& image,float* theta){
    Point A,B;
    A.x = 550;
    A.y = 550 + ((100*theta[1] + theta[0]) / theta[2]) * 5;
    B.x = 50 - ((100*theta[2] + theta[0]) / theta[1]) * 5;
    B.y = 50;
    line(image,A,B,255,1,LINE_AA);
    imshow("Coordinate",image);
}

int main(int argc,char ** argv){
    fstream file;
    file.open("data.txt");
    if(!file){
        cout << "无法打开指定文件" << endl;
        return -1;
    }

    string data[51];
    int x1[50],x2[50],y[50];
    int temp1,temp2,temp3;
    int idex = 0;
    int _idex = 0;
    int num = 0;
    int i = 0;
    bool ol = false;
    while(getline(file,data[i])){
        //cout<<"data["<<i<<"]:"<<data[i] << endl;

        idex = data[i].find_first_of(",",0);
        if(idex == -1)
            return -1;
        temp1 = atoi(data[i].substr(0,idex).c_str());

        _idex = data[i].find_first_of(",",idex + 1);
        if(_idex == -1)
            return -1;
        temp2 = atoi(data[i].substr(idex + 1,_idex - idex - 1).c_str());

        temp3 = atoi(data[i].substr(_idex + 1).c_str());

        //cout<<"temp1="<<temp1<<"\t"<<"temp2="<<temp2<<"\t"<<"temp="<<temp3<<endl;

        for(int j = 0; j < num;j ++){
            if(temp1 == x1[j] && temp2 == x2[j])
                ol = true;
        }
        if(!ol){
            x1[num] = temp1;
            x2[num] = temp2;
            y[num] = temp3;
            //cout<<"x1["<<num<<"]="<<x1[num]<<"\t"<<"x2["<<num<<"]="<<x2[num]<<"\t"<<"y["<<num<<"]="<<y[num]<<endl;
            num ++;
        }
        i ++;
        ol = false;
    }
    file.close();

    namedWindow("Coordinate");
    Mat coord = Mat::zeros(600,600,CV_8UC1);
    plot(coord);

    for(int j = 0;j < num;j ++){
        if(y[j] == 0){
            DrawPoint(coord,x1[j],x2[j]);
            //cout<<x1[j]<<"\t"<<x2[j]<<"\t"<<y[j]<<endl;
        }
        else{
            DrawX(coord,x1[j],x2[j]);
            //cout<<x1[j]<<"\t"<<x2[j]<<"\t"<<y[j]<<endl;
        }
    }

    float tempJ = 0.0;
    srand((unsigned)time(0));
    float theta[3];
    for(int j = 0;j < 3;j ++){
        int yy = rand() % 100;
        theta[j] = (float)yy / 100;
    }
    float temp[3] = {0.0,0.0,0.0};
    float alpha = atof(argv[1]);
    int xx = atoi(argv[2]);
    i = 0;
    for(int j = 0;j < xx;j ++){
        //DrawLine(coord,theta);
        tempJ = J_theta(theta,x1,x2,y,num);
        cout << "J=" << tempJ << endl;
        for(int k = 0;k < 50;k ++){
            if(i+k >= num)
                i -= num;
            temp[0] += alpha * (y[i+k] - h_x(theta,x1,x2,i+k));
            temp[1] += alpha * (y[i+k] - h_x(theta,x1,x2,i+k)) * x1[i+k];
            temp[2] += alpha * (y[i+k] - h_x(theta,x1,x2,i+k)) * x2[i+k];
        }
        for(int k = 0;k < 3;k ++){
            theta[k] += temp[k] / 50;
            temp[k] = 0.0;
            cout<<"theta["<<k<<"]:"<<theta[k]<<"\t";
        }
        cout << endl;
        i += 50;
        if(i >= num)
            i -= num;
        //DrawLine(coord,theta);
    }
    DrawLine(coord,theta);
    //imshow("Coordinate",coord);
    waitKey();
    return 0;
}
