# waifu2x-ncnn-vulkan-GUI

画像アップスケーリングツール([waifu2x-ncnn-vulkan](https://github.com/nihui/waifu2x-ncnn-vulkan))のGUI版。
<p>waifu2x-ncnn-vulkan(コマンドライン)にユーザーインターフェース機能をを提供する。</p>
<p>本ツールは画像だけでなく、動画をアップスケーリングすることも可能。</p>

---

## ダウンロード

[Release](https://github.com/XyLe-GBP/waifu2x-ncnn-vulkan-GUI/releases)

---

## 概要

<p>本アプリケーション実行には、以下の再頒布パッケージのインストールが必要です。</p>

[Microsoft Visual C++ Runtime](https://aka.ms/vs/16/release/VC_redist.x64.exe)

<p>まだインストールしていない方は上記からダウンロードし、PCにインストールしてください。</p>
<p>再頒布パッケージをインストールしたくない、という方は下記DLLを手動で<code>C:/Windows/System32/</code>に配置するかもしくはEXEと同階層に置くことでも起動可能です。</p>

<code>MSVCP140.DLL</code>,<code>VCRUNTIME140.DLL</code>,<code>MFC140U.DLL</code>が最低限必要です。
<p>(※v1.3 以前のバージョンでは必要ありません)</p>

<p>注意として、waifu2xはGPUを使用してアップスケーリングを行います。内臓グラフィックでも処理は可能ですが、GPUを使用するとより高速に変換することが可能です。</p>
<p>※CPU内臓グラフィックでも変換は可能ですが、動画の場合は一度フレーム分割を行ってからアップスケーリングを行うため、処理にものすごく時間が掛かります。</p>

---

### 最低必須

<p>OS: Windows 7 以上の 64bit PC (32ビット用exeはありません)</p>

### 推薦

<p>OS: Windows10 64bit</p>
<p>CPU: Intel Core i3以上, AMD Ryzen 3以上</p>
<p>RAM: 8GB 以上</p>
<p>GPU: NVIDIA GeForce GTX 1060 以上</p>

### 自身のテスト環境

<p>Develpoment: Microsoft Visual Studio 2019</p>
<p>OS: Windows10 Pro 64bit, 20H2</p>
<p>CPU: Intel(R) Core(TM) i9-9900K CPU @ 3.60GHz</p>
<p>GPU: NVIDIA Geforce GTX 1070Ti</p>
<p>RAM: 32768MB</p>

---

### その他

<p>使用ツール</p>
<p>OpenCV: https://opencv.org/</p>
<p>FFmpeg: https://ffmpeg.org</p>
<p>waifu2x-ncnn-vulkan: https://github.com/nihui/waifu2x-ncnn-vulkan</p>

---

### ライセンスについて

<p>このツールはMITライセンスでリリースされています。</p>
