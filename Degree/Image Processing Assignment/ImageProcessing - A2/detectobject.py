import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt

def detectObject(images):
    template = cv.imread(images[images.index('ImageA.jpeg')], 0)
    for index,image in enumerate(images[1:]):
        imageToProccess = cv.imread(image)
        grey = cv.cvtColor(imageToProccess, cv.COLOR_BGR2GRAY)
 
        h,w = template.shape[::]
        res = cv.matchTemplate(grey, template, cv.TM_CCOEFF_NORMED)
        threshold = 0.8
        loc = np.where(res>=threshold) 
        for point in zip(*loc[::-1]):
            cv.rectangle(imageToProccess, point, (point[0]+w, point[1]+h), (0,0,255), 1)
        plt.subplot(1,4,index+1),plt.imshow(cv.cvtColor(imageToProccess, cv.COLOR_BGR2RGB))
        plt.title('DetectObj_'+image),plt.xticks([]),plt.yticks([])
    plt.show()
    return
