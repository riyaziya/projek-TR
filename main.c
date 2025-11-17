#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "mahasiswa.txt"
#define FILE_JURUSAN_NAME "jurusan.txt"

struct Mahasiswa {
    char nama[50];
    char nim[10];
    char jenis_kelamin;
    char alamat[50];
    char nama_jurusan[26];
};

struct Jurusan {
    char kode_jurusan[3];
    char nama_jurusan[26];
};

// === Validasi Huruf ===
int validHuruf(char teks[]) {
    for (int i = 0; teks[i]; i++) {
        char c = teks[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' '))
            return 0;
    }
    return 1;
}

// === Input Aman Angka ===
int inputAngka(const char *label) {
    int val;
    while (1) {
        printf("%s", label);
        if (scanf("%d", &val) == 1) {
            while (getchar() != '\n');
            return val;
        } else {
            printf("Input salah! Harus angka.\n");
            while (getchar() != '\n');
        }
    }
}

// === Input Aman Teks ===
void inputTeks(const char *label, char *tujuan) {
    do {
        printf("%s", label);
        scanf(" %[^\n]", tujuan);
        if (!validHuruf(tujuan))
            printf("Input hanya boleh huruf dan spasi!\n");
    } while (!validHuruf(tujuan));
}

char* cariJurusan(struct Jurusan daftar[], const char *nim){
    static char hasil[26];
    char kode_jurusan_mhs[3];
    strncpy(kode_jurusan_mhs,nim+0, 2);
    kode_jurusan_mhs[2] = '\0';
    for(int i=0;i<10;i++){
        if(strcmp(daftar[i].kode_jurusan, kode_jurusan_mhs)==0){
            return daftar[i].nama_jurusan;
        }
    }
    return "";
}

// === Membaca Jurusan.txt return Jurusan[] ===
struct Jurusan* readJurusanFile(){
    FILE *fp = fopen(FILE_JURUSAN_NAME, "r");
    struct Jurusan j;
    static struct Jurusan daftar[10];

    if(fp == NULL){
        printf("Belum ada data jurusan tersimpan! \n");
        return NULL;
    }

    int num = 0;
    while (fscanf(fp, " %2[^;];%25[^\n]",
                  j.kode_jurusan, j.nama_jurusan) == 2){
        daftar[num++] = j;
    }
    fclose(fp);
    if(num != 0){
        daftar[num].nama_jurusan[0] = '\0';
        return daftar;
    }else{
        printf("Belum ada data jurusan tersimpan!\n");
        return NULL;
    }
};

// === Membaca Mahasiswa.txt (return mahasiwa[])===
struct Mahasiswa* readMahasiswaFile(){
    FILE *fp = fopen(FILE_NAME, "r");
    struct Mahasiswa m;
    static struct Mahasiswa daftar[100];
    struct Jurusan *daftarJurusan;

    if(fp == NULL){
        printf("Belum ada data mahasiswa tersimpan! \n");
        return NULL;
    }

    int num = 0;
    while (fscanf(fp, " %9[^;];%49[^;];%c;%49[^\n]",
                  m.nim, m.nama, &m.jenis_kelamin, m.alamat) == 4){
        strcpy(m.nama_jurusan, "");
        daftar[num++] = m;
    }
    fclose(fp);

    daftarJurusan = readJurusanFile();
    if(daftarJurusan != NULL){
        for(int i=0;i<num;i++){
            strcpy(daftar[i].nama_jurusan, cariJurusan(daftarJurusan, daftar[i].nim));
        }
    }
    if(num != 0){
        daftar[num].nama[0] = '\0';
        return daftar;
    }else{
        printf("Belum ada data mahasiswa tersimpan!\n");
        return NULL;
    }
}

void tampilkanMenu() {
    system("cls");
    printf("\n=== Menu Manajemen Data ===\n");
    printf("1. Master Data Mahasiswa\n");
    printf("2. Master Data Jurusan\n");
    printf("3. Reporting\n");
    printf("4. Keluar\n");
    printf("Pilih opsi (1-4): ");
}

