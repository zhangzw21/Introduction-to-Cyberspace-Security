#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// 按照递减顺序排序
bool compare(pair<char, int> a, pair<char, int> b) {
	return a.second > b.second;
}

int main() {
	string text = "UZ QSO VUOHXMOPV GPOZPEVSG ZWSZ OPFPESX UDBMETSX AIZ VUEPHZ HMDZSHZO WSFP APPD TSVP QUZW YMXUZUHSX EPYEPOPDZSZUFPO MB ZWP FUPZ HMDJ UD TMOHMQ";

	// 统计密文文本中各字母的出现频率
	map<char, int> frequencyMap;
	for (char c : text) {
		if (isalpha(c)) {
			frequencyMap[c]++;
		}
	}
	
	// 将频率统计结果存入vector中，以便排序
	vector<pair<char, int>> frequencyVector(frequencyMap.begin(), frequencyMap.end());

	// 按照频率的递减顺序排序
	sort(frequencyVector.begin(), frequencyVector.end(), compare);

	int totalCharacters = 0; // 总字符数
	for (auto character : frequencyVector) {
		totalCharacters += character.second;
	}

	// 输出结果
	cout << left << setw(10) << "密文字母" << setw(10) << "出现次数" << "出现频率" << endl;
	cout << fixed << setprecision(2);
	for (auto character : frequencyVector) {
		double frequencyPercentage = (static_cast<double>(character.second) / totalCharacters) * 100.0;
		cout << left << setw(10) << character.first << setw(10) << character.second << frequencyPercentage << "%" << endl;
	}
	cout << endl;

	char mapping_alpha[26] = { ' ' };
	mapping_alpha[frequencyVector[0].first - 'A'] = 'e';  // 极高频
	mapping_alpha[frequencyVector[1].first - 'A'] = 't';  // 次高频
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == frequencyVector[0].first) {
			text[i] = 'e';
		}
		else if (text[i] == frequencyVector[1].first) {
			text[i] = 't';
		}
	}
	cout << "将密文中字母P替换为e，字母Z替换为t，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['W' - 'A'] = 'h';  // the
	mapping_alpha['S' - 'A'] = 'a';  // that
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'W') {
			text[i] = 'h';
		}
		else if (text[i] == 'S') {
			text[i] = 'a';
		}
	}
	cout << "将密文中字母W替换为h，字母S替换为a，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['F' - 'A'] = 'v';  // have
	mapping_alpha['U' - 'A'] = 'i';  // it
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'F') {
			text[i] = 'v';
		}
		if (text[i] == 'U') {
			text[i] = 'i';
		}
	}
	cout << "将密文中字母F替换为v，字母U替换为i，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['Q' - 'A'] = 'w';  // with
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'Q') {
			text[i] = 'w';
		}
	}
	cout << "将密文中字母Q替换为w，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['O' - 'A'] = 's';  // was
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'O') {
			text[i] = 's';
		}
	}
	cout << "将密文中字母O替换为s，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['E' - 'A'] = 'r';  // several
	mapping_alpha['X' - 'A'] = 'l';  // several
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'E') {
			text[i] = 'r';
		}
		if (text[i] == 'X') {
			text[i] = 'l';
		}
	}
	cout << "将密文中字母E替换为r，字母X替换为l，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['Y' - 'A'] = 'p';  // representative
	mapping_alpha['D' - 'A'] = 'n';  // representative
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'Y') {
			text[i] = 'p';
		}
		if (text[i] == 'D') {
			text[i] = 'n';
		}
	}
	cout << "将密文中字母Y替换为p，字母D替换为n，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['V' - 'A'] = 'd';  // direct
	mapping_alpha['H' - 'A'] = 'c';  // direct
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'V') {
			text[i] = 'd';
		}
		if (text[i] == 'H') {
			text[i] = 'c';
		}
	}
	cout << "将密文中字母V替换为d，字母H替换为c，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['G' - 'A'] = 'y';  // yesterday
	mapping_alpha['M' - 'A'] = 'o';  // contacts  political
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'G') {
			text[i] = 'y';
		}
		if (text[i] == 'M') {
			text[i] = 'o';
		}
	}
	cout << "将密文中字母G替换为y，字母M替换为o，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['A' - 'A'] = 'b';  // been
	mapping_alpha['B' - 'A'] = 'f';  // of
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'A') {
			text[i] = 'b';
		}
		if (text[i] == 'B') {
			text[i] = 'f';
		}
	}
	cout << "将密文中字母A替换为b，字母B替换为f，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['T' - 'A'] = 'm';  // informal
	mapping_alpha['I' - 'A'] = 'u';  // but
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'T') {
			text[i] = 'm';
		}
		if (text[i] == 'I') {
			text[i] = 'u';
		}
	}
	cout << "将密文中字母T替换为m，字母I替换为u，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	mapping_alpha['J' - 'A'] = 'g';  // cong
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == 'J') {
			text[i] = 'g';
		}
	}
	cout << "将密文中字母J替换为g，替换后的密文：" << endl;
	cout << text << endl;
	cout << endl;

	cout << "      [替换表]" << endl;
	cout << "密文字母" << "    " << "明文字母" << endl;
	for (int i = 0; i < 26; i++) {
		cout << "   " << char('A' + i) << "           " << mapping_alpha[i] << endl;
	}
	return 0;
}

