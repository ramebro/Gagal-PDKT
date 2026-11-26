
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// ==========================================
// 1. STRUKTUR DATA
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
    {"Ruang B11 101", true},
    {"Ruang B11 102", true},
    {"Ruang B11 103", true},
    {"Ruang B11 104", true},
    {"Ruang B11 105", true},
    {"Ruang B11 201", true},
    {"Ruang B11 202", true},
    {"Ruang B11 203", true},
    {"Ruang B11 204", true},
    {"Ruang B11 205", true},
    {"Ruang B12 101", true},
    {"Ruang B12 102", true},
    {"Ruang B12 103", true},
    {"Ruang B12 104", true},
    {"Ruang B12 105", true},
    {"Ruang B12 201", true},
    {"Ruang B12 202", true},
    {"Ruang B12 203", true},
    {"Ruang B12 204", true},
    {"Ruang B12 205", true},
    {"Aula B12", true}
};

vector<Peminjaman> daftarPinjam;

vector<Akun> daftarAkun = {
    {"rafael", "123456", "user"},
    {"admin", "admin123", "admin"}
};

const string PASSWORD_ADMIN = "123321";

// 3. FUNGSI TAMBAHAN
void urutkanKelas() {
    for (size_t i = 0; i < daftarKelas.size(); i++) {
        for (size_t j = 0; j + 1 < daftarKelas.size() - i; j++) {
            if (daftarKelas[j].namaKelas > daftarKelas[j + 1].namaKelas) {
                swap(daftarKelas[j], daftarKelas[j + 1]);
            }
        }
    }
}

// Mengubah enum status menjadi tulisan
string statusKeString(StatusPinjam s) {
    if (s == MENUNGGU) return "Menunggu";
    if (s == DISETUJUI) return "Disetujui";
    return "Ditolak";
}

// 4. FITUR USER: CEK & CARI RUANG KOSONG

