/**
 * Bagian yang aku kerjain  : Fitur Pencarian Ruang Kosong & Persetujuan Peminjaman (Admin)
 * Project      : ARC (Academic Reservation Classroom)
 * Nama       : Iqbal Aditya
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// STRUKTUR DATA & REFERENSI GLOBAL
// ==========================================

enum StatusPinjam {
    MENUNGGU,
    DISETUJUI,
    DITOLAK
};

struct Kelas {
    string namaKelas;
    bool isKosong;
};

struct Peminjaman {
    string namaPeminjam;
    string namaKelas;
    string keperluan;
    StatusPinjam status;
};

extern vector<Kelas> daftarKelas;
extern vector<Peminjaman> daftarPinjam;
extern void urutkanKelas();

// ==========================================
// 1. FITUR USER: CEK & CARI RUANG KOSONG
// ==========================================

void tampilkanDanCariKelasKosong() {
    urutkanKelas();

    cout << "\n=== DAFTAR SEMUA KELAS KOSONG (URUT A-Z) ===" << endl;
    cout << left << setw(5) << "No" << setw(20) << "Nama Kelas" << "Status" << endl;
    cout << "------------------------------------" << endl;

    int nomor = 1;
    for (size_t i = 0; i < daftarKelas.size(); i++) {
        if (daftarKelas[i].isKosong) {
            cout << left << setw(5) << nomor 
                 << setw(20) << daftarKelas[i].namaKelas 
                 << "Kosong" << endl;
            nomor++;
        }
    }
    cout << "------------------------------------" << endl;

    // Fitur Searching Ruang Kosong (Linear Search + Substring Matching)
    char mauCari;
    cout << "Apakah ingin mencari nama ruangan tertentu? (y/n): ";
    cin >> mauCari;

    if (mauCari == 'y' || mauCari == 'Y') {
        string kataKunci;
        cout << "Masukkan kata kunci nama ruangan (contoh: Ruang 101 / Aula): ";
        cin.ignore();
        getline(cin, kataKunci);

        cout << "\n--- Hasil Pencarian Ruang Kosong ---" << endl;
        bool ketemu = false;
        for (size_t i = 0; i < daftarKelas.size(); i++) {
            if (daftarKelas[i].namaKelas.find(kataKunci) != string::npos && daftarKelas[i].isKosong) {
                cout << "- " << daftarKelas[i].namaKelas << " [TERSEDIA / KOSONG]" << endl;
                ketemu = true;
            }
        }

        if (!ketemu) {
            cout << "Ruangan dengan kata kunci \"" << kataKunci << "\" tidak ditemukan atau sedang dipakai." << endl;
        }
        cout << "------------------------------------\n" << endl;
    }
}

// ==========================================
// 2. FITUR ADMIN: PERSETUJUAN PEMINJAMAN
// ==========================================

void permohonanPeminjaman() {
    cout << "\n=== DAFTAR PERMOHONAN PEMINJAMAN ===" << endl;
    bool adaData = false;

    for (size_t i = 0; i < daftarPinjam.size(); i++) {
        if (daftarPinjam[i].status == MENUNGGU || daftarPinjam[i].status == DISETUJUI) {
            string labelStatus = (daftarPinjam[i].status == MENUNGGU) ? "MENUNGGU" : "DISETUJUI";
            cout << i + 1 << ". Peminjam: " << daftarPinjam[i].namaPeminjam 
                 << " | Kelas: " << daftarPinjam[i].namaKelas 
                 << " | Keperluan: " << daftarPinjam[i].keperluan 
                 << " [" << labelStatus << "]" << endl;
            adaData = true;
        }
    }

    if (!adaData) {
        cout << "Tidak ada permohonan peminjaman aktif.\n" << endl;
        return;
    }

    size_t nomorPilih;
    cout << "\nMasukkan nomor urut permohonan yang ingin diproses/diubah (0 untuk batal): ";
    cin >> nomorPilih;

    if (nomorPilih > 0 && nomorPilih <= daftarPinjam.size()) {
        size_t index = nomorPilih - 1;
        
        if (daftarPinjam[index].status == DITOLAK) {
            cout << "[GAGAL] Permohonan ini sudah ditolak.\n" << endl;
            return;
        }

        int keputusan;
        cout << "Tindakan untuk " << daftarPinjam[index].namaPeminjam << " (" << daftarPinjam[index].namaKelas << "):" << endl;
        cout << "1. Terima (ACC)" << endl;
        cout << "2. Tolak / Batalkan Persetujuan" << endl;
        cout << "Pilih (1/2): ";
        cin >> keputusan;

        if (keputusan == 1) {
            daftarPinjam[index].status = DISETUJUI;

            // Sinkronisasi: Tandai ruang kelas menjadi terpakai (false)
            for (size_t j = 0; j < daftarKelas.size(); j++) {
                if (daftarKelas[j].namaKelas == daftarPinjam[index].namaKelas) {
                    daftarKelas[j].isKosong = false;
                    break;
                }
            }
            cout << "[SUKSES] Peminjaman DISETUJUI! Kelas otomatis ditandai terpakai.\n" << endl;
        } else if (keputusan == 2) {
            daftarPinjam[index].status = DITOLAK;

            // Sinkronisasi: Kembalikan ruang kelas menjadi kosong (true)
            for (size_t j = 0; j < daftarKelas.size(); j++) {
                if (daftarKelas[j].namaKelas == daftarPinjam[index].namaKelas) {
                    daftarKelas[j].isKosong = true;
                    break;
                }
            }
            cout << "[INFO] Peminjaman DITOLAK / DIBATALKAN. Status ruangan kembali kosong.\n" << endl;
        } else {
            cout << "[GAGAL] Pilihan tindakan tidak valid.\n" << endl;
        }
    } else if (nomorPilih != 0) {
        cout << "[GAGAL] Nomor urut tidak valid.\n" << endl;
    }
}