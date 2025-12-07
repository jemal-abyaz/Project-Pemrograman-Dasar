
//==========================================
// PROGRAM TOKO ONLINE BUKAN ORANG JAWA //
//==========================================

#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ==================================
// CLASS PRODUCT
// ==================================
class Product {
public:
    int id;
    string name;
    double price;
    int stock;

    Product(int id, string name, double price, int stock)
        : id(id), name(name), price(price), stock(stock) {}

    void display() {
        cout << id << ". " << name
             << " | Rp " << price
             << " | Stok: " << stock << endl;
    }

    void reduceStock(int qty) {
        stock -= qty;
    }
};

// ==================================
// CLASS STORE
// ==================================
class Store {
private:
    vector<Product> products;

public:

    // Tambah produk (single + multiple)
    void addProducts() {
        int jumlah;
        cout << "Ingin menambah berapa produk? ";
        cin >> jumlah;

        for (int i = 0; i < jumlah; i++) {
            int id, stock;
            string name;
            double price;

            cout << "\n=== Input Produk ke-" << (i + 1) << " ===\n";

            cout << "ID Produk: ";
            cin >> id;

            cout << "Nama Produk (tanpa spasi): ";
            cin >> name;

            cout << "Harga: ";
            cin >> price;

            cout << "Stok: ";
            cin >> stock;

            products.push_back(Product(id, name, price, stock));
        }

        cout << "\nProduk berhasil ditambahkan!\n";
    }

    // Tampilkan semua produk
    void showProducts() {
        cout << "\n=== Daftar Produk ===\n";
        if (products.empty()) {
            cout << "Belum ada produk.\n\n";
            return;
        }

        for (auto &p : products)
            p.display();

        cout << endl;
    }

    // Edit produk
    void editProduct() {
        if (products.empty()) {
            cout << "Belum ada produk.\n";
            return;
        }

        int id;
        cout << "Masukkan ID produk yang ingin diedit: ";
        cin >> id;

        for (auto &p : products) {
            if (p.id == id) {
                cout << "\n=== Edit Produk ===\n";
                cout << "Nama lama : " << p.name << endl;
                cout << "Harga lama: " << p.price << endl;
                cout << "Stok lama : " << p.stock << endl;

                string newName;
                double newPrice;
                int newStock;

                cout << "Nama baru (tanpa spasi): ";
                cin >> newName;

                cout << "Harga baru: ";
                cin >> newPrice;

                cout << "Stok baru: ";
                cin >> newStock;

                p.name = newName;
                p.price = newPrice;
                p.stock = newStock;

                cout << "Produk berhasil diperbarui!\n";
                return;
            }
        }

        cout << "Produk dengan ID tersebut tidak ditemukan.\n";
    }

    // Hapus produk (single + multiple)
    void deleteProducts() {
        if (products.empty()) {
            cout << "Belum ada produk.\n";
            return;
        }

        showProducts();

        vector<int> ids;
        int id;

        cout << "Masukkan ID yang ingin dihapus (pisahkan spasi, akhiri 0): ";

        while (cin >> id && id != 0) {
            ids.push_back(id);
        }

        clearInput();

        cout << "\n=== PROSES HAPUS PRODUK ===\n";

        for (int pid : ids) {
            bool found = false;

            for (int i = 0; i < products.size(); i++) {
                if (products[i].id == pid) {
                    cout << "Produk " << products[i].name 
                         << " (ID " << pid << ") dihapus.\n";
                    products.erase(products.begin() + i);
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Produk dengan ID " << pid << " tidak ditemukan.\n";
            }
        }

        cout << "\nPenghapusan selesai.\n";
    }

    // Beli produk (single + multiple)
    void buyProducts() {
        if (products.empty()) {
            cout << "Belum ada produk.\n";
            return;
        }

        showProducts();

        vector<int> ids;
        int id;

        cout << "Masukkan ID produk yang ingin dibeli (pisahkan spasi, akhiri 0): ";

        while (cin >> id && id != 0) {
            ids.push_back(id);
        }

        clearInput();

        double grandTotal = 0;

        cout << "\n=== PROSES PEMBELIAN ===\n";

        for (int pid : ids) {
            bool found = false;

            for (auto &p : products) {
                if (p.id == pid) {
                    found = true;

                    int qty;
                    cout << "Jumlah untuk " << p.name << ": ";
                    cin >> qty;

                    if (p.stock >= qty) {
                        double subtotal = p.price * qty;
                        p.reduceStock(qty);

                        grandTotal += subtotal;

                        cout << "Subtotal " << p.name 
                             << ": Rp " << subtotal << endl;
                    } else {
                        cout << "Stok tidak cukup untuk " << p.name << endl;
                    }

                    break;
                }
            }

            if (!found) {
                cout << "Produk dengan ID " << pid << " tidak ditemukan.\n";
            }
        }

        cout << "\n=== TOTAL BELANJA ===\n";
        cout << "Total: Rp " << grandTotal << endl;
    }

    // Load dari file
    void loadFromFile() {
        ifstream file("input.txt");
        if (!file.is_open()) {
            cout << "File input.txt tidak ditemukan. Mulai dengan data kosong.\n";
            return;
        }

        int id, stock;
        double price;
        string name;

        while (file >> id >> name >> price >> stock) {
            products.push_back(Product(id, name, price, stock));
        }

        file.close();
        cout << "Data berhasil dimuat dari input.txt\n";
    }

    // Save ke file
    void saveToFile() {
        ofstream file("output.txt");

        for (auto &p : products) {
            file << p.id << " "
                 << p.name << " "
                 << p.price << " "
                 << p.stock << endl;
        }

        file.close();
        cout << "Data berhasil disimpan ke output.txt\n";
    }
};

// ==================================
// MENU ADMIN
// ==================================
void menuAdmin(Store &toko) {
    int pilih;
    do {
        cout << "\n=== MENU ADMIN ===\n";
        cout << "1. Tambah Produk\n";
        cout << "2. Tampilkan Produk\n";
        cout << "3. Edit Produk\n";
        cout << "4. Hapus Produk\n";
        cout << "5. Simpan ke File\n";
        cout << "0. Logout\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch(pilih) {
            case 1: toko.addProducts(); break;
            case 2: toko.showProducts(); break;
            case 3: toko.editProduct(); break;
            case 4: toko.deleteProducts(); break;
            case 5: toko.saveToFile(); break;
            case 0: cout << "Logout...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilih != 0);
}

// ==================================
// MENU KONSUMEN
// ==================================
void menuCustomer(Store &toko) {
    int pilih;

    do {
        cout << "\n=== MENU KONSUMEN ===\n";
        cout << "1. Lihat Produk\n";
        cout << "2. Beli Produk\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch(pilih) {
            case 1: toko.showProducts(); break;
            case 2: toko.buyProducts(); break;
            case 0: cout << "Kembali...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilih != 0);
}

// ==================================
// MAIN PROGRAM
// ==================================
int main() {
    Store toko;
    toko.loadFromFile();

    int role;

    do {
        cout << "\n=== SELAMAT DATANG DI TOKO BUKAN ORANG JAWA ===\n";
        cout << "\n=== LOGIN TOKO ONLINE ===\n";
        cout << "1. Admin\n";
        cout << "2. Konsumen\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> role;

        switch(role) {
            case 1: menuAdmin(toko); break;
            case 2: menuCustomer(toko); break;
            case 0: cout << "Keluar...\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (role != 0);

    return 0;
}
