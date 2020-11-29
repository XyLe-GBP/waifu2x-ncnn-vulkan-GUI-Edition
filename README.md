# waifu2x-ncnn-vulkan-GUI

画像アップスケールツールのGUI版

## ダウンロード

[Release](https://github.com/XyLe-GBP/waifu2x-ncnn-vulkan-GUI/releases)

## 概要

<p>本アプリケーション実行には、以下の再頒布パッケージのインストールが必要です。</p>

[Microsoft Visual C++ Runtime](https://aka.ms/vs/16/release/VC_redist.x64.exe)

<p>まだインストールしていない方は上記からダウンロードし、PCにインストールしてください。</p>
<p>再頒布パッケージをインストールしたくない、という方は下記DLLを手動で<code>C:/Windows/System32/</code>に配置するかもしくはEXEと同階層に置くことでも起動可能です。</p>

<code>MSVCP140.DLL</code>,<code>VCRUNTIME140.DLL</code>,<code>MFC140U.DLL</code>が最低限必要です。
<p>(※v1.3 以前のバージョンでは必要ありません)</p>

---

### 最低必須

<p>OS: Windows 7 以上の 64bit PC (32ビット用exeはありません)</p>
<p>CPU: Intel Core i3 以上(Core i5 以上推薦) または、AMD Ryzen 3 以上(Ryzen 5 以上推薦)</p>
<p>RAM: 4GB以上(8GB以上あると尚良い)</p>
<p>GPU: NVIDIA Geforce または NVIDIA TITAN もしくは AMD Radeon などのGPU(グラフィックボード)</p>
<p>※グラフィックボードなしのCPU内臓グラフィックでも変換は可能ですが、動画の場合は動画の長さにもよりますが処理にものすごく時間が掛かります。</p>

### 推薦

<p>OS: Windows10 64bit</p>
<p>CPU: Intel Core i7, AMD Ryzen 7</p>
<p>RAM: 8GB 以上</p>
<p>GPU: NVIDIA GeForce GTX 1060 以上</p>

---

### その他

<p>使用ツール</p>
<p>OpenCV (image2png.exe)</p>
<p>FFmpeg</p>
waifu2x-ncnn-vulkan: https://github.com/nihui/waifu2x-ncnn-vulkan

### ライセンスについて

<p>このツールはMITライセンスでリリースされています。</p>
