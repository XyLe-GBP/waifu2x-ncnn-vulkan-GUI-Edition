# waifu2x-ncnn-vulkan-GUI-Edition

GUI version of the image upscaling tool ([waifu2x-ncnn-vulkan](https://github.com/nihui/waifu2x-ncnn-vulkan)).
<p>Provides user interface functionality to waifu2x-ncnn-vulkan (command line). </p>
<p>This tool can upscale not only images but also videos. </p>

---

## Download

[Release](https://github.com/XyLe-GBP/waifu2x-ncnn-vulkan-GUI/releases)

---

## Description

The following redistribution package must be installed in order to run this application.

[Microsoft Visual C++ Runtime](https://aka.ms/vs/16/release/VC_redist.x64.exe)

If you have not installed it yet, please download it from above and install it on your PC.

If you do not want to install the redistribution package, you can start the program by manually placing the following DLLs in C:/Windows/System32/ or in the same directory as the EXE.

At least <code>MSVCP140.DLL</code>, <code>VCRUNTIME140.DLL</code>, and <code>MFC140U.DLL</code> are required.

(*Not required for v1.3 or earlier versions)

As a reminder, waifu2x uses the GPU to do the upscaling. Although processing is possible with the built-in graphics, using the GPU allows for faster conversion.

If you want to convert a video, you need to split the frame before upscaling, which takes a lot of time.

---

### What is this?

You can enlarge in while keeping the image clean.

Examples:  
**※Click on the image to enlarge.**  

<kbd><img src="https://user-images.githubusercontent.com/59692068/124342813-b6824200-dc01-11eb-9603-7e5127ef55af.jpg" width="350"><br>**Original (640x800)**</kbd>
<kbd><img src="https://user-images.githubusercontent.com/59692068/124342863-2395d780-dc02-11eb-94e7-05e4ba0f8b6a.png" width="350"><br>**waifu2x (640x800, Reduction Level2, CUnet)**</kbd>

<kbd><img src="https://user-images.githubusercontent.com/59692068/124342967-fa297b80-dc02-11eb-845f-90fb6236dd82.png" width="350"><br>**Enlarged (1280x1600, Bilinear interpolation)**</kbd>
<kbd><img src="https://user-images.githubusercontent.com/59692068/124342972-0ca3b500-dc03-11eb-8ac5-ba8cf4b21ca6.png" width="350"><br>**Enlarged (1280x1600, Lanczos3 interpolation)**</kbd>

<kbd><img src="https://user-images.githubusercontent.com/59692068/124342976-14635980-dc03-11eb-8207-feb18e0e36de.png" width="350"><br>**waifu2x (1280x1600, Reduction Level2, CUnet)**</kbd>
<kbd><img src="https://user-images.githubusercontent.com/59692068/124343761-07496900-dc09-11eb-8465-ca91b6839623.png" width="350"><br>**waifu2x (1280x1600, Reduction Level2, RGB)**</kbd>

<kbd><img src="https://user-images.githubusercontent.com/59692068/124343766-0d3f4a00-dc09-11eb-8ed6-53f80b983930.png" width="350"><br>**waifu2x (1280x1600, Reduction Level2, Photo)**</kbd>
<kbd><img src="https://user-images.githubusercontent.com/59692068/124343864-c4d45c00-dc09-11eb-97fd-337a68323bd4.png" width="350"><br>**waifu2x (1280x1600, Reduction Level3, CUnet, TTA)**</kbd>

Image Source: **THE IDOLM@STER: Haruka Amami**  

Image upscaling is the main focus, but video upscaling is also possible.  
See the comparison video below.  

[Upscaling comparison video](https://www.youtube.com/embed/hU3T_Gu3Ehk)

When upscaling video, first use FFmpeg to perform frame splitting.  
When you do this, it puts a very large load on your PC.  
If you have a low-spec PC, it may take a very long time.  

Basically, the time taken for upscaling is proportional to the time and resolution of the video that is the target of upscaling.  
This means that upscaling a video with a long duration and relatively high resolution will take a long time.  

However, if your PC is equipped with a powerful GPU, this process can be shortened.  
Upscaling speed is highly dependent on the performance of the GPU.  
The higher the performance of the GPU, the faster the upscaling speed will be.  
It is not recommended to use the CPU's built-in graphics (iGPU).  
NVIDIA or AMD (RADEON) GPU is preferred.

---

### System Requirements

<p>OS: Windows 7 or higher 64bit PC (There is no 32-bit exe.)</p>

### Recommended

<p>OS: Windows10 64bit</p>
<p>CPU: Intel Core i3 or adove, AMD Ryzen 3 or above</p>
<p>RAM: 8GB or above</p>
<p>GPU: NVIDIA GeForce GTX 1060 or above</p>

### My own test environment

<p>Development: Microsoft Visual Studio 2019</p>
<p>OS: Windows10 Pro 64bit, 20H2</p>
<p>CPU: Intel(R) Core(TM) i9-9900K CPU @ 3.60GHz</p>
<p>GPU: NVIDIA Geforce GTX 1070Ti</p>
<p>RAM: 32GB</p>

---

### Others

Used tools  

[OpenCV](https://opencv.org/)  
[FFmpeg](https://ffmpeg.org)  
[waifu2x-ncnn-vulkan](https://github.com/nihui/waifu2x-ncnn-vulkan)  

---

### About Licensing

<p>This tool is released under the MIT license.</p>
