#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
                                printf("---------------------------------------------------------------------------------------------------------\n");
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
                                strcpy(mhs.nim, (strcat(dataJurusan[pilihan_input_jurusan-1].kode_jurusan, nim_str)));

                                // input data nama dst
                                inputTeks("Masukkan nama : ", mhs.nama);
                                printf("Masukkan jenis kelamin : ");
                                scanf(" %c",&mhs.jenis_kelamin);
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
                              {

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
                              }
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
                                        fprintf(fp, "\n%s;%s\n", jrs.kode_jurusan,jrs.nama_jurusan);
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
                printf("Menu Report masih belom dibuat.\n");
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
