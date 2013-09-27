#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
  // Set locale to old school cp866 used for console IO
  setlocale(LC_ALL, "Russian_Russia.866");

  if (argc < 2)
  {
    wcout << L"Введите регулярное выражения слова в качестве единственного параметра." << endl;
    return 1;
  }

  wstring maskStr(argv[1]);

  wregex mask(maskStr, std::regex_constants::icase);

  // Open file as unicode stream
  wifstream ifile("ru_RU.dic");
  
  if (!ifile)
  {
    wcout << L"В рабочей директории должен находиться файл ru_RU.dic в кодировке KOI8-R." << endl;
    return 1;
  }

  // Set locale to KOI8-R
  locale koi8r("Russian_Russia.20866");
  ifile.imbue(koi8r);

  wstring word;

  std::vector<wstring> words;

  // Search through dictionary
  while (getline(ifile, word))
  {
    // Skip proper nouns, they begin with capital letter
    if (iswupper(word[0]))
    {
      continue;
    }

    size_t pos = word.find('/');

    // Trim meta information coming after slash sign
    if (pos != wstring::npos)
    {
      word.erase(pos);
    }

    // Check match against specified mask
    if (regex_match(word, mask))
    {
      words.push_back(std::move(word));
    }
  }

  sort(words.begin(), words.end(), [] (const wstring& a, const wstring& b)
  { 
    if (a.size() != b.size()) return a.size() < b.size(); 
    return a < b;
  });

  for (const auto& word : words)
  {
    wcout << word << std::endl;
  }

	return 0;
}