
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// 1. STRUKTUR DATA & REFERENSI GLOBAL
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

struct Akun {
    string username;
    string password;
    string role;
};

extern vector<Kelas> daftarKelas;
extern vector<Peminjaman> daftarPinjam;
extern void urutkanKelas();



// ==========================================
// 2. DATA GLOBAL
// ==========================================

vector<Kelas> daftarKelas = {
    {"Ruang 101", true},
    {"Ruang 102", false},
    {"Ruang 103", true},
    {"Ruang 104", true},
    {"Aula", true}
};

vector<Peminjaman> daftarPinjam;

vector<Akun> daftarAkun = {
    {"rafael", "123456", "user"},
    {"admin", "admin123", "admin"}
};

const string PASSWORD_ADMIN = "123321";

// ==========================================
// 3. FUNGSI TAMBAHAN
// ==========================================

void urutkanKelas() {
    for (int i = 0; i < daftarKelas.size(); i++) {
        for (int j = 0; j < daftarKelas.size() - i - 1; j++) {
            if (daftarKelas[j].namaKelas >
                daftarKelas[j + 1].namaKelas) {

                swap(daftarKelas[j],
                     daftarKelas[j + 1]);
            }
        }
    }
}


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
// 5. FITUR ADMIN: PERSETUJUAN PEMINJAMAN
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

    string username, password;
    string role;
    string kodeAdmin;

    cout << "\n========== TAMBAH AKUN ==========\n";

    cout << "Username baru: ";
    cin >> username;

    // Cek username sudah digunakan

    for (size_t i = 0; i < daftarAkun.size(); i++) {

        if (daftarAkun[i].username == username) {

            cout << "Username sudah digunakan!\n";
            return;
        }
    }

    cout << "Password baru: ";
    cin >> password;

    int pilihan;

    cout << "\nJenis akun:\n";
    cout << "1. User Biasa\n";
    cout << "2. Admin\n";
    cout << "Pilih: ";
    cin >> pilihan;

    if (pilihan == 1) {

        role = "user";

    } else if (pilihan == 2) {

        cout << "Masukkan password khusus admin: ";
        cin >> kodeAdmin;

        if (kodeAdmin != PASSWORD_ADMIN) {

            cout << "Password khusus admin salah!\n";
            return;
        }

        role = "admin";

    } else {

        cout << "Pilihan tidak valid!\n";
        return;
    }

    daftarAkun.push_back({username, password, role});

    cout << "\nAkun berhasil dibuat!\n";
    cout << "Username: " << username << "\n";
    cout << "Jenis akun: " << role << "\n";
}

// ==========================================
// 8. MENU USER BIASA
// ==========================================

void menuUser(string username) {

    int pilihan;

    do {

        cout << "\n========== MENU USER ==========\n";
        cout << "Login sebagai: " << username << "\n";

        cout << "1. Cek Ruang Tersedia\n";
        cout << "2. Logout\n";

        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {

            case 1:
                tampilkanDanCariKelasKosong();
                break;

            case 2:
                cout << "Berhasil logout!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 2);
}

// ==========================================
// 9. MENU ADMIN
// ==========================================

void menuAdmin(string username) {

    int pilihan;

    do {

        cout << "\n========== MENU ADMIN ==========\n";
        cout << "Login sebagai: " << username << "\n";

        cout << "1. Persetujuan Pinjaman\n";
        cout << "2. Logout\n";

        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {

            case 1:
                permohonanPeminjaman();
                break;

            case 2:
                cout << "Berhasil logout!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 2);
}

// ==========================================
// 10. PROGRAM UTAMA
// ==========================================

int main() {

    int pilihan;

    do {

        cout << "\n================================\n";
        cout << "       A.R.C ROOM SYSTEM        \n";
        cout << "================================\n";

        cout << "1. Login\n";
        cout << "2. Tambah Akun\n";
        cout << "3. Keluar Program\n";

        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {

            case 1: {

                int hasilLogin = login();

                if (hasilLogin == 1) {

                    menuUser("rafael");

                } else if (hasilLogin == 2) {

                    menuAdmin("admin");
                }

                break;
            }

            case 2:
                tambahAkun();
                break;

            case 3:
                cout << "\nTerima kasih telah menggunakan ARC!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 3);

    return 0;
}