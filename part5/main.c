/*Authors: Alexander Murray, Parth Italia, and Dr. Pallipuram */
/*Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "main.h"
#include "amplify.h"

int main (int argc, char **argv)
{
	/*Declare Variables*/
	float *org_img;
	float *Gx_mask, *Gy_mask;
	float *IGx, *IGy;	
	float *Gxy, *I_angle, *High_res;
	float *suppressed, *threshold, *hyst;
	float *Vmap, *Hmap;
	float *HRV, *HRH;
	int i, j, m, n;
	int img_width, img_height;
	int alpha;
	int sigma;
	int gauss_width;
	
	struct timeval start, stop, rStart, rStop, wStop, wStart, calcStart, calcStop;
	float time, calcTime, wTime, rTime;

	char nameIGy[20];
	char nameIGx[20];
	char gxName[20];
	char gyName[20];
	char fileGXY[20];
	char fileIAng[20];
	char filetHold[20];
	char fileSupp[25];
	char fileHyst[25];
	char fileHigh[25];
	
	if(argc != 5)
	{
		printf("\n Correct Usage: ./myexec <image name> <gaussian width> <sigma> <alpha>");
		return 0;
	}
	
	//Use timer to determine read time. 
	gettimeofday(&rStart, NULL);
    read_image_template(argv[1], &org_img, &img_width, &img_height); 
	gettimeofday(&rStop, NULL);
	
	//Read arguments from the command line 
	gauss_width = atoi(argv[2]);
	sigma = atoi(argv[3]);
	alpha = atoi(argv[4]);
	
	//Start timer 
	gettimeofday(&start, NULL);
	
	//Allocate memory for masks
	Gx_mask = (float *)malloc(sizeof(float) * (gauss_width * gauss_width));
	Gy_mask = (float *)malloc(sizeof(float) * (gauss_width * gauss_width));

	//Allocate memory for intermediate images
	IGy = (float *)malloc(sizeof(float) * (img_width * img_height));
	IGx = (float *)malloc(sizeof(float) * (img_width * img_height));
	
	Gxy = (float *)malloc(sizeof(float) * (img_width * img_height));
	I_angle = (float *)malloc(sizeof(float) * (img_width * img_height));
	
	suppressed = (float *)malloc(sizeof(float) * (img_width * img_height));
	threshold = (float *)malloc(sizeof(float) * (img_width * img_height));
	High_res = (float *)malloc(sizeof(float) * ((img_width * alpha) * (img_height * alpha)));
	
	HRV = (float *)malloc(sizeof(float) * ((img_width * alpha) * (img_height * alpha)));
	HRH = (float *)malloc(sizeof(float) * ((img_width * alpha) * (img_height * alpha)));
	
	hyst = (float *)malloc(sizeof(float) * (img_width * img_height));
	
	Vmap = (float *)malloc(sizeof(float) * (img_width * img_height));
	Hmap = (float *)malloc(sizeof(float) * (img_width * img_height));

/*1. Canny Edge Detector */
	//Start calculation time timer

	printf("\n 1. Executing the Canny Edge Detector...");
	gettimeofday(&calcStart, NULL);
	//Give Gx_mask, Gy_mask values
	gaussianX_mask(Gx_mask, gauss_width, sigma);
	gaussianY_mask(Gy_mask, gauss_width, sigma);


	printf("\n 1a. Producing magnitude and angle maps...");
	//Get IGy: convolve(org_img,Gx_mask)
	convolve<float>(org_img, img_width, img_height, Gx_mask, gauss_width, IGy);
	//Get IGx: convolve(org_img, Gy_mask)
	convolve<float>(org_img, img_width, img_height, Gy_mask, gauss_width, IGx);
	//Get magnitude based on ONLY IGy and IGx
	magnitude(IGx, IGy, img_width, img_height, Gxy); 
	//Get directionality based on ONLY IGy and IGx
	angle(IGy, IGx, img_width, img_height, I_angle); 
	gettimeofday(&calcStop, NULL);
	memcpy(suppressed, Gxy, sizeof(float) * img_width * img_height);
	//Suppress auxillary image


	printf("\n 1b. Running non-maximal suppression and edge-linking...");
	nonmaximal_suppression<float>(suppressed, I_angle, 0, img_width, img_height);
	//Use thresholding to remove some noise from auxillary image
	double_thresh(suppressed, threshold, img_width, img_height); //VKP: Need some changes here
	edge_linking(threshold, hyst, img_width, img_height);


	printf("\n 1c. Edge map ready...");

	printf("\n 2. Replicating vertical edge rods...");
	//Edge Keeping in vertical and horizontal directions. 
