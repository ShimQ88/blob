#include "opencv4/opencv.hpp"
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <vector>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
using namespace chrono;


/*********************************************************************************************
 * compile with:
 * g++ -std=c++11 assignment1.cpp -o assignment1 `pkg-config --cflags --libs opencv`
*********************************************************************************************/

#define Mpixel(image,x,y) ((uchar *)((image).data +(y)*((image).step)))[(x)*((image).channels())]
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]
#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]
int counterColour=0;

void colour_count(const std::vector< tuple<int, int> > &n)
{
	if(n.empty()==true){

	}else{
		counterColour++;
	}

}
void blob(Mat image2, Mat image3){
	vector< tuple<int, int> > SET[image2.cols];
	int A[image2.cols][image2.rows];
	int s1,s2;

	for(int x=0; x<image2.cols;x++){
		for(int y=0; y<image2.rows;y++){
			A[x][y]=-1;
		}
	}
	int i=0;
	int index=0, counter=-1;
	bool is_s1_in=false;
	for(int y=1; y<image2.rows;y++){
		for(int x=1; x<image2.cols;x++){
			if(Mpixel(image2,x,y)!=0){
				if((Mpixel(image2,x-1,y)!=0)||(Mpixel(image2,x,y-1)!=0)){
					s1=A[x-1][y];
					s2=A[x][y-1];
					if(s1!=-1){
						SET[s1].push_back(make_tuple(x,y));
						
						A[x][y]=s1;
						is_s1_in=true;
					}
					if(s2!=-1){
						SET[s2].push_back(make_tuple(x,y));
						A[x][y]=s2;
						is_s1_in=false;

					}
					if((s1!=s2)&&(s1!=-1)&&(s2!=-1)){
						SET[s1].insert(SET[s1].end(),SET[s2].begin(),SET[s2].end());						
						for (int j = 0; j < SET[s2].size(); j++ ){
							A[get<0>(SET[s2][j])][get<1>(SET[s2][j])]=s1;
						}				
						SET[s2].clear();
					}
					is_s1_in=false;

				}else{
					counter++;
					SET[counter].push_back(make_tuple(x,y));
					A[x][y]=counter;
				}

			}
		}
	}
	//colour counting
	// for(int i=0;i<image2.cols;i++){
	// 	colour_count(SET[i]);
	// }

	//put random colour to image3
	int rand_numb1 = rand() % 100; 
	int rand_numb2 = rand() % 100; 
	int rand_numb3 = rand() % 100; 
	for(int y=0; y<image3.rows;y++){
		for(int x=0; x<image3.cols;x++){
			if(y==0){
				colour_count(SET[x]);
			}
			if(A[x][y]==-1){
				pixelB(image3,x,y)=0;
				pixelG(image3,x,y)=0;
				pixelR(image3,x,y)=0;	
			}
			if(A[x][y]!=-1){
				
				pixelB(image3,x,y)=(A[x][y]+1)*rand_numb1;
				pixelG(image3,x,y)=(A[x][y]+1)*rand_numb2;
				pixelR(image3,x,y)=(A[x][y]+1)*rand_numb3;
			}
		}
	}
}

