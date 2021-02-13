# waifu2x-ncnn-vulkan-GUI

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

### System Requirements

<p>OS: Windows 7 or higher 64bit PC (There is no 32-bit exe.)</p>

### Recommended

<p>OS: Windows10 64bit</p>
<p>CPU: Intel Core i3 or adove, AMD Ryzen 3 or above</p>
<p>RAM: 8GB or above</p>
<p>GPU: NVIDIA GeForce GTX 1060 or above</p>

### My own test environment

<p>Develpoment: Microsoft Visual Studio 2019</p>
<p>OS: Windows10 Pro 64bit, 20H2</p>
<p>CPU: Intel(R) Core(TM) i9-9900K CPU @ 3.60GHz</p>
<p>GPU: NVIDIA Geforce GTX 1070Ti</p>
<p>RAM: 32GB</p>

---

### Others

<p>Used tools</p>
<p>OpenCV: https://opencv.org/</p>
<p>FFmpeg: https://ffmpeg.org</p>
<p>waifu2x-ncnn-vulkan: https://github.com/nihui/waifu2x-ncnn-vulkan</p>

---

### ライセンスについて

<p>This tool is released under the MIT license.</p>
