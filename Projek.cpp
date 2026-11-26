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

// ==========================================
// 2. DATA GLOBAL
// ==========================================

vector<Kelas> daftarKelas = {
    {"Ruang 101", true},
    {"Ruang 102", true},
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
    for (size_t i = 0; i < daftarKelas.size(); i++) {
        for (size_t j = 0; j + 1 < daftarKelas.size() - i; j++) {
            if (daftarKelas[j].namaKelas > daftarKelas[j + 1].namaKelas) {
                swap(daftarKelas[j], daftarKelas[j + 1]);
            }
        }
    }
}

string statusKeString(StatusPinjam s) {
    if (s == MENUNGGU) return "Menunggu";
    if (s == DISETUJUI) return "Disetujui";
    return "Ditolak";
}

// ==========================================
// 4. FITUR USER: CEK, CARI & AJUKAN RUANG
// ==========================================

void tampilkanDanCariKelasKosong() {
    urutkanKelas();
    cout<<string(40,'=')<<endl;
    cout << " DAFTAR SEMUA KELAS KOSONG (URUT A-Z) " << endl;
    cout<<string(40,'=')<<endl;
    cout << left << setw(5) << "No" << setw(20) << "Nama Kelas" << "Status" << endl;
    cout << string(40,'-') << endl;

    int nomor = 1;
    for (size_t i = 0; i < daftarKelas.size(); i++) {
        if (daftarKelas[i].isKosong) {
            cout << left << setw(5) << nomor<< setw(20) << daftarKelas[i].namaKelas
                 << "Kosong" << endl;
            nomor++;
        }
    }
    cout << string(40,'-') << endl;

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
        cout << string(40,'-') << "2\n" << endl;
    }
}

void ajukanPeminjaman(const string& username) {
    urutkanKelas();
    cout << "\n=== AJUKAN PEMINJAMAN RUANGAN ===" << endl;

    vector<int> indeksKosong;
    for (size_t i = 0; i < daftarKelas.size(); i++) {
        if (daftarKelas[i].isKosong) {
            cout << (indeksKosong.size() + 1) << ". " << daftarKelas[i].namaKelas << endl;
            indeksKosong.push_back(static_cast<int>(i));
        }
    }

    if (indeksKosong.empty()) {
        cout << "Tidak ada ruangan kosong saat ini.\n";
        return;
    }

    size_t pilih;
    cout << "Pilih nomor ruangan (0 untuk batal): ";
    cin >> pilih;

    if (pilih == 0) return;
    if (pilih > indeksKosong.size()) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    string keperluan;
    cout << "Masukkan keperluan peminjaman: ";
    cin.ignore();
    getline(cin, keperluan);

    string namaRuang = daftarKelas[indeksKosong[pilih - 1]].namaKelas;
    daftarPinjam.push_back({username, namaRuang, keperluan, MENUNGGU});

    cout << "Permohonan peminjaman " << namaRuang << " berhasil dikirim (Status: Menunggu).\n";
}

// ==========================================
// 5. FITUR ADMIN: PERSETUJUAN PEMINJAMAN
// ==========================================

void permohonanPeminjaman() {
    cout << string(40, '=') << endl;
    cout << "DAFTAR PERMOHONAN PEMINJAMAN" << endl;
    cout << string(40, '=') << endl;

    bool adaData = false;
    for (size_t i = 0; i < daftarPinjam.size(); i++) {
        if (daftarPinjam[i].status == MENUNGGU) {
            string statusStr = statusKeString(daftarPinjam[i].status);
            cout << i + 1 << ". Peminjam: " << daftarPinjam[i].namaPeminjam<< " | Kelas: " << daftarPinjam[i].namaKelas
                 << " | Keperluan: " << daftarPinjam[i].keperluan<< " | Status: " << statusStr << endl;
            adaData = true;
        }
    }

    if (!adaData) {
        cout << "Tidak ada permohonan peminjaman aktif.\n";
        return;
    }

    size_t nomorPilih;
    cout << "\nNomor permohonan (0 untuk batal): ";
    cin >> nomorPilih;

    if (nomorPilih == 0) return;

    if (nomorPilih > daftarPinjam.size()) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    size_t index = nomorPilih - 1;

    if (daftarPinjam[index].status == DITOLAK) {
        cout << "Permohonan sudah ditolak.\n";
        return;
    }

    int keputusan;
    cout << "\n1. Terima (ACC)\n";
    cout << "2. Tolak / Batalkan Persetujuan\n";
    cout << "Pilih: ";
    cin >> keputusan;

    if (keputusan == 1) {
        daftarPinjam[index].status = DISETUJUI;

        for (size_t j = 0; j < daftarKelas.size(); j++) {
            if (daftarKelas[j].namaKelas == daftarPinjam[index].namaKelas) {
                daftarKelas[j].isKosong = false;
                break;
            }
        }

        cout << "Peminjaman berhasil disetujui!\n";
    } else if (keputusan == 2) {
        daftarPinjam[index].status = DITOLAK;

        for (size_t j = 0; j < daftarKelas.size(); j++) {
            if (daftarKelas[j].namaKelas == daftarPinjam[index].namaKelas) {
                daftarKelas[j].isKosong = true;
                break;
            }
        }

        cout << "Peminjaman berhasil ditolak.\n";
    } else {
        cout << "Pilihan tidak valid!\n";
    }
}

// ==========================================
// 6. LOGIN & TAMBAH AKUN
// ==========================================

int login(string& userAktif) {
    string username, password;
    cout << "\n========== LOGIN ==========\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (size_t i = 0; i < daftarAkun.size(); i++) {
        if (daftarAkun[i].username == username && daftarAkun[i].password == password) {
            userAktif = daftarAkun[i].username;
            if (daftarAkun[i].role == "user") {
                cout << "Login berhasil sebagai User!\n";
                return 1;
            } else if (daftarAkun[i].role == "admin") {
                cout << "Login berhasil sebagai Admin!\n";
                return 2;
            }
        }
    }

    cout << "Username atau password salah!\n";
    return 0;
}

void tambahAkun() {
    string username, password;
    string role;
    string kodeAdmin;

    cout << "\n========== TAMBAH AKUN ==========\n";
    cout << "Username baru: ";
    cin >> username;

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
// 7. MENU USER BIASA
// ==========================================

void menuUser(const string& username) {
    int pilihan;

    do {
        cout << "\n========== MENU USER ==========\n";
        cout << "Login sebagai: " << username << "\n";
        cout << "1. Cek & Cari Ruang Tersedia\n";
        cout << "2. Ajukan Peminjaman Ruang\n";
        cout << "3. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tampilkanDanCariKelasKosong();
                break;
            case 2:
                ajukanPeminjaman(username);
                break;
            case 3:
                cout << "Berhasil logout!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 3);
}

// ==========================================
// 8. MENU ADMIN
// ==========================================

void menuAdmin(const string& username) {
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
// 9. PROGRAM UTAMA
// ==========================================

int main() {
    int pilihan;

    do {
        cout << "\n" << string(40, '=') << "\n";
        cout << "       A.R.C ROOM SYSTEM        \n";
        cout << string(40, '=') << "\n";
        cout << "1. Login\n";
        cout << "2. Tambah Akun\n";
        cout << "3. Keluar Program\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                string userAktif;
                int hasilLogin = login(userAktif);

                if (hasilLogin == 1) {
                    menuUser(userAktif);
                } else if (hasilLogin == 2) {
                    menuAdmin(userAktif);
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