void tampilkanDanCariKelasKosong() {
    urutkanKelas();

    // Mengumpulkan indeks ruangan kosong
    vector<int> indeksKosong;

    for (size_t i = 0; i < daftarKelas.size(); i++) {
        if (daftarKelas[i].isKosong) {
            indeksKosong.push_back(i);
        }
    }

    if (indeksKosong.empty()) {
        cout << "\nTidak ada ruangan kosong saat ini.\n";
        return;
    }

    int halaman = 0;
    const int jumlahPerHalaman = 5;
    int pilihan;

    do {
        int totalHalaman =
            (indeksKosong.size() + jumlahPerHalaman - 1)
            / jumlahPerHalaman;

        cout << "\n=== DAFTAR RUANGAN KOSONG ===\n";
        cout << "Halaman " << halaman + 1<< " dari " << totalHalaman << endl;

        cout << left << setw(5) << "No" << setw(20) << "Nama Ruangan"
             << "Status" << endl;
         cout << string(40,'-')<<endl;

        // Menentukan batas data halaman
        int awal = halaman * jumlahPerHalaman;
        int akhir = awal + jumlahPerHalaman;

        if (akhir > indeksKosong.size()) {
            akhir = indeksKosong.size();
        }

        // Menampilkan maksimal 5 ruangan
        for (int i = awal; i < akhir; i++) {
            int indexKelas = indeksKosong[i];

            cout << left << setw(5) << i + 1
                 << setw(20) << daftarKelas[indexKelas].namaKelas
                 << "Kosong" << endl;
        }

        cout << string(40,'-')<<endl;

        cout << "1. Halaman Berikutnya\n";
        cout << "2. Halaman Sebelumnya\n";
        cout << "3. Cari Ruangan\n";
        cout << "0. Kembali ke Menu User\n";
        cout << "Pilih: ";
        cin >> pilihan;

        if (pilihan == 1) {
            if (halaman < totalHalaman - 1) {
                halaman++;
            } else {
                cout << "Ini sudah halaman terakhir!\n";
            }
        }
        else if (pilihan == 2) {
            if (halaman > 0) {
                halaman--;
            } else {
                cout << "Ini sudah halaman pertama!\n";
            }
        }
        else if (pilihan == 3) {
            string kataKunci;

            cout << "Masukkan nama ruangan yang dicari: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, kataKunci);

            cout << "\n--- HASIL PENCARIAN RUANG KOSONG ---\n";

            bool ketemu = false;

            for (size_t i = 0; i < daftarKelas.size(); i++) {
                if (daftarKelas[i].namaKelas.find(kataKunci)
                    != string::npos
                    && daftarKelas[i].isKosong) {

                    cout << "- " << daftarKelas[i].namaKelas
                         << " [TERSEDIA]\n";

                    ketemu = true;
                }
            }

            if (!ketemu) {
                cout << "Ruangan tidak ditemukan atau sedang digunakan.\n";
            }

            cout << string(40,'-')<<endl;
        }
        else if (pilihan != 0) {
            cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 0);
}

// ==========================================
// 5. FITUR USER: AJUKAN PEMINJAMAN
// ==========================================

void ajukanPeminjaman(const string& username) {
    urutkanKelas();

    cout << "\n=== AJUKAN PEMINJAMAN RUANGAN ===\n";

    vector<int> indeksKosong;

    for (size_t i = 0; i < daftarKelas.size(); i++) {
        if (daftarKelas[i].isKosong) {
            cout << indeksKosong.size() + 1
                 << ". " << daftarKelas[i].namaKelas << endl;

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

    if (pilih == 0) {
        return;
    }

    if (pilih > indeksKosong.size()) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    string keperluan;

    cout << "Masukkan keperluan peminjaman: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, keperluan);

    string namaRuang =
        daftarKelas[indeksKosong[pilih - 1]].namaKelas;

    daftarPinjam.push_back({
        username,
        namaRuang,
        keperluan,
        MENUNGGU
    });

    cout << "\nPermohonan peminjaman " << namaRuang
         << " berhasil dikirim (Status: Menunggu).\n";
}

// ==========================================
// 6. FITUR USER: RIWAYAT PEMINJAMAN
// ==========================================

void riwayatPeminjaman(const string& username) {
    int pilihan;

    do {
        cout << "\n========== RIWAYAT PEMINJAMAN ==========\n";
        cout << "1. Semua Riwayat\n";
        cout << "2. Riwayat Disetujui (ACC)\n";
        cout << "3. Riwayat Tidak Disetujui\n";
        cout << "4. Riwayat Menunggu\n";
        cout << "0. Kembali ke Menu User\n";
        cout << "Pilih filter: ";
        cin >> pilihan;

        if (pilihan == 0) {
            break;
        }

        if (pilihan < 1 || pilihan > 4) {
            cout << "Pilihan tidak valid!\n";
            continue;
        }

        cout << "\n========== HASIL RIWAYAT ==========\n";

        cout << left
             << setw(5) << "No"
             << setw(20) << "Nama Ruangan"
             << setw(25) << "Keperluan"
             << "Status" << endl;

        cout << string(65, '-') << endl;

        bool adaRiwayat = false;
        int nomor = 1;

        for (size_t i = 0; i < daftarPinjam.size(); i++) {

            // Hanya riwayat user yang sedang login
            if (daftarPinjam[i].namaPeminjam != username) {
                continue;
            }

            bool sesuaiFilter = false;

            // Filter semua riwayat
            if (pilihan == 1) {
                sesuaiFilter = true;
            }

            // Filter disetujui
            else if (pilihan == 2 &&
                     daftarPinjam[i].status == DISETUJUI) {
                sesuaiFilter = true;
            }

            // Filter ditolak
            else if (pilihan == 3 &&
                     daftarPinjam[i].status == DITOLAK) {
                sesuaiFilter = true;
            }

            // Filter menunggu
            else if (pilihan == 4 &&
                     daftarPinjam[i].status == MENUNGGU) {
                sesuaiFilter = true;
            }

            if (sesuaiFilter) {
                cout << left
                     << setw(5) << nomor
                     << setw(20) << daftarPinjam[i].namaKelas
                     << setw(25) << daftarPinjam[i].keperluan
                     << statusKeString(daftarPinjam[i].status)
                     << endl;

                nomor++;
                adaRiwayat = true;
            }
        }

        cout << string(65, '-') << endl;

        if (!adaRiwayat) {
            cout << "Tidak ada riwayat pada filter ini.\n";
        }

    } while (pilihan != 0);
}

// ==========================================
// 7. FITUR ADMIN: PERSETUJUAN PEMINJAMAN
// ==========================================

void permohonanPeminjaman() {
    cout << string(50, '=') << endl;
    cout << "DAFTAR PERMOHONAN PEMINJAMAN" << endl;
    cout << string(50, '=') << endl;

    // Menyimpan indeks permohonan yang menunggu
    vector<int> indeksMenunggu;
    bool adaData = false;

    for (size_t i = 0; i < daftarPinjam.size(); i++) {
        if (daftarPinjam[i].status == MENUNGGU) {
            indeksMenunggu.push_back(i);

            cout << indeksMenunggu.size()
                 << ". Peminjam: " << daftarPinjam[i].namaPeminjam
                 << " | Kelas: " << daftarPinjam[i].namaKelas
                 << " | Keperluan: " << daftarPinjam[i].keperluan
                 << " | Status: "
                 << statusKeString(daftarPinjam[i].status)
                 << endl;

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

    if (nomorPilih == 0) {
        return;
    }

    if (nomorPilih > indeksMenunggu.size()) {
        cout << "Nomor tidak valid!\n";
        return;
    }

    // Mengambil indeks asli data peminjaman
    size_t index = indeksMenunggu[nomorPilih - 1];

    int keputusan;

    cout << "\n1. Terima (ACC)\n";
    cout << "2. Tolak\n";
    cout << "Pilih: ";
    cin >> keputusan;

    if (keputusan == 1) {
        daftarPinjam[index].status = DISETUJUI;

        for (size_t j = 0; j < daftarKelas.size(); j++) {
            if (daftarKelas[j].namaKelas ==
                daftarPinjam[index].namaKelas) {

                daftarKelas[j].isKosong = false;
                break;
            }
        }

        cout << "Peminjaman berhasil disetujui!\n";
    }
    else if (keputusan == 2) {
        daftarPinjam[index].status = DITOLAK;

        cout << "Peminjaman berhasil ditolak.\n";
    }
    else {
        cout << "Pilihan tidak valid!\n";
    }
}

// ==========================================
// 8. LOGIN
// ==========================================

int login(string& userAktif) {
    string username, password;

    cout << "\n========== LOGIN ==========\n";
    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    for (size_t i = 0; i < daftarAkun.size(); i++) {
        if (daftarAkun[i].username == username &&
            daftarAkun[i].password == password) {

            userAktif = daftarAkun[i].username;

            if (daftarAkun[i].role == "user") {
                cout << "Login berhasil sebagai User!\n";
                return 1;
            }
            else if (daftarAkun[i].role == "admin") {
                cout << "Login berhasil sebagai Admin!\n";
                return 2;
            }
        }
    }

    cout << "Username atau password salah!\n";
    return 0;
}

// ==========================================
// 9. FITUR TAMBAH AKUN
// ==========================================

void tambahAkun() {
    string username, password;
    string role;
    string kodeAdmin;

    cout << "\n========== TAMBAH AKUN ==========\n";
    cout << "Username baru: ";
    cin >> username;

    // Memeriksa username yang sudah digunakan
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
    }
    else if (pilihan == 2) {
        cout << "Masukkan password khusus admin: ";
        cin >> kodeAdmin;

        if (kodeAdmin != PASSWORD_ADMIN) {
            cout << "Password khusus admin salah!\n";
            return;
        }

        role = "admin";
    }
    else {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    daftarAkun.push_back({username, password, role});

    cout << "\nAkun berhasil dibuat!\n";
    cout << "Username: " << username << "\n";
    cout << "Jenis akun: " << role << "\n";
}

// ==========================================
// 10. MENU USER
// ==========================================

void menuUser(const string& username) {
    int pilihan;

    do {
        cout << "\n========== MENU USER ==========\n";
        cout << "Login sebagai: " << username << "\n";
        cout << "1. Cek & Cari Ruang Tersedia\n";
        cout << "2. Ajukan Peminjaman Ruang\n";
        cout << "3. Riwayat Peminjaman\n";
        cout << "4. Logout\n";
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
                riwayatPeminjaman(username);
                break;

            case 4:
                cout << "Berhasil logout!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 4);
}

// ==========================================
// 11. MENU ADMIN
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
// 12. PROGRAM UTAMA
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
                string userAktif;
                int hasilLogin = login(userAktif);

                if (hasilLogin == 1) {
                    menuUser(userAktif);
                }
                else if (hasilLogin == 2) {
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
