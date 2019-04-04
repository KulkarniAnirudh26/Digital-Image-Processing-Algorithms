
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

const float pi=22.0/7.0;

double gaussian_filter_5x5(int i, int j, int k, int l) {
    int act_k = i + k -2;
    int act_l = j + l-2;
    float sigma = 1; 
    double weight_value = 0;
    //double denom = 1.0/(sqrt(2*pi))*sigma ;
    double A = -(pow((act_k - i),2) + pow((act_l - j),2));
    double B = 2*sigma*sigma;
    double numerator  = exp(A/B);
    //weight_value  = numerator/denom;
    return A/B;
}

double gaussian_filter_3x3(int i, int j, int k, int l) {
    int act_k = i + k -1;
    int act_l = j + l-1;
    float sigma = 1; 
    double weight_value = 0;
    //double denom = 1.0/(sqrt(2*pi))*sigma ;
    double A = -(pow((act_k - i),2) + pow((act_l - j),2));
    double B = 2*sigma*sigma;
    double numerator  = exp(A/B);
    //weight_value  = numerator/denom;
    return A/B;
}


int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int Size1 = 256;
    int Size2 = 256;

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
    unsigned char Imagedata[Size1][Size2][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size1*Size2*BytesPerPixel, file);
    fclose(file);

    double sum =0;
    double weight = 0;
    double weight1;
    double weight2;
    double sigmac = 2000;
    double weight_sum = 0;
    unsigned char NewImage[Size1][Size2][1];
    for(int i =2 ; i<Size1-2; i++){
        for(int j =2; j<Size2-2; j++) {
            for (int k =0; k<5;k++){
                for(int m=0;m<5;m++){
                    weight1 = gaussian_filter_5x5(i,j,k,m);
                    weight2 = (double)((Imagedata[i][j][0] - Imagedata[i+k-2][j+m-2][0])*(Imagedata[i][j][0] - Imagedata[i+k-2][j+m-2][0]))/(2*sigmac*sigmac); 
                    weight = exp(-weight1-weight2);
                    sum = sum + Imagedata[i+k-2][j+m-2][0]*weight;
                    weight_sum = weight_sum + weight;
                    //if(i == 1){
                      // cout << sum << "\t";
                    //}
                }
            }
            NewImage[i][j][0] = sum/weight_sum;
            sum = 0;
            weight_sum = 0;
            //weight1 =0;
            //weight2 =0;
            //weight = 0;
        }

    }
/*
double a1;
a1 = ((Imagedata[1][1][0] - Imagedata[0][0][0])*(Imagedata[1][1][0] - Imagedata[0][0][0]))/(2*2*2*2); 
cout << (int)Imagedata[1][1][0] << "\t"<< (int)Imagedata[0][0][0] << a1 <<endl;
*/
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(NewImage, sizeof(unsigned char), Size1*Size2*1, file);
    fclose(file);
    
        return 0;
}

