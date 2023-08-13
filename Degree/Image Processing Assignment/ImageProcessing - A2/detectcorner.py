import cv2 as cv
import numpy as np 

imagesCorner = []

'''
    This function take in images file name to load,
    then detect each image of sub-images corner
    saved found out of corners in images Corners then return them. 
'''
def detectCorner(images):
    for item in images:
        img = cv.imread(item)
        grey = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

        #DetectConer then mark up with circle
        corners = cv.goodFeaturesToTrack(grey,200,0.5,2)
        corners = np.int0(corners)
        imagesCorner.append(corners)
        for corner in corners:
            x,y = corner.ravel()
            cv.circle(img, (x,y), 5, (0,255,255),-1)
        cv.imshow(item, img)

    return imagesCorner