void median_filter(Mat image1,Mat image2,int window_size){
	//picture elements(pels)
	int function_size_input=window_size;//this is for window size

	int picture_x_size=image1.cols;
	int picture_y_size=image1.rows;
	int mdn;//median value in a window
	int ltmdn=0;// number of pels having gray levels less than mdn in a window
	int window_x_size=function_size_input;
	int window_y_size=function_size_input;
	//int hist[window_x_size*window_y_size];
	int index_for_array=0;
	int count_of_less_than_median=0;
	int median_value;
	int left_column[window_y_size];
	int right_column[window_y_size];
	// int left_column[window_y_size-1];
	// int right_column[window_y_size-1];
	int hist_index=0;
	int g1;

	int th=(window_x_size*window_y_size/2);
	
	// for(int i=0;i<picture_y_size;i++){
	// 		Mpixel(image2,637,i)=255;
	// 	}
	
	for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
		
		int hist[256];
		for(int q=0;q<256;q++){
			hist[q]=0;
		}

		int index_for_hist=0;
		int y_sizeof=i+window_y_size;
		int x_sizeof=(window_x_size/2)+window_x_size;
		for(int a=i;a<y_sizeof;a++){
			for(int b=(window_x_size/2);b<x_sizeof;b++){
				index_for_hist=(int)Mpixel(image1,a,b);
				hist[index_for_hist]=hist[index_for_hist]+1;
			}
		}


		int counter_for_find_median=(window_x_size*window_y_size/2)+1;
		int counter_for_find_less_than_median=0;

		for(int z=0;z<256;z++){
			if(hist[z]!=0){	
				counter_for_find_median=counter_for_find_median-hist[z];
				if(counter_for_find_median<=0){
					median_value=z;
					mdn=median_value;
					break;
				}else{
					counter_for_find_less_than_median
					=counter_for_find_less_than_median+hist[z];
				}
			}		
		}

		ltmdn=counter_for_find_less_than_median;

		//Mpixel(image2,i,(window_y_size/2))=mdn;

		for(int j=(window_y_size/2)+1;j<picture_y_size-(window_y_size/2);j++){//j indicates picture column number

	
			int index_for_left_column=0;
			int index_for_right_column=0;

			for(int l=i;l<i+window_x_size;l++){
				left_column[index_for_left_column]=(int)Mpixel(image1,l,j);
				index_for_left_column++;

				right_column[index_for_right_column]=(int)Mpixel(image1,l,j+window_x_size-1);				
				index_for_right_column++;	
			}
			
					
			

			for(int k=0;k<window_y_size;k++){
				g1=left_column[k];
				hist[g1]=hist[g1]-1;
				if(g1<mdn){
					ltmdn=ltmdn-1;
				}
				g1=right_column[k];
				hist[g1]=hist[g1]+1;
				if(g1<mdn){
					ltmdn=ltmdn+1;
				}

				if(ltmdn>th){
					while(true){
						mdn=mdn-1;
						ltmdn=ltmdn-hist[mdn];
						if(ltmdn<=th){
							break;
						}
					}
				}else{
					while(ltmdn+hist[mdn]<=th){
						ltmdn=ltmdn+hist[mdn];
						mdn=mdn+1;
					}
				}
			}
			Mpixel(image2,i,j)=mdn; //determine pixel
		}
	}

	for(int j=0;j<window_size/2+1;j++){	
		for(int i=0;i<picture_y_size;i++){
			Mpixel(image2,j,i)=0;
			Mpixel(image2,picture_x_size-1-j,i)=0;
		}
	}
	for(int j=0;j<window_size/2+1;j++){	
		for(int i=window_size/2;i<picture_x_size-(window_size/2);i++){
			Mpixel(image2,i,j)=0;
			Mpixel(image2,i,picture_y_size-1-j)=0;
		}
	}

}
int main(int argc, char** argv)
{
	Mat image;
	double fps=0.0;
	if(argc==2){//static image mode
    	cout<<"image mode"<<endl;
    	system_clock::time_point start = system_clock::now();
    	namedWindow("original_image", 0);
		namedWindow("thrsholding_image", 0);
		namedWindow("blob_image", 0);
		image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
		Mat image2(image.rows,image.cols,CV_8UC3);
		Mat image3(image.rows,image.cols,CV_8UC3);

		//Thresholding processing
		if(!image.data){printf("Error\n"); exit(0); }
		for(int x=0; x<image.cols;x++){
			for(int y=0; y<image.rows;y++){
				if(Mpixel(image,x,y)>=39){//50 is good //40 is bolt
					Mpixel(image2,x,y)=255;
				}else{
					Mpixel(image2,x,y)=0;
				}
			}
		}
		//end of thrsholding image

		median_filter(image2,image2,11); //increased window size for accuracy of image
		
		blob(image2,image3);

		putText(image3, to_string(counterColour), Point2f(50,50), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2,8,false);
		
		imshow("original_image",image);
		imshow("thrsholding_image",image2);
		imshow("blob_image" ,image3);

		counterColour=0;
		system_clock::time_point end = system_clock::now();
		double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		fps = 1000000/seconds;
	    cout << "frames " << fps << " seconds " << seconds << endl;
		waitKey(0);
    }else{//dynamic camera mode
    	VideoCapture cap;
	  	cap.open(0);
	 	if (!cap.isOpened()){
	        cout << "Failed to open camera" << endl;
	        return 0;
	    }
	    
	   	cout << "Opened camera" << endl;
	   	namedWindow("WebCam", 1);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	   	cap >> image;
	   	int key=0;
	   	
	   	while (1){
	    	system_clock::time_point start = system_clock::now();
	     	
	       	cap >> image;
	       	if( image.empty() )
            break;
	       	imshow("WebCam", image);
	       	namedWindow("blob image", 0);
	       	Mat image2(image.rows,image.cols,CV_8UC3);
			Mat image3(image.rows,image.cols,CV_8UC3);
			//set binary image 
			if(!image.data){printf("Error\n"); exit(0); }
			for(int x=0; x<image.cols;x++){
				for(int y=0; y<image.rows;y++){
                    if(Mpixel(image,x,y)>=39){
						Mpixel(image2,x,y)=255;
					}else{
						Mpixel(image2,x,y)=0;
					}
				}
			}
            median_filter(image2,image2,7);
			blob(image2,image3);
		
			putText(image3, to_string(counterColour), Point2f(50,50), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2,8,false);

			imshow("blob image" ,image3);
		
	       	key=waitKey(1);
	       	if(key==113 || key==27) return 0;//either esc or 'q'
	     	system_clock::time_point end = system_clock::now();
	     	double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	     	//fps = 1000000*10.0/seconds;
	     	fps = 1000000/seconds;
	     	cout << "frames " << fps << " seconds " << seconds << endl;
	     	counterColour=0;
	   	}
    }


}
