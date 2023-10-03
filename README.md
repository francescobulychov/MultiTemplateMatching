# Multi template matching using OpenCV
Simple code that allows real-time detection of multiple and different objects from a program.  


<img src="https://github.com/francescobulychov/MultiTemplateMatching/assets/144119910/81c2854c-b8d6-42ce-9bc4-dd14b4dc5dc3" width="400" height="400" />



## How it works
The program allows to use images provided by the user to conduct a real-time search for similar templates, utilizing an accuracy setting that can be configured. This operation can be performed simultaneously on multiple images.

OpenCV offers various [comparison methods](https://docs.opencv.org/4.x/d4/dc6/tutorial_py_template_matching.html), which can give better or worse results depending on the context. There are also different [thresholding methods](https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html) to filter out results that don't meet a certain accuracy level.  
In my case I found that the best results come from using the `TM_SQDIFF_NORMED` comparison method and the `THRESH_BINARY_INV` thresholding method. I also noticed that is possible to improve results by first converting the colors from BGR to HSV.

## How to use the program
- Add the images (.png or .jpg) of the objects inside the folder of the executable.
- Select the program writing the name of its window here: `LPCWSTR windowTitle = L"Program Name";`.
- Select the image using the function: `detectImage(img, output, "image_path.png", accuracy);`.
  - Repeat for every image you want to detect.

## How to install OpenCV (Visual Studio)
**Attention**: in this example the OpenCV folder is located in `C:/opencv` and version `4.8.0` has been used.  
Using a different path or version may require some changes.
- Download and install the workload `Desktop development with C++` from Visual Studio Installer.
- Download the last OpenCV release from the [official repository](https://github.com/opencv/opencv).
- In the properties of the project, add the following files:
  - `VC++ Directories` > `Include Directories` > `C:\opencv\build\include`
  - `VC++ Directories` > `Library Directories` > `C:\opencv\build\x64\vc16\lib`
  - `Linker` > `Input` > `Additional Dependencies` > `opencv_world480.lib`
- Copy `C:\opencv\build\x64\vc16\bin\opencv_world480.dll` in the folder of the executable.


