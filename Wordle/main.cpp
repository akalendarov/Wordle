#include "bits/stdc++.h"
using namespace std;

#define sz(x) int((x).size())
#define all(x) (x).begin(), (x).end()
#define x first
#define y second

const string Alphabet = "abcdefghijklmnopqrstuvwxyz";
const int Length = 5;

enum ELetterCount {
	Atleast,
	Exactly,
};

enum ELetterPosition {
	No = 0,
	Maybe,
	Yes,
};

struct TLetterInfo {
	pair<ELetterCount, int> count;
	vector<ELetterPosition> pos;
};

vector<TLetterInfo> GetInfo(const vector<pair<string, string>>& a) {
	vector<TLetterInfo> result;

	for (char c : Alphabet) {
		pair<ELetterCount, int> count = { ELetterCount::Atleast, 0 };
		for (const auto &i : a) {
			int c_grey = 0, c_yellow = 0, c_green = 0;
			for (int pos = 0; pos < Length; ++pos) {
				if (i.x[pos] == c) {
					if (i.y[pos] == '0') {
						++c_grey;
					}
					else if (i.y[pos] == '1') {
						++c_yellow;
					}
					else {
						assert(i.y[pos] == '2');
						++c_green;
					}
				}
			}

			pair<ELetterCount, int> current;
			if (c_grey != 0) {
				current = { ELetterCount::Exactly, c_green + c_yellow };
			}
			else {
				current = { ELetterCount::Atleast, c_green + c_yellow };
			}

			if (count.x == ELetterCount::Atleast) {
				if (current.x == ELetterCount::Atleast) {
					count.y = max(count.y, current.y);
				}
				else {
					assert(current.x == ELetterCount::Exactly);
					assert(current.y >= count.y);
					count = current;
				}
			}
			else {
				assert(count.x == ELetterCount::Exactly);
				if (current.x == ELetterCount::Atleast) {
					assert(current.y <= count.y);
				}
				else {
					assert(current.x == ELetterCount::Exactly);
					assert(current.y == count.y);
				}
			}
		}

		vector<ELetterPosition> positions(Length, ELetterPosition::Maybe);
		for (const auto& i : a) {
			for (int pos = 0; pos < Length; ++pos) {
				if (i.x[pos] == c) {
					if (i.y[pos] == '0') {
						assert(positions[pos] != ELetterPosition::Yes);
						positions[pos] = ELetterPosition::No;
					}
					else if (i.y[pos] == '1') {
						assert(positions[pos] != ELetterPosition::Yes);
						positions[pos] = ELetterPosition::No;
					}
					else {
						assert(i.y[pos] == '2');
						assert(positions[pos] != ELetterPosition::No);
						positions[pos] = ELetterPosition::Yes;
					}
				}
			}
		}

		int min_c = 0, max_c = 0;
		for (const auto& i : positions) {
			if (i == ELetterPosition::Maybe) {
				++max_c;
			}
			else if (i == ELetterPosition::Yes) {
				++min_c;
				++max_c;
			}
		}
		assert(min_c <= count.y);
		assert(count.y <= max_c);

		result.push_back({ count, positions });
	}

	return result;
}

bool Check(const string& s, const vector<TLetterInfo>& info) {
	for (int i = 0; i < sz(Alphabet); ++i) {
		for (int pos = 0; pos < sz(s); ++pos) {
			if (s[pos] == Alphabet[i]) {
				if (info[i].pos[pos] == ELetterPosition::No) {
					return false;
				}
			}
			else {
				if (info[i].pos[pos] == ELetterPosition::Yes) {
					return false;
				}
			}
		}

		int cnt_now = count(all(s), Alphabet[i]);
		int min_cnt = cnt_now;
		int max_cnt = cnt_now;
		for (int pos = sz(s); pos < Length; ++pos) {
			if (info[i].pos[pos] == ELetterPosition::Yes) {
				++min_cnt;
				++max_cnt;
			}
			else if (info[i].pos[pos] == ELetterPosition::Maybe) {
				++max_cnt;
			}
		}

		if (info[i].count.x == ELetterCount::Atleast) {
			if (max_cnt < info[i].count.y) {
				return false;
			}
		}
		else {
			assert(info[i].count.x == ELetterCount::Exactly);
			if (info[i].count.y < min_cnt || max_cnt < info[i].count.y) {
				return false;
			}
		}
	}

	return true;
}

void Go(string s, const vector<TLetterInfo> &info) {
	if (!Check(s, info)) {
		return;
	}
	if (sz(s) == Length) {
		cout << s << endl;
		return;
	}

	for (char c : Alphabet) {
		Go(s + c, info);
	}
}

int main() {
	// Example: My wordle 415
	const vector<pair<string, string>> tries = {
		{"adieu", "00101"},
		{"minus", "01110"},
		{"cutin", "01121"},
		{"unlit", "22022"},
		{"unfit", "22222"}
	};

	for (const auto& i : tries) {
		assert(sz(i.x) == Length);
		assert(sz(i.y) == Length);

		for (const auto& j : i.x) {
			assert(Alphabet.find(j) != string::npos);
		}

		for (const auto& j : i.y) {
			assert('0' <= j && j <= '2');
		}
	}

	auto info = GetInfo(tries);
	assert(sz(info) == sz(Alphabet));

	auto printPositions = [](const vector<ELetterPosition>& pos) {
		assert(sz(pos) == Length);
		for (int i = 0; i < Length; ++i) {
			if (pos[i] == ELetterPosition::No) {
				cout << '-';
			}
			else if (pos[i] == ELetterPosition::Yes) {
				cout << '+';
			}
			else {
				assert(pos[i] == ELetterPosition::Maybe);
				cout << '?';
			}
		}
		cout << endl;
	};

	for (int i = 0; i < sz(Alphabet); ++i) {
		cout << "Letter " << Alphabet[i] << endl;
		if (info[i].count.x == ELetterCount::Atleast) {
			if (info[i].count.y == 0) {
				cout << "No info about count" << endl;
			}
			else {
				cout << "Atleast " << info[i].count.y << " times" << endl;
				printPositions(info[i].pos);
			}
		}
		else {
			assert(info[i].count.x == ELetterCount::Exactly);
			if (info[i].count.y == 0) {
				cout << "Not in string" << endl;
			}
			else {
				cout << "Exactly " << info[i].count.y << " times" << endl;
				printPositions(info[i].pos);
			}
		}
		cout << endl;
	}

	Go("", info);
}