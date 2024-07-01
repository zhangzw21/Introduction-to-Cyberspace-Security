#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <algorithm>

using namespace std;

// 初始换位表
int IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9,  1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7};

// 逆初始置换表
int IP_inverse[] = { 40, 8, 48, 16, 56, 24, 64, 32,
                    39, 7, 47, 15, 55, 23, 63, 31,
                    38, 6, 46, 14, 54, 22, 62, 30,
                    37, 5, 45, 13, 53, 21, 61, 29,
                    36, 4, 44, 12, 52, 20, 60, 28,
                    35, 3, 43, 11, 51, 19, 59, 27,
                    34, 2, 42, 10, 50, 18, 58, 26,
                    33, 1, 41, 9, 49, 17, 57, 25 };

// 扩展换位表
int E[48] = {
    32, 1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9,  10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1 };

// P盒置换表
int P[32] = { 16, 7, 20, 21, 29, 12, 28, 17,
           1, 15, 23, 26, 5, 18, 31, 10,
           2, 8, 24, 14, 32, 27, 3, 9,
           19, 13, 30, 6, 22, 11, 4, 25 };

// 循环移位表
int moveKey[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// PC1换位表（64—>56）
int PC1[56] = { 
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4 };

//PC2换位表（56—>48）
int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32 };

// 代换盒S盒
int S_BOX[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};
// 初始置换
string initial_permutation(string plainText) {
    string result;
    for (int i = 0; i < 64; ++i) {
        result += plainText[IP[i] - 1];
    }
    return result;
}

// 逆初始置换
string inverse_initial_permutation(string cipherText) {
    string result;
    for (int i = 0; i < 64; ++i) {
        result += cipherText[IP_inverse[i] - 1];
    }
    return result;
}

// 子密钥生成
vector<string> generate_subkeys(string key) {
    vector<string> subkeys;
    string key_permuted;
    for (int i = 0; i < 56; ++i) {
        key_permuted += key[PC1[i] - 1];
    }

    string C = key_permuted.substr(0, 28);
    string D = key_permuted.substr(28);

    for (int i = 0; i < 16; ++i) {  // 16个子密钥
        int n = moveKey[i];  // 左移n位
        while (n--) {
            char temp = C[0];
            for (int j = 0; j < 27; j++) {
                C[j] = C[j + 1];
            }
            C[27] = temp;

            temp = D[0];
            for (int j = 0; j < 27; j++) {
                D[j] = D[j + 1];
            }
            D[27] = temp;
        }

        // 合并 C 和 D
        string CD = C + D;

        // PC-2 置换得到子密钥
        string subkey;
        for (int j = 0; j < 48; ++j) {
            subkey += CD[PC2[j] - 1];
        }
        subkeys.push_back(subkey);
    }
    return subkeys;
}

string xorFunction(string L, string R) {
    string result;
    for (int i = 0; i < L.size(); ++i) {
        result += to_string((L[i] - '0') ^ (R[i] - '0'));
    }
    return result;
}

// 轮函数F
string roundFunction(string R, string subkey) {
    string expanded_R;  // 32为扩展为48位
    for (int i = 0; i < 48; ++i) {
        expanded_R += R[E[i] - 1];
    }

    string xor_result = xorFunction(expanded_R, subkey);

    string result;
    for (int i = 0; i < 8; ++i) {  // 48位分成8个6位数块
        string B = xor_result.substr(i * 6, 6);
        int row = (B[0] << 1) + B[5];
        int col = (B[1] << 3) + (B[2] << 2) + (B[3] << 1) + B[4];
        int val = S_BOX[i][row][col];  // S盒代换,6位变为4位
        result += bitset<4>(val).to_string();
    }

    string permuted_result;
    for (int i = 0; i < 32; ++i) {
        permuted_result += result[P[i] - 1];  // P盒代换
    }

    return permuted_result;
}



// Feistel加密
string feistelEncrypt(string plainText, string key) {
    string permuted_plaintext = initial_permutation(plainText);
    vector<string> subkeys = generate_subkeys(key);

    string L = permuted_plaintext.substr(0, 32);
    string R = permuted_plaintext.substr(32);

    for (int i = 0; i < 16; ++i) {
        string temp = R;
        R = xorFunction(L, roundFunction(R, subkeys[i]));
        L = temp;
    }

    return inverse_initial_permutation(R + L);
}

// Feistel解密
string feistelDecrypt(string cipherText, string key) {
    string permuted_ciphertext = initial_permutation(cipherText);
    vector<string> subkeys = generate_subkeys(key);

    string L = permuted_ciphertext.substr(0, 32);
    string R = permuted_ciphertext.substr(32);

    for (int i = 15; i >= 0; --i) {

        string temp = R;
        R = xorFunction(L, roundFunction(R, subkeys[i]));
        L = temp;
    }
    return inverse_initial_permutation(R + L);
}

