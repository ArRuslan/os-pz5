#include <algorithm>
#include <codecvt>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <locale>
#include <fstream>
#include <vector>

#ifdef UNICODE
typedef wchar_t TCHAR;
#define _tcscpy(x, y) wcscpy(x, y)
#define _tcslen(x) wcslen(x)
#define _tcscmp(x, y) wcscmp(x, y)
#define _TEXT(x) L ## x
#define _tprintf wprintf

#else
typedef char TCHAR;
#define _tcscpy(x, y) strcpy(x, y)
#define _tcslen(x) strlen(x)
#define _tcscmp(x, y) strcmp(x, y)
#define _TEXT(x) x
#define _tprintf printf

#endif

void reop_stdout() {
    freopen(nullptr, "a", stdout);
}

wchar_t* any_to_wchar(void* in, int size) {
    bool is_wchar = false;
    for(int i = 0; i < size * 4; i++) {
        if(((char*)in)[i] == 0) {
            if(i != size - 1)
                is_wchar = true;
            break;
        }
    }

    if(!is_wchar) {
        size_t len = mbstowcs(nullptr, (char*)in, 0);
        auto* out = new wchar_t[len+1];
        mbstowcs(out, (char*)in, len);
        return out;
    } else {
        auto* out = new wchar_t[size];
        wcscpy(out, (wchar_t*)in);
        return out;
    }
}

bool task_h1_compare(void* path1in, void* path2in, int path1len, int path2len) {
    wchar_t* path1 = any_to_wchar(path1in, path1len);
    wchar_t* path2 = any_to_wchar(path2in, path2len);

    size_t len1 = wcslen(path1);
    size_t len2 = wcslen(path2);
    if(path1[len1 - 1] == L'\\')
        len1--;
    if(path2[len2 - 1] == L'\\')
        len2--;

    if(len1 != len2) {
        delete[] path1;
        delete[] path2;
        return false;
    }

    for(int i = 0; i < len1; i++) {
        if(path1[i] == path2[i])
            continue;
        int c1 = path1[i] >= 'a' ? path1[i] - 'a' : path1[i] - 'A';
        int c2 = path2[i] >= 'a' ? path2[i] - 'a' : path2[i] - 'A';
        if(c1 != c2) {
            delete[] path1;
            delete[] path2;
            return false;
        }
    }

    delete[] path1;
    delete[] path2;
    return true;
}

void task_h1() {
    //char path1[] = "C:\\Windows\\System32";
    TCHAR path1[] = _TEXT("C:\\Windows\\System32\\");
    TCHAR path2[] = _TEXT("C:\\WINDOWS\\System32\\");

    if(task_h1_compare((void*)path1, (void*)path2, sizeof(path1)/sizeof(path1[0]), sizeof(path2)/sizeof(path2[0])))
        _tprintf(_TEXT("Paths are the same!\n"));
    else
        _tprintf(_TEXT("Paths are not the same!\n"));
}

void task_h2() {
    const TCHAR* lines[] = {
        _TEXT("Hello"),
        _TEXT("world"),
        _TEXT("Test"),
        _TEXT("test"),
        _TEXT("456"),
        _TEXT("123")
    };

    const size_t length = sizeof(lines) / sizeof(lines[0]);

    std::sort(lines, lines+length, [=](const TCHAR* a, const TCHAR* b) {
        return _tcscmp(a, b) < 0;
    });

    _tprintf(_TEXT("Sorted:\n"));
    for (auto & line : lines) {
        _tprintf(_TEXT("  "));
        _tprintf(line);
        _tprintf(_TEXT("\n"));
    }
}

void task_h3() {
    reop_stdout();

    std::string filename;
    printf("File name to sort: ");
    std::getline(std::cin, filename);

    std::ifstream file(filename, std::ios::binary);

    std::vector<std::vector<char>> lines;

    char cur_char;
    lines.emplace_back();
    while(file.good()) {
        file.read(&cur_char, 1);
        if(cur_char == 0x0a) {
            lines.emplace_back();
            continue;
        }
        lines[lines.size()-1].push_back(cur_char);
    }

    file.close();

    printf("");

    std::sort(lines.begin(), lines.end(), [=](const std::vector<char>& a, const std::vector<char>& b) {
        int ai = 0, bi = 0;
        uint32_t ac, bc;

        int i;
        while(ai < a.size() && bi < b.size()) {
            ac = bc = 0;

            for(i = 0; i < 4; i++) {
                ac += a[ai+i];
                ac <<= 8 * i;
                if((a[ai+i] & 0b10000000) != 0b10000000)
                    break;
            }
            ai += i + 1;

            for(i = 0; i < 4; i++) {
                bc += b[bi+i];
                bc <<= 8 * i;
                if((b[bi+i] & 0b10000000) != 0b10000000)
                    break;
            }
            bi += i + 1;

            if(ac != bc)
                return ac < bc;
        }

        return a.size() < b.size();
    });

    std::ofstream ofile(filename+".sorted.txt", std::ios::binary | std::ios::trunc);

    cur_char = 0x0a;
    for(auto & line : lines) {
        ofile.write(&line[0], line.size());
        if(line != lines[lines.size()-1])
            ofile.write(&cur_char, 1);
    }

    ofile.close();

    printf("Done!\n");
}

int main() {
    task_h1();
    task_h2();
    task_h3();

    return 0;
}
