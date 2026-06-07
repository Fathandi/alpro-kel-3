"""
Fashion MNIST Sample Image Downloader
=====================================
Script untuk mengunduh 100 gambar sample acak dari dataset Fashion MNIST
dan menyimpannya dalam format PNG untuk keperluan testing.

Dibuat oleh: Fathih Apriandi
"""

import tensorflow as tf
import numpy as np
from PIL import Image
import os
import random

# ============================================================================
# KONFIGURASI FOLDER OUTPUT
# ============================================================================

# Nama folder tujuan penyimpanan gambar sample
folder_output = "test-image"

# Membuat folder jika belum ada, exist_ok=True mencegah error jika folder sudah ada
# Fungsi ini penting agar program tidak crash saat folder sudah tersedia
os.makedirs(folder_output, exist_ok=True)

# ============================================================================
# LOAD DATASET FASHION MNIST
# ============================================================================

# Load dataset Fashion MNIST resmi dari Keras
# Dataset ini berisi 70.000 gambar grayscale 28x28 pixels:
# - x_train: 60.000 gambar untuk training
# - y_train: 60.000 label untuk training (0-9)
# - x_test: 10.000 gambar untuk testing
# - y_test: 10.000 label untuk testing (0-9)
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.fashion_mnist.load_data()

# ============================================================================
# GABUNGKAN SEMUA DATA
# ============================================================================

# Menggabungkan data training dan testing menjadi satu array
# Menggunakan np.concatenate untuk menggabung array sepanjang axis 0 (baris)
# Tujuan: agar kita bisa random sampling dari keseluruhan 70.000 gambar
# tanpa bias hanya dari training atau testing set saja
x_semua_data = np.concatenate((x_train, x_test))

# ============================================================================
# RANDOM SAMPLING 100 GAMBAR
# ============================================================================

# Mengambil 100 index acak dari total 70.000 gambar
# random.sample() memastikan tidak ada duplikasi index
# range(len(x_semua_data)) menghasilkan angka 0 sampai 69999
# Parameter 100 adalah jumlah sample yang diinginkan
indeks_terpilih = random.sample(range(len(x_semua_data)), 100)

# ============================================================================
# PROSES PENYIMPANAN GAMBAR
# ============================================================================

# Loop untuk setiap index yang terpilih
# enumerate() memberikan counter otomatis mulai dari 0
# i = counter iterasi (0-99), idx = index gambar di dataset (random)
for i, idx in enumerate(indeks_terpilih):
    # Ambil array gambar dari dataset berdasarkan index
    # array_gambar berisi matrix 28x28 dengan nilai pixel 0-255
    array_gambar = x_semua_data[idx]
    
    # Konversi numpy array menjadi PIL Image object
    # PIL Image diperlukan untuk menyimpan gambar ke file
    gambar = Image.fromarray(array_gambar)
    
    # Konversi ke mode grayscale (L = Luminance)
    # Meskipun dataset sudah grayscale, konversi ini memastikan
    # format file PNG tersimpan dengan benar dalam mode grayscale
    gambar = gambar.convert("L")
    
    # Membuat nama file dengan format sample_1.png, sample_2.png, dst
    # i+1 karena kita ingin mulai dari 1 bukan 0 (lebih user-friendly)
    nama_file = f"sample_{i+1}.png"
    
    # Menyimpan gambar ke folder output dengan nama file yang sudah ditentukan
    # os.path.join() menggabungkan path folder dan nama file secara cross-platform
    gambar.save(os.path.join(folder_output, nama_file))

# ============================================================================
# PESAN SELESAI
# ============================================================================

# Menampilkan pesan konfirmasi bahwa proses berhasil
print(f"Selesai! 100 gambar acak disimpan di folder '{folder_output}'")