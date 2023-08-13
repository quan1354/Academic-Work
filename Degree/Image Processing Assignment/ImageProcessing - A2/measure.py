import math

distance = []

"""
    This function get every image of corners in list,
    then take x and y out from image A of corners with ravel(),
    then similar take other image of x and y, together put in compute average distance.
    the procedure is repear until all image has been compute with image A,
    all result are save in distance list to be show
"""
def measure(cornersList):
    # get image A of corner
    imageACorner = cornersList[0]
    # get x1 and y1 from Image A of corners 
    for corner in imageACorner:
        x1,y1 = corner.ravel()
    # calculate distance for ImageA of coners to others 4 images of coners
    for i in range(1,len(cornersList)):
        distance.append(computeDistance(x1,y1,len(imageACorner), cornersList[i]))
    return distance

def computeDistance(x1,y1,length1,OtherImageConers):
    totalCorners = length1 + len(OtherImageConers)
    for corner in OtherImageConers:
        x2,y2 = corner.ravel()
    
    distance = math.sqrt(math.pow((x1-x2),2)+ math.sqrt(pow((y1-y2),2)))
    averageDistance = distance/totalCorners
    return averageDistance