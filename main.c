#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
    printf("3. Hapus Jurusan\n");
    printf("4. Edit Jurusan\n");
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
void unduhDataMahasiswa() {
    struct Mahasiswa *daftarMhs = readMahasiswaFile();
    struct Jurusan *daftarJur = readJurusanFile();
    if (!daftarMhs || !daftarJur) {
        printf("Data mahasiswa atau jurusan tidak ditemukan!\n");
        return;
    }

    int countMhs = 0, countJur = 0;
    while (daftarMhs[countMhs].nim[0] != '\0') countMhs++;
    while (daftarJur[countJur].kode_jurusan[0] != '\0') countJur++;

    int pilihan;
    printf("Pilih format unduh:\n1. TXT\n2. CSV\n");
    pilihan = inputAngka("Masukkan pilihan (1/2): ");

    // Hitung jumlah mahasiswa per jurusan
    int maxRows = 0;
    int jumlahPerJurusan[25] = {0};
    for (int j = 0; j < countJur; j++) {
        for (int i = 0; i < countMhs; i++) {
            if (strcmp(daftarMhs[i].nama_jurusan, daftarJur[j].nama_jurusan) == 0) {
                jumlahPerJurusan[j]++;
            }
        }
        if (jumlahPerJurusan[j] > maxRows)
            maxRows = jumlahPerJurusan[j];
    }

    if (pilihan == 1) { // TXT
        FILE *dst = fopen("hasil_mahasiswa.txt", "w");
        if (!dst) { printf("Gagal membuat file TXT!\n"); return; }

        // Header jurusan
        for (int j = 0; j < countJur; j++)
            fprintf(dst, "| %-30s ", daftarJur[j].nama_jurusan);
        fprintf(dst, "|\n");

        // Garis pemisah
        for (int j = 0; j < countJur; j++)
            fprintf(dst, "+--------------------------------");
        fprintf(dst, "+\n");

        // Isi mahasiswa per jurusan
        for (int row = 0; row < maxRows; row++) {
            for (int j = 0; j < countJur; j++) {
                int found = 0;
                int currentIndex = 0;
                for (int i = 0; i < countMhs; i++) {
                    if (strcmp(daftarMhs[i].nama_jurusan, daftarJur[j].nama_jurusan) == 0) {
                        if (currentIndex == row) {
                            char buf[35];
                            snprintf(buf, sizeof(buf), "%s - %s", daftarMhs[i].nim, daftarMhs[i].nama);
                            fprintf(dst, "| %-30s ", buf);
                            found = 1;
                            break;
                        }
                        currentIndex++;
                    }
                }
                if (!found)
                    fprintf(dst, "| %-30s ", "");
            }
            fprintf(dst, "|\n");
        }

        // Garis akhir
        for (int j = 0; j < countJur; j++)
            fprintf(dst, "+--------------------------------");
        fprintf(dst, "+\n");

        fclose(dst);
        printf("Data berhasil diunduh ke: hasil_mahasiswa.txt\n");

    } else if (pilihan == 2) { // CSV
        FILE *dst = fopen("mahasiswa.csv", "w");
        if (!dst) { printf("Gagal membuat file CSV!\n"); return; }

        // Header jurusan
        for (int j = 0; j < countJur; j++) {
            fprintf(dst, "%s", daftarJur[j].nama_jurusan);
            if (j != countJur - 1) fprintf(dst, ";");
        }
        fprintf(dst, "\n");

        // Isi per baris
        for (int row = 0; row < maxRows; row++) {
            for (int j = 0; j < countJur; j++) {
                int found = 0;
                int currentIndex = 0;
                for (int i = 0; i < countMhs; i++) {
                    if (strcmp(daftarMhs[i].nama_jurusan, daftarJur[j].nama_jurusan) == 0) {
                        if (currentIndex == row) {
                            fprintf(dst, "%s - %s", daftarMhs[i].nim, daftarMhs[i].nama);
                            found = 1;
                            break;
                        }
                        currentIndex++;
                    }
                }
                if (!found) fprintf(dst, "");
                if (j != countJur - 1) fprintf(dst, ";");
            }
            fprintf(dst, "\n");
        }

        fclose(dst);
        printf("Data berhasil diunduh ke: mahasiswa.csv\n");
    } else {
        printf("Pilihan tidak valid!\n");
    }

    printf("Tekan ENTER untuk kembali...");
    getchar();
}


