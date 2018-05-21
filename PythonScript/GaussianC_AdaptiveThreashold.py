# import cv2 as cv
# import numpy as np
# from matplotlib import pyplot as plt
# img = cv.imread('../WatchMe/Images/poor_3.bmp',0)
# # global thresholding
# ret1,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
# # Otsu's thresholding
# ret2,th2 = cv.threshold(img,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
# # Otsu's thresholding after Gaussian filtering
# blur = cv.GaussianBlur(img,(5,5),0)
# ret3,th3 = cv.threshold(blur,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
# # plot all the images and their histograms
# images = [img, 0, th1,
          # img, 0, th2,
          # blur, 0, th3]
# titles = ['Original Noisy Image','Histogram','Global Thresholding (v=127)',
          # 'Original Noisy Image','Histogram',"Otsu's Thresholding",
          # 'Gaussian filtered Image','Histogram',"Otsu's Thresholding"]
# for i in range(3):
    # plt.subplot(3,3,i*3+1),plt.imshow(images[i*3],'gray')
    # plt.title(titles[i*3]), plt.xticks([]), plt.yticks([])
    # plt.subplot(3,3,i*3+2),plt.hist(images[i*3].ravel(),256)
    # plt.title(titles[i*3+1]), plt.xticks([]), plt.yticks([])
    # plt.subplot(3,3,i*3+3),plt.imshow(images[i*3+2],'gray')
    # plt.title(titles[i*3+2]), plt.xticks([]), plt.yticks([])
# plt.show()




# import cv2 as cv
# import numpy as np
# from matplotlib import pyplot as plt
# img = cv.imread('../WatchMe/Images/poor_3.bmp',0)
# img = cv.medianBlur(img,5)
# ret,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
# th2 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C,\
            # cv.THRESH_BINARY,11,2)
# th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            # cv.THRESH_BINARY,11,2)
# titles = ['Original Image', 'Global Thresholding (v = 127)',
            # 'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
# images = [img, th1, th2, th3]
# for i in range(4):
    # plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
    # plt.title(titles[i])
    # plt.xticks([]),plt.yticks([])
# plt.show()


import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('../WatchMe/Images/poor_3.bmp',0)

# global thresholding
ret1,th1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)

# Otsu's thresholding
ret2,th2 = cv2.threshold(img,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

# Otsu's thresholding after Gaussian filtering
blur = cv2.GaussianBlur(img,(5,5),0)
ret3,th3 = cv2.threshold(blur,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

# plot all the images and their histograms
images = [img, 0, th1,
          img, 0, th2,
          blur, 0, th3]
titles = ['Original Noisy Image','Histogram','Global Thresholding (v=127)',
          'Original Noisy Image','Histogram',"Otsu's Thresholding",
          'Gaussian filtered Image','Histogram',"Otsu's Thresholding"]

for i in range(3):
    plt.subplot(3,3,i*3+1),plt.imshow(images[i*3],'gray')
    plt.title(titles[i*3]), plt.xticks([]), plt.yticks([])
    plt.subplot(3,3,i*3+2),plt.hist(images[i*3].ravel(),256)
    plt.title(titles[i*3+1]), plt.xticks([]), plt.yticks([])
    plt.subplot(3,3,i*3+3),plt.imshow(images[i*3+2],'gray')
    plt.title(titles[i*3+2]), plt.xticks([]), plt.yticks([])
plt.show()
