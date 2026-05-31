# LWZip — Cross-Platform Archive Manager

## Project Overview

基於 [ip7z/7zip](https://github.com/ip7z/7zip) 原始碼修改的跨平台壓縮/解壓縮 GUI 程式。
支援 macOS 與 Windows，具備編碼切換、密碼加解密、多格式支援等功能。

## Architecture

### Tech Stack

- **Language:** C++ (與 7zip 原始碼一致)
- **GUI Framework:** Qt 6 (跨平台)
- **Core Engine:** 7zip C/C++ core (直接整合，非外部依賴)
- **Build System:** CMake
- **Distribution:** Portable executables (macOS / Windows)

### Directory Structure

```
lwzip/
├── Asm/           — 7zip 組合語言優化 (CRC, SHA, AES, LZMA)
├── C/             — 7zip C 核心 (LZMA SDK)
├── CPP/           — 7zip C++ 層 (Archive, Compress, Crypto, UI)
├── DOC/           — 7zip 原始文件
├── GUI/           — [新增] Qt 跨平台 GUI
│   ├── FileManager/   — 完整檔案管理器 (類似 7zFM)
│   ├── Simple/        — 簡易壓縮/解壓工具 (類似 7zG)
│   ├── Common/        — 共用 UI 元件 (編碼選擇器、密碼對話框等)
│   ├── i18n/          — 多語系翻譯檔
│   └── Resources/     — 圖示、樣式表等資源
├── CMakeLists.txt — 頂層 CMake 建構檔
└── CLAUDE.md
```

## Features

### 1. 雙模式 GUI

- **File Manager 模式:** 完整檔案管理器，可瀏覽壓縮檔內容、拖放操作
- **Simple 模式:** 輕量壓縮/解壓介面，右鍵選單整合

### 2. 壓縮格式支援

**讀寫 (壓縮 + 解壓):**
- 7z, ZIP (預設), XZ, GZIP, BZIP2, TAR, WIM

**唯讀 (僅解壓):**
- RAR (含 RAR5), CAB, ISO, NSIS, UDF, ARJ, CPIO, RPM, ZSTD, CHM, VHD, XAR, SquashFS, QCOW, APM, HFS

**預設壓縮格式:** ZIP

### 3. 編碼切換 (Encoding Switching)

- GUI 內建編碼選擇按鈕，可即時切換顯示編碼
- 支援編碼：UTF-8, Shift-JIS, Big5, GBK, EUC-KR, ISO-8859-1, Windows-1252 等
- 壓縮檔內的檔名顯示會即時跟隨編碼切換更新
- 解壓時可指定目標編碼，避免亂碼問題
- 整合 7zip 現有的 `-mcl` (local code page) / `-mcu` (UTF-8) 邏輯

### 4. 密碼加密/解密

- 壓縮時可設定密碼 (支援 AES-256 加密)
- 解壓時彈出密碼輸入對話框
- 支援加密格式：7z AES, ZIP AES, ZipCrypto
- 密碼強度提示 UI

### 5. 多語系 (i18n)

- GUI 介面語言可透過按鈕即時切換
- 初期支援：繁體中文、簡體中文、英文、日文
- 使用 Qt Linguist (.ts/.qm) 翻譯系統

### 6. 跨平台

- **macOS:** 原生 macOS 風格，支援 Intel + Apple Silicon
- **Windows:** 原生 Windows 風格，支援 x64
- Portable executable 形式發布，無需安裝

## Build Instructions

```bash
# Prerequisites: Qt 6, CMake 3.20+, C++17 compiler

mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
cmake --build . --parallel
```

## Design Principles

- 保持 7zip core 的修改最小化，方便未來同步上游更新
- Qt GUI 層與 7zip core 透過清晰的 interface 隔離
- 編碼處理集中在 `GUI/Common/` 的 EncodingManager 模組
- 所有 UI 字串使用 Qt tr() 機制，支援動態語言切換

## Development Guidelines

- 遵循 7zip 原有的 coding style (C++ 層)
- Qt GUI 層使用 Qt 官方推薦的命名慣例
- 新增功能需同時考慮 macOS 和 Windows 的行為差異
- 編碼相關邏輯必須有完整的 unit test 覆蓋