void unduhDataJurusan() {
    int pilihanunduhjurusan;
    FILE *src;

    src = fopen("jurusan.txt", "r");
    if (!src) {
        printf("File jurusan.txt tidak ditemukan!\n");
        return;
    }

    printf("Pilih format unduh:\n");
    printf("1. TXT\n");
    printf("2. CSV\n");
    pilihanunduhjurusan = inputAngka("Masukkan pilihan (1/2): ");

    char line[200];

    if (pilihanunduhjurusan == 1) {
        FILE *dst = fopen("hasil_jurusan.txt", "w");
        if (!dst) { fclose(src); return; }

        fprintf(dst, "+------------+----------------------+\n");
        fprintf(dst, "|   Kode     |     Nama Jurusan     |\n");
        fprintf(dst, "+------------+----------------------+\n");

        while (fgets(line, sizeof(line), src)) { 
            char *kode = strtok(line, ";"); 
            char *nama = strtok(NULL, "\n"); 
            fprintf(dst, "| %-10s | %-20s |\n", kode, nama); }

        fprintf(dst, "+------------+----------------------+\n");
        fclose(dst);
        printf("Data jurusan berhasil diunduh ke: hasil_jurusan.txt\n");
        getchar();

    } else if (pilihanunduhjurusan == 2) {
        FILE *dst = fopen("jurusan.csv", "w");
        if (!dst) { fclose(src); return; }

        fprintf(dst, "Kode;Nama Jurusan\n");

        while (fgets(line, sizeof(line), src)) {
            char *kode = strtok(line, ";");
            char *nama = strtok(NULL, "\n");
            fprintf(dst, "%s;%s\n", kode, nama);
        }

        fclose(dst);
        printf("Data jurusan berhasil diunduh ke: jurusan.csv\n");
        getchar();

    } else {
        printf("Pilihan tidak valid!\n");
    }

    fclose(src);
}

void statistikJurusan() {
    FILE *src = fopen("mahasiswa.txt", "r");
    if (!src) {
        printf("File mahasiswa.txt tidak ditemukan!\n");
        return;
    }

    char line[200], nim[20], nama[50], jk[5], asal[50];
    int countTI = 0, countSI = 0, countDKV = 0, countPTI = 0;

    while (fgets(line, sizeof(line), src)) {
        if (sscanf(line, "%[^;];%[^;];%[^;];%[^\n]", nim, nama, jk, asal) == 4) {
            // ambil dua digit awal NIM
            char kode[3];
            strncpy(kode, nim, 2);
            kode[2] = '\0';

            // Hitung berdasarkan mapping
            if (strcmp(kode, "67") == 0) countTI++;
            else if (strcmp(kode, "68") == 0) countSI++;
            else if (strcmp(kode, "69") == 0) countDKV++;
            else if (strcmp(kode, "70") == 0) countPTI++;
        }
    }

    fclose(src);

    // Tampilkan tabel
    printf("+----------------------+--------+\n");
    printf("| Nama Jurusan         | Jumlah |\n");
    printf("+----------------------+--------+\n");
    printf("| Teknik Informatika   | %-6d |\n", countTI);
    printf("| Sistem Informasi     | %-6d |\n", countSI);
    printf("| DKV                  | %-6d |\n", countDKV);
    printf("| Pendidikan TI        | %-6d |\n", countPTI);
    printf("+----------------------+--------+\n\n");
    printf("Tekan ENTER untuk kembali...");
    getchar();
}


