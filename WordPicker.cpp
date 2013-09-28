#include "stdafx.h"

using namespace std;

vector<wstring> words;

vector<wstring> ReadWords()
{
  wifstream input("data\\freqrnc2011.csv");

  if (!input)
  {
    wcerr << L"Не найден файл со словарём \"data\freqrnc2011.csv\"!" << endl;    
    throw runtime_error("dictionary not found");
  }

  // An instance of codecvt is deleted by the locale object. 
  locale utf8(locale::empty(), new codecvt_utf8<wchar_t>);
  input.imbue(utf8);

  vector<wstring> words;

//  wregex mask(L"^:Al*\ts\t:n");
  wregex mask(L"\ts\t([0-9]+\\.[0-9]+)");
  wsmatch mr; 

  wstring word;

  while (getline(input, word))
  {
    if (regex_search(word, mr, mask))
    {
      words.push_back(move(word));
    }
  }

  return words;
}

int _tmain(int argc, _TCHAR* argv[])
{
  // Set locale to old school cp866 used for console IO
  setlocale(LC_ALL, "Russian_Russia.866");

  if (argc < 2)
  {
    wcerr << L"Введите регулярное выражения слова в качестве единственного параметра." << endl;
    return 1;
  }

  // Initialize words list
  words = ReadWords();

	return 0;
}