//	vertical_edgekeeping(Vmap, hyst, I_angle, HRV, alpha, org_img, img_width, img_height);
//	horizontal_edgekeeping(Hmap, hyst, I_angle, HRH, alpha, org_img, img_width, img_height);

	vertical_edge_keeping(hyst,I_angle,org_img,img_width,img_height,alpha,HRV,Vmap);

	printf("\n 3. Replicating horizontal edge rods...");
	horizontal_edge_keeping(hyst,I_angle,org_img,img_width,img_height,alpha,HRH,Hmap);	
	
	memcpy(High_res, HRV, sizeof(float)*alpha*img_width*img_height*alpha);
	
	for(i=0;i<img_height; i++)
	{
		for(j=0;j<img_width; j++)
		{
			for(m=0;m<alpha; m++)
			{
				for(n=0;n<alpha;n++)
				{
					if((i*alpha+m)<alpha*img_height && (j*alpha+n) <alpha*img_width)
					{
						High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] += HRH[(i*alpha+m)*img_width*alpha+(j*alpha+n)];
						if((Hmap[i*img_width+j] == Vmap[i*img_width+j]) && Hmap[i*img_width+j] == 255)
						{
							//High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] /= 2;
							Hmap[i*img_width+j] = 125;
							Vmap[i*img_width+j] = 125;
						}
						else if((Hmap[i*img_width+j] == 255 && Vmap[i*img_width+j] != 255) 
							|| (Hmap[i*img_width+j] != 255 && Vmap[i*img_width+j] == 255))
						{
							High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] = High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] + HRH[(i*alpha+m)*img_width*alpha+(j*alpha+n)];
						}
					}
				}
			}
		}
	}

	//Mean Keepinig

	printf("\n 4. Interpolating non-edge pixels...");
	mean_keeping(org_img,hyst,img_width,img_height,alpha,Vmap,Hmap,High_res);

	/********************* Lenna no artifacts up to this point********************/
	/* Find highest pixel value in the High Resolution Image */
	float high_val = 0;
	for(i=0;i<img_height; i++)
	{
		for(j=0;j<img_width; j++)
		{
			for(m=0;m<alpha; m++)
			{
				for(n=0;n<alpha;n++)
				{
					if((i*alpha+m)<alpha*img_height && (j*alpha+n) <alpha*img_width)
					{
						if(High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] > high_val)
						{
							high_val = High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)];
						}
					}
				}
			}
		}
	} 
	/* Normalize the image */
	for(i=0;i<img_height; i++)
	{
		for(j=0;j<img_width; j++)
		{
			for(m=0;m<alpha; m++)
			{
				for(n=0;n<alpha;n++)
				{
					if((i*alpha+m)<alpha*img_height && (j*alpha+n) <alpha*img_width)
					{
						High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)] =  High_res[(i*alpha+m)*img_width*alpha+(j*alpha+n)]*(255/high_val);
					}
				}
			}
		}
	}

	//Mean keeping
	//mean(org_img, High_res, hyst, img_width, img_height, alpha, Vmap, Hmap);

	//Put image name into string
	sprintf(nameIGy,"HRV_%d.pgm", img_width);
	sprintf(nameIGx,"HRH_%d.pgm", img_width);
	sprintf(fileGXY, "Gxy_img_%d.pgm", img_width);
	sprintf(fileIAng, "I_Angle_%d.pgm", img_width);
	sprintf(fileSupp, "Suppressed_%d.pgm", img_width);
	sprintf(filetHold, "Threshold_%d.pgm", img_width);
	sprintf(fileHyst, "Hystersis_%d.pgm", img_width);
	sprintf(fileHigh, "output%d.pgm", img_width*alpha);
	
	//Calculate write time 
	gettimeofday(&wStart, NULL);
	//Write out the images
	write_image_template<float>(nameIGy, HRV, alpha * img_width, alpha * img_height);
	write_image_template<float>(nameIGx, HRH, alpha *img_width, alpha * img_height); 
	write_image_template<float>(fileGXY, Gxy, img_width, img_height);
//	write_image_template<float>(fileIAng, I_angle, img_width, img_height);
	write_image_template<float>(fileSupp, suppressed, img_width, img_height);
	write_image_template<float>(filetHold, threshold, img_width, img_height);
	write_image_template<float>(fileHyst, hyst, img_width, img_height);
	write_image_template<float>(fileHigh, High_res, img_width * alpha, img_height * alpha);

	printf("\n 5. The output is written in: %s \n",fileHigh);

	gettimeofday(&wStop, NULL);
	
	gettimeofday(&stop, NULL);
	//Calculate elapsed times (in us)
	time = (float)((stop.tv_sec * 1000000 + stop.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	rTime = (float)((rStop.tv_sec * 1000000 + rStop.tv_usec) - (rStart.tv_sec * 1000000 + rStart.tv_usec));
	wTime = (float)((wStop.tv_sec * 1000000 + wStop.tv_usec) - (wStart.tv_sec * 1000000 + wStart.tv_usec));
	calcTime = (float)((calcStop.tv_sec * 1000000 + calcStop.tv_usec) - (calcStart.tv_sec * 1000000 + calcStart.tv_usec));
	free(IGx);
	free(IGy);
	free(Gxy);
	free(I_angle);
	free(suppressed);
	free(hyst);
	free(High_res);
	free(threshold);
	
	return 0;

}
