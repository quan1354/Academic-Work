import cv2 as cv
from math import log10, sqrt
import numpy as np
import os
img = cv.imread('109055.png', cv.IMREAD_UNCHANGED)

'''
  Image Pre-Processing
'''
# 2i) Convert to Gray
gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
cv.imshow('wolf Gray', gray)
# 2ii) Resize the image while maintaining the aspect ratio of the image
def rescaleFrame(frame, scale_percent):
  width = int(frame.shape[1] * scale_percent / 100)
  height = int(frame.shape[0] * scale_percent / 100)
  dimensions = (width, height)
  return cv.resize(frame, dimensions, interpolation=cv.INTER_AREA)

resizeImg =  rescaleFrame(gray,220)
cv.imshow('wolf Resize',resizeImg)
print(gray.shape)
print(resizeImg.shape)

# 2iii) Save the processed image object to tagged image file format (TIFF)
saveImg = cv.imwrite('myWolf.tiff',resizeImg)

'''
  Domain Filtering
'''
def PSNRFunc(original, compressed):
    mse = np.mean((original - compressed) ** 2)
    print("======================================")
    print("MSE Value:", mse)
    if(mse == 0):  # MSE is zero means no noise is present in the signal .
                   # Therefore PSNR have no importance.
        return 'No noise present in signal MSE is 0' 
    max_pixel = 255.0
    psnr = 20 * log10(max_pixel / sqrt(mse))
    return psnr

def compressionRatioGrey(img, filename):
    print("Uncompressed size:", img.size, "bytes")
    info = os.stat(filename)
    print("Compressed size: ", info.st_size, "bytes")
    cr = float(info.st_size)/img.size
    print("Compression ratio:", cr)
    crPercent = cr*100 # Convert ratio to percentage.
    print("Compression ratio in percentage:", crPercent, "%")
    print("======================================")

imgMyWolf = cv.imread('myWolf.tiff', cv.IMREAD_GRAYSCALE)
# Mean filter
mean = cv.blur(imgMyWolf, (7,7),0)
cv.imwrite('meanWolf.tiff',mean)
cv.imshow('wolf mean',mean)
# Median filter
median = cv.medianBlur(imgMyWolf, 7,0)
cv.imwrite('medianWolf.tiff',median)
cv.imshow('wolf median',median)
# Gaussian filter
gaussian = cv.GaussianBlur(imgMyWolf, (7,7), 0)
cv.imwrite('gaussianWolf.tiff',gaussian)
cv.imshow('wolf gaussian',gaussian)
#Bilateral filter
bilateral = cv.bilateralFilter(imgMyWolf, 5, 15, 15)
cv.imwrite('bilateralWolf.tiff',bilateral)
cv.imshow('wolf bilateral',bilateral)

# display PSNR result
print('Original Image PSNR: ' + str(PSNRFunc(imgMyWolf,imgMyWolf)))
print('mean PSNR: ' + str(PSNRFunc(imgMyWolf, mean)))
print('median PSNR: ' + str(PSNRFunc(imgMyWolf, median)))
print('gaussian PSNR: ' + str(PSNRFunc(imgMyWolf, gaussian)))
print('bilateral PSNR: ' + str(PSNRFunc(imgMyWolf, bilateral)))

# display CR result
print('Original Image CR: ')
str(compressionRatioGrey(imgMyWolf, 'myWolf.tiff'))
print('mean CR: ')
str(compressionRatioGrey(imgMyWolf, 'meanWolf.tiff'))
print('median CR: ')
str(compressionRatioGrey(imgMyWolf, 'medianWolf.tiff'))
print('gaussian CR: ')
str(compressionRatioGrey(imgMyWolf, 'gaussianWolf.tiff'))
print('bilateral CR: ')
str(compressionRatioGrey(imgMyWolf, 'bilateralWolf.tiff'))

cv.waitKey(0)
cv.destroyAllWindows()