void tampilkanMenuMahasiswa() {
    system("cls");
    printf("\n=== Master Data Mahasiswa ===\n");
    printf("1. Daftar Mahasiswa\n");
    printf("2. Tambah Mahasiswa\n");
    printf("3. Hapus Mahasiswa\n");
    printf("4. Edit Mahasiswa\n");
    printf("5. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}

void tampilkanMenuJurusan() {
    system("cls");
    printf("\n=== Master Data Jurusan ===\n");
    printf("1. Daftar Jurusan\n");
    printf("2. Tambah Jurusan\n");
    printf("3. Edit Jurusan\n");
    printf("4. Hapus Jurusan\n");
    printf("5. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}

void tampilkanMenuReporting() {
    system("cls");
    printf("\n=== Reporting ===\n");
    printf("1. Statistik\n");
    printf("2. Unduh Data Mahasiswa\n");
    printf("3. Unduh Data Jurusan\n");
    printf("4. Kembali ke Menu Utama\n");
    printf("Pilih: ");
}


int main() {
    int pilihanUtama, pilihanMahasiswa, pilihanJurusan;

    struct Mahasiswa *data;
    int jumlahMahasiswa = 0;

    struct Jurusan *dataJurusan;
    int jumlahJurusan = 0;

    do {
        tampilkanMenu();
        pilihanUtama = inputAngka("");
        switch (pilihanUtama) {
              case 1: // MASTER DATA MAHASISWA
                do {
                    tampilkanMenuMahasiswa();
                    pilihanMahasiswa = inputAngka("");

                    switch (pilihanMahasiswa) {
                        case 1: // Daftar Mahasiswa
                            system("cls");
                            printf("=========================================================================================================\n");
                            printf("|\t\t\t\t\t\tDaftar Mahasiswa\t\t\t\t\        |\n");
                            printf("=========================================================================================================\n");
                            data = readMahasiswaFile();
                            if (data == NULL) {
                                printf("Daftar mahasiswa kosong atau file tidak ditemukan.\n");
                                break;
                            }
                            if (data[0].nim == NULL){
                                printf("Daftar mahasiswa kosong.\n");
                            }else{
                                printf("| %-3s | %-10s | %-20s | %-20s | %-13s | %-20s |\n",
                                       "No", "NIM", "Jurusan", "Nama", "Jenis Kelamin", "Alamat");
                                printf("---------------------------------------------------------------------------------------------------------\n");
                                for (int i = 0; i < 100; i++) {
                                    if (data[i].nim[0] != '\0') {
                                        printf("| %-3d | %-10s | %-20s | %-20s | %-13c | %-20s |\n",
                                               i + 1,
                                               data[i].nim,
                                               data[i].nama_jurusan,
                                               data[i].nama,
                                               data[i].jenis_kelamin,
                                               data[i].alamat);
                                    } else {
                                        break;
                                    }
                                }
                                printf("---------------------------------------------------------------------------------------------------------\n");
                            }
                            system("pause");
                            break;


                        case 2: // Tambah Mahasiswa
                           system("cls");
                           int pilihan_input_jurusan;
                           int input_nim;
                           struct Mahasiswa mhs;
                           printf("=== Tambah Data Mahasiswa ===");
                           printf("Pilihan jurusan:\n");
                           dataJurusan=readJurusanFile();
                           if (dataJurusan[0].kode_jurusan == NULL){
                                printf("Daftar Jurusan kosong.\n");
                            }else{
                                for (int i = 0; i < 100; i++) {
                                    if (dataJurusan[i].kode_jurusan[0] != '\0') {
                                        printf("%d. (%s) %s\n",
                                               i + 1,
                                               dataJurusan[i].kode_jurusan,
                                               dataJurusan[i].nama_jurusan);
                                    } else {
                                        break;
                                    }
                                }
                            }
                            printf("Pilih jurusan:");
                            pilihan_input_jurusan=inputAngka("");
                            printf("Masukkkan NIM: %s", dataJurusan[pilihan_input_jurusan-1].kode_jurusan);
                            input_nim=inputAngka("");
                            char nim_str[10];
                            sprintf(nim_str, "%d", input_nim);
                            strcpy(mhs.nim,(strcat(dataJurusan[pilihan_input_jurusan-1].kode_jurusan, nim_str)));
                            inputTeks("Masukkan nama:", mhs.nama);



                            printf("Data berhasil ditambahkan!\n");
                            system("pause");
                            break;

                        case 3: { // Hapus Mahasiswa
                            /*
                            if (jumlahMahasiswa == 0) { printf("Data kosong.\n"); break; }
                            int nimHapus = inputAngka("Masukkan NIM yang akan dihapus: ");
                            int idx = -1;
                            for (int i = 0; i < jumlahMahasiswa; i++)
                                if (data[i].nim == nimHapus) { idx = i; break; }

                            if (idx == -1)
                                printf("Mahasiswa tidak ditemukan.\n");
                            else {
                                char konfirmasi;
                                printf("Hapus %s (NIM %d)? (Y/N): ", data[idx].nama, nimHapus);
                                scanf(" %c", &konfirmasi);
                                if (konfirmasi=='Y'||konfirmasi=='y') {
                                    for (int i = idx; i < jumlahMahasiswa-1; i++)
                                        data[i] = data[i+1];
                                    jumlahMahasiswa--;
                                    printf("Data berhasil dihapus.\n");
                                } else printf("Penghapusan dibatalkan.\n");
                            }
                            */
                            break;
                        }

                        case 4: // Edit Mahasiswa
                            /*
                            if (jumlahMahasiswa == 0) { printf("Data kosong.\n"); break; }
                            int nimEdit, idxEdit=-1;
                            nimEdit = inputAngka("Masukkan NIM yang akan diedit: ");
                            for (int i=0;i<jumlahMahasiswa;i++)
                                if (data[i].nim==nimEdit) { idxEdit=i; break; }
                            if (idxEdit==-1) printf("Mahasiswa tidak ditemukan.\n");
                            else {
                                inputTeks("Masukkan Nama baru: ", data[idxEdit].nama);
                                inputTeks("Masukkan Jurusan baru: ", data[idxEdit].jurusan);
                                printf("Data berhasil diupdate!\n");
                            }
                            */
                            break;

                        case 5:
                            printf("Kembali ke menu utama...\n");
                            break;
                        default:
                            printf("Opsi tidak valid.\n");
                            break;
                    }
                } while (pilihanMahasiswa != 5);
                break;

            case 2: // MASTER DATA JURUSAN
                do {
                    tampilkanMenuJurusan();
                    pilihanJurusan = inputAngka("");

                    switch (pilihanJurusan) {
                          case 1: // Daftar Jurusan
                            system("cls");
                            printf("============================================\n");
                            printf("|              Daftar Jurusan              |\n");
                            printf("============================================\n");
                            dataJurusan = readJurusanFile();
                            if (dataJurusan == NULL) {
                                printf("Daftar jurusan kosong atau file tidak ditemukan.\n");
                                break;
                            }
                            if (dataJurusan[0].kode_jurusan == NULL){
                                printf("Daftar Jurusan kosong.\n");
                            }else{
                                printf("| %-3s | %-10s | %-19s |\n",
                                       "No", "Kode Jurusan", "Nama Jurusan");
                                printf("--------------------------------------------\n");
                                for (int i = 0; i < 100; i++) {
                                    if (dataJurusan[i].kode_jurusan[0] != '\0') {
                                        printf("| %-3d | %-12s | %-19s |\n",
                                               i + 1,
                                               dataJurusan[i].kode_jurusan,
                                               dataJurusan[i].nama_jurusan);
                                    } else {
                                        break;
                                    }
                                }
                                printf("--------------------------------------------\n");
                            }
                            system("pause");
                            break;


                        case 2: // Tambah Jurusan
                            /*
                             inputTeks("Masukkan Kode Jurusan: ", jurusan[jumlahJurusan].kode);
                            inputTeks("Masukkan Nama Jurusan: ", jurusan[jumlahJurusan].nama);
                            jumlahJurusan++;
                            printf("Data jurusan berhasil ditambahkan!\n");
                            break;



                            */
                            break;

                        case 3: // Edit Jurusan

                           /* if (jumlahJurusan==0) { printf("Data kosong.\n"); break; }
                            char kodeEdit[10];
                            inputTeks("Masukkan Kode Jurusan yang akan diedit: ", kodeEdit);
                            int idxJur=-1;
                            for (int i=0;i<jumlahJurusan;i++)
                                if (strcmp(jurusan[i].kode,kodeEdit)==0) { idxJur=i; break; }
                            if (idxJur==-1) printf("Jurusan tidak ditemukan.\n");
                            else {
                                inputTeks("Masukkan Kode baru: ", jurusan[idxJur].kode);
                                inputTeks("Masukkan Nama baru: ", jurusan[idxJur].nama);
                                printf("Data berhasil diupdate!\n");
                            }*/

                            break;

                        case 4: // Hapus Jurusan

                           /* if (jumlahJurusan==0) { printf("Data kosong.\n"); break; }
                            char kodeHapus[10];
                            inputTeks("Masukkan Kode Jurusan yang akan dihapus: ", kodeHapus);
                            int idxHapus=-1;
                            for (int i=0;i<jumlahJurusan;i++)
                                if (strcmp(jurusan[i].kode,kodeHapus)==0) { idxHapus=i; break; }
                            if (idxHapus==-1) printf("Jurusan tidak ditemukan.\n");
                            else {
                                char konfirmasi;
                                printf("Hapus jurusan %s (Kode %s)? (Y/N): ", jurusan[idxHapus].nama, kodeHapus);
                                scanf(" %c",&konfirmasi);
                                if (konfirmasi=='Y'||konfirmasi=='y') {
                                    for (int i=idxHapus;i<jumlahJurusan-1;i++)
                                        jurusan[i]=jurusan[i+1];
                                    jumlahJurusan--;
                                    printf("Data berhasil dihapus.\n");
                                } else printf("Penghapusan dibatalkan.\n");
                            }*/

                            break;

                        case 5:
                            printf("Kembali ke menu utama...\n");
                            break;

                        default:
                            printf("Opsi tidak valid.\n");
                            break;
                    }

                } while (pilihanJurusan != 5);
                break;

           case 3://REPORTING
                printf("Menu Report masih belom dibuat.\n");


                break;

            case 4:
                printf("Keluar dari program...\n");
                break;

            default:
                printf("Opsi tidak valid.\n2");
                break;
        }

    } while (pilihanUtama != 4);

    return 0;
}
