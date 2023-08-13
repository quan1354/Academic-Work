from detectcorner import detectCorner
from measure import measure
from detectobject import detectObject
import cv2 as cv

'''
    To fullfill submisstion requirement, 
    algorithm are separate to do in three file 
    as expected and use a main.py file to run them.  
'''
## answer for Question 1a, of image file name
images = ('ImageA.jpeg','ImageWithObject1.jpeg','ImageNoObject1.jpeg','ImageWithObject2.jpeg','ImageNoObject2.jpeg')


if __name__ == '__main__':
    ## 1b) corners detection
    corners = detectCorner(images)

    ## 1c) distance computation
    distance = measure(corners)
    ## show computation result
    print('-------------|1c Average Distance Calculation Result|------------------')
    for index,image in enumerate(images[1:]):
        print("image 1 corners to " + image + " corners -> " + str(distance[index]) ,end="\n")

    ## 1d) Object detection
    #detectObject(images)

    ## hold key to close all windows
    cv.waitKey(0)
    cv.destroyAllWindows()