// 字符串转换为二进制字符串
string strToBinary(string str) {
    string binaryString;
    for (char c : str) {
        int asciiValue = static_cast<int>(c);
        binaryString += bitset<8>(asciiValue).to_string();
    }
    return binaryString;
}

// 二进制字符串转换为字符串
string binaryToStr(string binaryString) {
    string result;
    for (int i = 0; i < binaryString.length(); i += 8) {
        string byteString = binaryString.substr(i, 8);
        int asciiValue = bitset<8>(byteString).to_ulong();
        result += static_cast<char>(asciiValue);
    }
    return result;
}

// 二进制字符串转换为十六进制字符串
string binaryToHex(string binaryString) {
    string hexString;
    for (int i = 0; i < binaryString.length(); i += 4) {
        string nibble = binaryString.substr(i, 4); // 每4位二进制数一组
        int decimalValue = bitset<4>(nibble).to_ulong(); // 将二进制数转换为十进制数
        char hexChar = (decimalValue < 10) ? ('0' + decimalValue) : ('A' + decimalValue - 10); // 转换为十六进制字符
        hexString += hexChar;
    }
    return hexString;
}

// 十六进制字符串转换为二进制字符串
string hexToBinary(string hexString) {
    string binaryString;
    for (char c : hexString) {
        int decimalValue = (c >= '0' && c <= '9') ? (c - '0') : (c - 'A' + 10); // 将十六进制字符转换为十进制数
        string nibble = bitset<4>(decimalValue).to_string(); // 将十进制数转换为四位二进制数
        binaryString += nibble;
    }
    return binaryString;
}

int main() {
    std::cout << "【Feistel加密/解密】" << endl;
    bool flag = true;
    string choice;
    string Key;
    string binaryKey;
    while (flag) {
        std::cout << "请选择：A、加密，B、解密" << endl;
        std::getline(cin, choice);
        while (choice != "A" && choice != "B" && choice != "a" && choice != "b") {
            std::cout << "对不起，您的输入不合法。请选择A或B，A表示加密，B表示解密。" << endl;
            std::getline(cin, choice);
        }
        std::cout << "请输入8位密钥：" << endl;
        std::getline(cin, Key);
        while (Key.size() != 8) {
            printf("您输入的密钥位数有误，请重新输入8位密钥：\n");
            std::getline(cin, Key);
        }
        binaryKey = strToBinary(Key);  // 64位二进制密钥

        // 加密
        if (choice == "A" || choice == "a") {
            std::cout << "请输入您想加密的内容：" << endl;
            string plainText;  // 明文
            std::getline(cin, plainText);
            while (plainText[0] == '\0') {
                std::cout << "对不起，明文不可为空，请您输入正确的明文。" << endl;
                std::getline(cin, plainText);
            }
            while (plainText.size() % 8 != 0) {  // 将明文空格填充到8个字符整数倍
                plainText += ' ';
            }
            string binaryPlainText = strToBinary(plainText);  // 二进制明文

            // 分组加密
            string binaryCipherText;  // 二进制密文串
            for (int i = 0; i < binaryPlainText.length(); i += 64) {
                string binaryBlock = binaryPlainText.substr(i, 64);  // 二进制明文串每64个字符一组
                binaryCipherText += feistelEncrypt(binaryBlock, binaryKey);
            }
            string cipherText = binaryToHex(binaryCipherText);  // 最终密文串以十六进制展示
            std::cout << "您的明文经过Feistel加密后的密文是： " << cipherText << endl;
        }

        // 解密
        else if (choice == "B" || choice == "b") {
            std::cout << "请输入密文内容：" << endl;  // 输入密文
            string cipherText;  // 密文
            std::getline(cin, cipherText);
            while (cipherText[0] == '\0') {
                std::cout << "对不起，密文不可为空，请您输入正确的密文。" << endl;
                std::getline(cin, cipherText);
            }
            string binaryCipherText = hexToBinary(cipherText);  // 二进制密文

            // 分组解密
            string binaryDecryptedText;  // 二进制解密明文串
            for (int i = 0; i < binaryCipherText.length(); i += 64) {
                string binaryBlock = binaryCipherText.substr(i, 64);  // 二进制密文串每64个字符一组
                binaryDecryptedText += feistelDecrypt(binaryBlock, binaryKey);
            }
            string decryptedText = binaryToStr(binaryDecryptedText);  // 最终解密明文串以字符展示
            std::cout << "您的密文经过DES解密后的明文是：" << decryptedText << endl;
        }
        flag = false;
        std::cout << "是否继续？" << endl;
        std::cout << "Y继续，N退出，请选择：" << endl;
        std::getline(cin, choice);
        while (choice != "Y" && choice != "y" && choice != "N" && choice != "n") {
            std::cout << "对不起，您的输入不合法。请选择Y或N，Y表示继续使用本程序，N表示退出。" << endl;
            std::getline(cin, choice);
        }
        if (choice == "Y" || choice == "y")
            flag = true;
    }
    return 0;
}