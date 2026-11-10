#include <bits/stdc++.h>
using namespace std;

class BankAccountPayment {
    protected:
        string bankCode;
        string bankName;
        string accountNumber;
        double balance;
    public:
        BankAccountPayment(string code, string name, string account, double money) {
            bankCode = code;
            bankName = name;
            accountNumber = account;
            balance = money;
        }
        virtual ~BankAccountPayment() = default;

        // Getter
        string getBankCode() const {
            return bankCode;
        }
        string getBankName() const {
            return bankName;
        }
        string getAccountNumber() const {
            return accountNumber;
        }
        double getBalance() const {
            return balance;
        }

        virtual double calculateFee(double amount) const = 0;

        virtual bool pay(double amount) {
            double total = amount + calculateFee(amount);
            if (amount > 0 && balance >= total) {
                balance -= total;
                return true;
            }
            return false;
        }
};

class VCBPayment : public BankAccountPayment {
    public:
        VCBPayment(string account, double money)
            : BankAccountPayment("VCB", "Vietcombank", account, money) {
        }

        double calculateFee(double) const override {
            return 1100.0;
        }
};

class TCBPayment : public BankAccountPayment {
    public:
        TCBPayment(string account, double money)
            : BankAccountPayment("TCB", "Techcombank", account, money) {
        }

        double calculateFee(double) const override {
            return 0.0;
        }
};

// 3. Lop cong dieu phoi (PaymentGateway)
class PaymentGateway {
    private:
        vector<shared_ptr<BankAccountPayment>> paymentMethods;

    public:
        // Them mot phuong thuc thanh toan vao danh muc
        void addPaymentMethod(shared_ptr<BankAccountPayment> method) {
            if (method != nullptr) {
                paymentMethods.push_back(method);
            }
        }

        // Duyet qua danh sach va in menu ra man hinh
        void displaySupportedBanks() const {
            cout << "=== CONG THANH TOAN NGAN HANG ===" << endl;
            if (paymentMethods.empty()) {
                cout << "Hien chua co ngan hang nao duoc ho tro." << endl;
                return;
            }

            for (size_t i = 0; i < paymentMethods.size(); ++i) {
                const auto& bank = paymentMethods[i];
                string acc = bank->getAccountNumber();
                string maskedAcc = (acc.length() > 6) ? (acc.substr(0, 6) + " ...") : acc;

                cout << "[" << (i + 1) << "] "
                     << bank->getBankName() << " (" << bank->getBankCode() << ")"
                     << " - So tai khoan: " << maskedAcc
                     << " | So du: " << bank->getBalance() << " VND"
                     << endl;
            }
        }

        // Tra ve doi tuong theo lua chon cua nguoi dung (1, 2, ...)
        shared_ptr<BankAccountPayment> selectBank(int index) const {
            if (index < 1 || index > static_cast<int>(paymentMethods.size())) {
                return nullptr;
            }
            return paymentMethods[index - 1];
        }

        int getBankCount() const {
            return static_cast<int>(paymentMethods.size());
        }
};

class MBBankPayment : public BankAccountPayment {
    public:
        MBBankPayment(string account, double money)
            : BankAccountPayment("MBB", "MBBank", account, money) {
        }

        double calculateFee(double amount) const override {
            return amount * 0.002;
        }
};

int main() {
    cout << setprecision(12);

    PaymentGateway gateway;

    // Khoi tao va dang ky cac ngan hang ho tro
    gateway.addPaymentMethod(make_shared<VCBPayment>("001100123456", 500000));
    gateway.addPaymentMethod(make_shared<TCBPayment>("190356789012", 300000));
    gateway.addPaymentMethod(make_shared<MBBankPayment>("0987654321", 1000000));

    gateway.displaySupportedBanks();

    int bankCount = gateway.getBankCount();
    if (bankCount == 0) {
        return 0;
    }

    int choice;
    cout << "Chon ngan hang (1-" << bankCount << "): ";
    if (!(cin >> choice)) {
        cout << "Lua chon khong hop le!" << endl;
        return 0;
    }

    shared_ptr<BankAccountPayment> selectedBank = gateway.selectBank(choice);
    if (selectedBank == nullptr) {
        cout << "Lua chon khong hop le!" << endl;
        return 0;
    }

    double amount;
    cout << "\nNhap so tien can thanh toan: ";
    if (!(cin >> amount) || amount <= 0) {
        cout << "So tien khong hop le!" << endl;
        return 0;
    }

    double fee = selectedBank->calculateFee(amount);
    double total = amount + fee;

    cout << "\n--- XU LY GIAO DICH ---" << endl;
    cout << "Ngan hang: " << selectedBank->getBankName() << endl;
    cout << "So tien: " << amount << " VND" << endl;
    cout << "Phi giao dich (" << selectedBank->getBankCode() << "): " << fee << " VND" << endl;
    cout << "Tong tien tru: " << total << " VND" << endl;

    if (selectedBank->pay(amount)) {
        cout << "-> THANH TOAN THANH CONG!" << endl;
        cout << "So du con lai: " << selectedBank->getBalance() << " VND" << endl;
    } else {
        cout << "-> THANH TOAN THAT BAI! (So du khong du)" << endl;
    }

    return 0;
}
