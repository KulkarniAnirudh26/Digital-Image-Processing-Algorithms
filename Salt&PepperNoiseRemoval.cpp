#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include<math.h>

using namespace std;

const float pi=22.0/7.0;

unsigned char* Function_sort(unsigned char array[9],int n){
	for (int a = 0; a < n; a++){
		for (int b = a+1; b <n ;b++){
			if (array[a] > array[b]){
				unsigned char temp = array[a];
				array[a] = array[b];
				array[b] = temp; 
			}
		}
	}
	return array;
}

double gaussian_filter_3x3(int i, int j, int k, int l) {
    int act_k = i + k -1;
    int act_l = j + l-1;
    float sigma = 1; 
    double weight_value = 0;
    double denom = 1.0/(sqrt(2*pi))*sigma ;
    double A = -(pow((act_k - i),2) + pow((act_l - j),2));
    double B = 2*sigma*sigma;
    double numerator  = exp(A/B);
    weight_value  = numerator/denom;
    return weight_value;
}

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size1 = 390;
	int Size2 = 300;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	}
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size1 = atoi(argv[4]);
			Size2 = atoi(argv[5]);
		}
	}

	// Allocate image data array
	unsigned char Imagedata0[Size2][Size1][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata0, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
	fclose(file);

	unsigned char Imagedata[Size1+2][Size2+2][BytesPerPixel];
    for(int i = 0; i<Size1; i++){
    	for (int j =0; j<Size2; j++) {
    		Imagedata[i+1][j+1][0] = Imagedata0[i][j][0];
    		Imagedata[i+1][j+1][1] = Imagedata0[i][j][1];
    		Imagedata[i+1][j+1][2] = Imagedata0[i][j][2];
    	}
    }
    for(int k =0; k< Size2+2 ;k++){
    	Imagedata[0][k][0] = Imagedata0[0][k][0];
    	Imagedata[0][k][1] = Imagedata0[0][k][1];
    	Imagedata[0][k][2] = Imagedata0[0][k][2];
    	Imagedata[Size1+1][k][0] = Imagedata0[Size1][k][0];
    	Imagedata[Size1+1][k][1] = Imagedata0[Size1][k][1];
    	Imagedata[Size1+1][k][2] = Imagedata0[Size1][k][2];
    }

    for(int m =0; m< Size1+2 ;m++){
    	Imagedata[m][0][0] = Imagedata0[m][0][0];
    	Imagedata[m][0][1] = Imagedata0[m][0][1];
    	Imagedata[m][0][2] = Imagedata0[m][0][2];
    	Imagedata[m][Size2+1][0] = Imagedata0[m][Size2][0];
    	Imagedata[m][Size2+1][1] = Imagedata0[m][Size2][1];
    	Imagedata[m][Size2+1][2] = Imagedata0[m][Size2][2];
    }
    
	unsigned char RedImagedata[Size2+2][Size1+2][1];
	unsigned char BlueImagedata[Size2+2][Size1+2][1];
	unsigned char GreenImagedata[Size2+2][Size1+2][1];


	for(int c =0; c < 3; c++) {
		for(int a=0; a < 258; a++ ) {
			for(int b =0; b < 258; b++) {
			if (c == 0) {
					BlueImagedata[a][b][0] = Imagedata[a][b][0];
			}
			if (c == 1) {
					GreenImagedata[a][b][0] = Imagedata[a][b][1];
						}
			if (c == 2) {
					RedImagedata[a][b][0] = Imagedata[a][b][2];

						}
		    }
	   }
	}

/*
	for(int k =0;k<256;k++){
		for(int m =0;m <256;m++){
			cout << (int)BlueImagedata[k][m][0] << "\t" << (int)GreenImagedata[k][m][0] << "\t" << (int)RedImagedata[k][m][0] << endl;
		}
	}
*/
	unsigned char array1[9];
	unsigned char array2[9];
	unsigned char array3[9];
	int index = 0;
	unsigned char RedImageImpulse[Size1+2][Size2+2][1];
	unsigned char BlueImageImpulse[Size1+2][Size2+2][1];
	unsigned char GreenImageImpulse[Size1+2][Size2+2][1];
	//Impulse noise removal
	for (int i= 0;i <258; i++){
		for (int j =0; j<258; j++) {
			for (int k=0; k<3; k++) {
				for(int m=0;m<3;m++){
					array1[index] = RedImagedata[i+k-1][j+m-1][0];
					array2[index] = GreenImagedata[i+k-1][j+m-1][0];
					array3[index] = BlueImagedata[i+k-1][j+m-1][0];
					index = index + 1;
				}
			}
			index = 0;
			unsigned char* p1 = Function_sort(array1,9);
			RedImageImpulse[i][j][0] = *(p1+4);//
			//Median value
			unsigned char* p2 = Function_sort(array2,9);
			GreenImageImpulse[i][j][0] = *(p2+4);

			unsigned char* p3 = Function_sort(array3,9);		
			BlueImageImpulse[i][j][0] = *(p3+4);//Median value
			
		}
	}

   //Convolution using 3x3 gaussian filter
     
     double sum_of_weights = 0;
     double kernel[3][3];
     for (int i =0; i <3;i++){
        for(int j=0;j<3;j++){
            double weight_value = gaussian_filter_3x3(1,1,i,j);
            sum_of_weights = sum_of_weights + weight_value;
            kernel[i][j] = weight_value;
           // cout << sum_of_weights << endl;;
        }
     }
     //double a = kernel[0][0]/sum_of_weights;
     //cout << "\t" << a << endl;
     for (int i =0; i <3;i++){
        for(int j=0;j<3;j++){
            kernel[i][j] = kernel[i][j]/sum_of_weights;
        }
    }

    unsigned char sum1 = 0;
    unsigned char sum2 = 0;
    unsigned char sum3 = 0;
    unsigned char RedImageG[Size1+2][Size2+2][1];
	unsigned char BlueImageG[Size1+2][Size2+2][1];
	unsigned char GreenImageG[Size1+2][Size2+2][1];

     for (int i =0; i < 258; i++){
        for (int j = 0; j <258; j++){
            for(int k =0; k <3;k++){
                for(int l =0; l<3;l++){
                    sum1 = sum1 + RedImageImpulse[i+k-1][j+l-1][0]*kernel[k][l];
                    sum2 = sum2 + GreenImageImpulse[i+k-1][j+l-1][0]*kernel[k][l];
                    sum3 = sum3 + BlueImageImpulse[i+k-1][j+l-1][0]*kernel[k][l];
                }
            }
            RedImageG[i][j][0] = sum1;
            GreenImageG[i][j][0] = sum2;
            BlueImageG[i][j][0] = sum3;
            sum1 =0;
            sum2 =0;
            sum3 = 0;
        }
     }

   //Computing Final Image
	unsigned char FinalImage[Size2+2][Size1+2][3]; 
    //cout << " \n At the end now" << endl;
	for(int c =0; c < 3; c++) {
		for(int a=0; a < 258; a++ ) {
			for(int b =0; b < 258; b++) {
			if (c == 0) {
					FinalImage[a][b][c] = BlueImageG[a][b][0];
			}
			if (c == 1) {
					FinalImage[a][b][c] = GreenImageG[a][b][0];
						}
			if (c == 2) {
					FinalImage[a][b][c] = RedImageG[a][b][0];

						}
		}
	   }
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(FinalImage, sizeof(unsigned char), (Size2+2)*(Size1+2)*3, file);
	fclose(file);
    
		return 0;
}