bool cariMahasiswaByNim(struct Mahasiswa daftar[], const char *nim) {
    for (int i = 0; i < 100; i++) {
        if (daftar[i].nim[0] == '\0')
            continue;

        if (strcmp(daftar[i].nim, nim) == 0) {
            return true;
        }
    }
    return false;
}

int cariIndexMahasiswaByNim(struct Mahasiswa daftar[], const char *nim) {
    for (int i = 0; i < 100; i++) {
        if (daftar[i].nim[0] == '\0')
            continue;

        if (strcmp(daftar[i].nim, nim) == 0) {
            return i;
        }
    }
    return -1;
}

bool cariJurusanByKode(struct Jurusan daftar[], const char *kode){
    for(int i=0;i<25;i++){
        if (daftar[i].kode_jurusan[0] == '\0')
            continue;
        if(strcmp(daftar[i].kode_jurusan, kode)==0){
            return true;
        }
    }
    return false;
}

int cariIndexJurusanByKode(struct Jurusan daftar[], const char *kode){
    for(int i=0;i<25;i++){
        if (daftar[i].kode_jurusan[0] == '\0')
            continue;
        if(strcmp(daftar[i].kode_jurusan, kode)==0){
            return i;
        }
    }
    return -1;
}

int main() {
    int pilihanUtama, pilihanMahasiswa, pilihanJurusan, pilihanreporting;

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
                            printf("|\t\t\t\t\t\tDaftar Mahasiswa\t\t\t\t\t|\n");
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
                                    if (data[i].nama[0] == '\0') {
                                        break;
                                    } else {
                                        printf("| %-3d | %-10s | %-20s | %-20s | %-13c | %-20s |\n",
                                               i + 1,
                                               data[i].nim,
                                               data[i].nama_jurusan,
                                               data[i].nama,
                                               data[i].jenis_kelamin,
                                               data[i].alamat);
                                    }
                                }
                            }
                            system("pause");
                            break;


                        case 2: // Tambah Mahasiswa
                            {
                                system("cls");
                                int pilihan_input_jurusan;
                                int input_nim;
                                struct Mahasiswa mhs;
                                printf("=== Tambah Data Mahasiswa ===\n");
                                printf("Pilihan jurusan : \n");
                                dataJurusan = readJurusanFile();
                                data = readMahasiswaFile();
                                if (dataJurusan == NULL) {
                                    printf("Daftar jurusan kosong atau file tidak ditemukan.\n");
                                    break;
                                }
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

                                char nim_str[10];
                                char nim_cari[10];
                                bool hasil_cek = false;

                                printf("Pilih jurusan : ");
                                pilihan_input_jurusan = inputAngka("");
                                do{
                                    printf("Masukkan NIM : %s", dataJurusan[pilihan_input_jurusan-1].kode_jurusan);
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    // gabungkan kode jurusan + nim ke nim_cari (tanpa mengubah kode_jurusan)
                                    sprintf(nim_cari, "%s%d", dataJurusan[pilihan_input_jurusan-1].kode_jurusan, input_nim);
                                    hasil_cek = cariMahasiswaByNim(data, nim_cari);

                                    if(hasil_cek==1){
                                        printf("Maaf NIM sudah terpakai, input ulang NIM! \n");
                                    }

                                }while(hasil_cek == 1);

                                // tambahkan nim dengan kode jurusan
                                sprintf(mhs.nim, "%s%d", dataJurusan[pilihan_input_jurusan-1].kode_jurusan, input_nim);

                                // input data nama dst
                                inputTeks("Masukkan nama : ", mhs.nama);
                                char jk;
                                do{
                                    printf("Jenis kelamin: (L/P) : ");
                                    scanf(" %c", &jk);
                                    jk = toupper(jk);
                                }while(jk != 'L' && jk != 'P');
                                mhs.jenis_kelamin = jk;                                
                                while (getchar() != '\n'); 
                                inputTeks("Masukkan alamat: ", mhs.alamat);

                                //append ke mahasiswa.txt

                                FILE *fp = fopen(FILE_NAME, "a");
                                fprintf(fp, "%s;%s;%c;%s\n", mhs.nim, mhs.nama, mhs.jenis_kelamin, mhs.alamat);
                                fclose(fp);

                                printf("\n\nData berhasil ditambahkan!\n");
                                system("pause");
                                break;
                            }
                        case 3:
                            {
                                // Hapus Mahasiswa
                                /*
                                   1. Baca file data mahasiswa
                                   2. Cari mahasiswa yg ingin dihapus
                                   3. Simpan index/urutan mahasiswa yg ingin dihapus
                                   4. Tulis ulang file tanpa urutan mahasiswa yg dihapus
                                */
                                system("cls");
                                printf("=== Hapus Data Mahasiswa ===\n");
                                data = readMahasiswaFile();
                                int input_nim;
                                char nim_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan NIM : ");
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    if(input_nim == 0){
                                        break;
                                    }
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexMahasiswaByNim(data, nim_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan/nim tidak ditemukan
                                        printf("Maaf NIM tidak ditemukan, input ulang NIM (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        // hasil_cek = index mahasiswa yg ingin dihapus
                                        printf("Data Mahasiswa : \n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        printf("Nama          : %s\n", data[hasil_cek].nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : %c\n", data[hasil_cek].jenis_kelamin);
                                        printf("Alamat        : %s\n\n", data[hasil_cek].alamat);
                                        printf("Yakin ingin menghapus data mahasiswa? (y/n)");
                                        char pilihan;
                                        scanf("%c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (data[i].nim[0] == '\0') {
                                                    break;
                                                }

                                                // Skip data yang akan dihapus (hasil_cek = index yang ditemukan)
                                                if (i == hasil_cek) {
                                                    continue;
                                                }else{
                                                    // Tulis data lain ke file
                                                    fprintf(fp, "%s;%s;%c;%s\n",
                                                            data[i].nim,
                                                            data[i].nama,
                                                            data[i].jenis_kelamin,
                                                            data[i].alamat);
                                                }

                                            }
                                            fclose(fp);
                                            printf("Data Mahasiswa berhasil dihapus! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;

                                        }
                                    }
                                }while(hasil_cek == -1);

                                break;
                            }

                        case 4:
                                // Edit Mahasiswa
                                /*
                                   1. Baca file data mahasiswa
                                   2. Cari mahasiswa yg ingin diedit
                                   3. Simpan index/urutan mahasiswa yg ingin diedit
                                   4. Input data baru (NIM tidak boleh diedit)
                                   5. Tulis ulang file tetapi index yg ingin diedit, tulis data yg baru
                                */
                                system("cls");
                                printf("=== Edit Data Mahasiswa ===\n");
                                data = readMahasiswaFile();
                                int input_nim;
                                char nim_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan NIM : ");
                                    // input nim (angka)
                                    input_nim = inputAngka("");
                                    if(input_nim == 0){
                                        break;
                                    }
                                    // ubah nim ke char[10]
                                    sprintf(nim_str, "%d", input_nim);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexMahasiswaByNim(data, nim_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan/nim tidak ditemukan
                                        printf("Maaf NIM tidak ditemukan, input ulang NIM (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        char input_nama[49];
                                        char input_alamat[49];
                                        // hasil_cek = index mahasiswa yg ingin diubah
                                        printf("Data Mahasiswa Sebelumnya: \n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        printf("Nama          : %s\n", data[hasil_cek].nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : %c\n", data[hasil_cek].jenis_kelamin);
                                        printf("Alamat        : %s\n\n", data[hasil_cek].alamat);
                                        printf("-----------------------------------------------\n\n");
                                        printf("Perubahan Data Mahasiswa :\n");
                                        printf("NIM           : %s\n", data[hasil_cek].nim);
                                        inputTeks("Nama          : ", input_nama);
                                        strcpy(data[hasil_cek].nama, input_nama);
                                        printf("Jurusan       : %s\n", data[hasil_cek].nama_jurusan);
                                        printf("Jenis Kelamin : ");
                                        scanf(" %c",&data[hasil_cek].jenis_kelamin);
                                        inputTeks("Alamat        : ", input_alamat);
                                        strcpy(data[hasil_cek].alamat, input_alamat);
                                        printf("\nYakin ingin mengubah data mahasiswa? (y/n)");
                                        char pilihan;
                                        scanf(" %c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (data[i].nim[0] == '\0') {
                                                    break;
                                                }

                                                // Tulis ulang ke file
                                                fprintf(fp, "%s;%s;%c;%s\n",
                                                            data[i].nim,
                                                            data[i].nama,
                                                            data[i].jenis_kelamin,
                                                            data[i].alamat);


                                            }
                                            fclose(fp);
                                            printf("Data Mahasiswa berhasil diubah! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;
                                        }
                                    }
                                }while(hasil_cek == -1);
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
                            {
                                system("cls");
                                printf("============================================\n");
                                printf("|             Daftar Jurusan               |\n");
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
                                        if (dataJurusan[i].nama_jurusan[0] == '\0') {
                                            break;
                                        } else {
                                            printf("| %-3d | %-12s | %-19s |\n",
                                                   i + 1,
                                                   dataJurusan[i].kode_jurusan,
                                                   dataJurusan[i].nama_jurusan);
                                        }
                                    }
                                    printf("--------------------------------------------\n");
                                }
                                system("pause");
                                break;
                            }

                        case 2: // Tambah Jurusan
                            {
                                system("cls");
                                struct Jurusan jrs;
                                printf("=== Tambah Jurusan ===\n");
                                dataJurusan = readJurusanFile();
                                bool hasil_cek = false;
                                int input_kode;
                                char cari_kode[3];
                                do{
                                    printf("Masukkan kode jurusan : ");
                                    input_kode = inputAngka("");
                                    sprintf(cari_kode, "%d", input_kode);

                                    hasil_cek = cariJurusanByKode(dataJurusan, cari_kode);
                                    if(hasil_cek==1){
                                        printf("Maaf kode jurusan sudah digunakan, input ulang kode! \n");
                                    }else{
                                        //masukkan data kode
                                        strcpy(jrs.kode_jurusan, cari_kode);
                                        inputTeks("Masukkan nama jurusan : ", jrs.nama_jurusan);
                                        FILE *fp = fopen(FILE_JURUSAN_NAME, "a");
                                        fprintf(fp, "%s;%s\n", jrs.kode_jurusan,jrs.nama_jurusan);
                                        fclose(fp);
                                        printf("Jurusan berhasil ditambahkan! \n\n");
                                        system("pause");
                                    }
                                }while(hasil_cek == 1);
                            }
                            break;

                        case 3: // Hapus Jurusan
                            {
                                system("cls");
                                printf("=== Hapus Data Jurusan ===\n");
                                dataJurusan = readJurusanFile();
                                int input_kode;
                                char kode_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan Kode : ");
                                    input_kode = inputAngka("");
                                    if(input_kode == 0){
                                        break;
                                    }
                                    sprintf(kode_str, "%d", input_kode);

                                    //cek kode apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexJurusanByKode(dataJurusan, kode_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan tidak ditemukan
                                        printf("Maaf Kode tidak ditemukan, input ulang Kode (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        // hasil_cek = index jurusan yg ingin dihapus
                                        printf("Data Jurusan : \n");
                                        printf("Kode Jurusan  : %s\n", dataJurusan[hasil_cek].kode_jurusan);
                                        printf("Nama Jurusan  : %s\n", dataJurusan[hasil_cek].nama_jurusan);
                                        printf("Yakin ingin menghapus data jurusan? (y/n)");
                                        char pilihan;
                                        scanf("%c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_JURUSAN_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (dataJurusan[i].kode_jurusan[0] == '\0') {
                                                    break;
                                                }

                                                // Skip data yang akan dihapus (hasil_cek = index yang ditemukan)
                                                if (i == hasil_cek) {
                                                    continue;
                                                }else{
                                                    // Tulis data lain ke file
                                                    fprintf(fp, "%s;%s\n",
                                                            dataJurusan[i].kode_jurusan,
                                                            dataJurusan[i].nama_jurusan);
                                                }

                                            }
                                            fclose(fp);
                                            printf("Data Jurusan berhasil dihapus! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;

                                        }
                                    }
                                }while(hasil_cek == -1);
                                break;
                            }
                        case 4: // Edit Jurusan
                            {
                                system("cls");
                                printf("=== Edit Data Jurusan ===\n");
                                dataJurusan = readJurusanFile();
                                int input_kode;
                                char kode_str[10];
                                int hasil_cek = -1;
                                do{
                                    printf("Masukkan Kode Jurusan : ");
                                    // input nim (angka)
                                    input_kode = inputAngka("");
                                    if(input_kode == 0){
                                        break;
                                    }
                                    // ubah nim ke char[10]
                                    sprintf(kode_str, "%d", input_kode);

                                    //cek nim apakah sudah pernah diinput
                                    //1 = found (ketemu)
                                    //0 = not found (tidak ketemu)

                                    hasil_cek = cariIndexJurusanByKode(dataJurusan, kode_str);

                                    if(hasil_cek==-1)
                                    {
                                        // index tidak ditemukan/nim tidak ditemukan
                                        printf("Maaf Kode tidak ditemukan, input ulang Kode (input 0 untuk kembali)! \n");
                                    }else
                                    {
                                        char nama_jurusan[26];
                                        // hasil_cek = index mahasiswa yg ingin diubah
                                        printf("Data Jurusan Sebelumnya: \n");
                                        printf("Kode Jurusan     : %s\n", dataJurusan[hasil_cek].kode_jurusan);
                                        printf("Nama Jurusan     : %s\n", dataJurusan[hasil_cek].nama_jurusan);
                                        printf("-----------------------------------------------\n\n");
                                        printf("Perubahan Data Jurusan :\n");
                                        printf("Kode Jurusan     : %s\n", dataJurusan[hasil_cek].kode_jurusan);
                                        inputTeks("Nama Jurusan     : ", nama_jurusan);
                                        strcpy(dataJurusan[hasil_cek].nama_jurusan, nama_jurusan);
                                        printf("\nYakin ingin mengubah data jurusan? (y/n)");
                                        char pilihan;
                                        scanf(" %c", &pilihan);
                                        if(pilihan == 'y' || pilihan == 'Y'){
                                            FILE *fp = fopen(FILE_JURUSAN_NAME, "w");
                                            for (int i = 0; i < 100; i++) {
                                                // Berhenti jika data sudah habis
                                                if (dataJurusan[i].kode_jurusan[0] == '\0') {
                                                    break;
                                                }

                                                // Tulis ulang ke file
                                                fprintf(fp, "%s;%s\n",
                                                            dataJurusan[i].kode_jurusan,
                                                            dataJurusan[i].nama_jurusan);


                                            }
                                            fclose(fp);
                                            printf("Data Jurusan berhasil diubah! \n\n");
                                            system("pause");
                                        }else{
                                            system("pause");
                                            break;
                                        }
                                    }
                                }while(hasil_cek == -1);
                                break;
                            }

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
            do {
                    tampilkanMenuReporting();
                    pilihanreporting = inputAngka("");

                    switch (pilihanreporting) {
                        case 1:
                        system("cls");
                        statistikJurusan();
                            break;
                        case 2:
                        system("cls");
                        unduhDataMahasiswa();
                        break;
                        case 3:
                        system("cls");
                        unduhDataJurusan();
                        break;
                        case 4:
                            printf("Kembali ke menu utama...\n");
                            break;
                    default:
                        printf("Opsi tidak valid.\n");
                        break;
                    }

                } while (pilihanreporting != 4);
                break;
            
            case 4:
                printf("Keluar dari program...\n");
                break;

            default:
                printf("Opsi tidak valid.\n2");
                break;
        }
    }while(pilihanUtama != 4);

    return 0;
}
