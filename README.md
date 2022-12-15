# DIP
# This project consists of various image processing techniques using c++ and a little bit of Opencv. 
# You can consider this as a standard made library which can provide various functionalities for both gray scale and RGB images such as:
  1. Uniform and Guassian Smoothing 
  2. Sobel and Canny Edge Detection
  3. QR Detection
  4. Thresholding
  5. Low pass and high pass filters in fourier domain
  6. Fourier Transformation
  
# To view the functions you can checkout the utility files. 
# To run the repository, run the make file.
  make
 #For further running the library you can make changes to the parameters.txt in the following format
    inputfile outputfile opencv no.of.roi x y Sx Sy function_name p1 p2
 x, y, Sx, Sy are the co-ordinates for the roi region. 
 
 # After modifying the parameters file, you can run the following command 
 ./iptool parameters.txt
 
