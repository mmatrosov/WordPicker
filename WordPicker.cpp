#include "stdafx.h"

using namespace std;

enum class DictType
{
  Freq, OpenOffice
};

//////////////////////////////////////////////////////////////////////////
///
vector<wstring> ReadWordsFreq()
{
#ifdef _DEBUG
  string dictPath = R"(data\freqrnc2011_crop.csv)";
#else
  string dictPath = R"(data\freqrnc2011.csv)";
#endif

  wifstream input(dictPath);

  if (!input)
  {
    cerr << "Dictionary not found in \"" + dictPath + "\"!" << endl;
    throw runtime_error("dictionary not found");
  }

  // An instance of codecvt is deleted by the locale object. 
  locale utf8(locale::empty(), new codecvt_utf8<wchar_t>);
  input.imbue(utf8);

  // Store frequency as a first field to use standard sorting order
  typedef pair<double, wstring> Cell;

  vector<Cell> table;

  wregex mask(LR"(^(.+)\ts\t([0-9]+\.[0-9]+).*)");
  wsmatch mr; 

  wstring line;

  while (getline(input, line))
  {
    if (regex_match(line, mr, mask))
    {
      wstring word = mr[1].str();
      double freq = boost::lexical_cast<double>(mr[2].str());
      table.push_back(make_pair(freq, move(word)));
    }
  }

  // Sort words by descending frequency
  sort(table.begin(), table.end(), greater<>());

  vector<wstring> words(table.size());

  transform(table.begin(), table.end(), words.begin(), 
    [](const Cell& cell) { return move(cell.second); });

  return words;
}

//////////////////////////////////////////////////////////////////////////
///
vector<wstring> ReadWordsOpenOffice()
{
#ifdef _DEBUG
  string dictPath = R"(data\ru_RU_crop.dic)";
#else
  string dictPath = R"(data\ru_RU.dic)";
#endif

  wifstream input(dictPath);

  if (!input)
  {
    cerr << "Dictionary not found in \"" + dictPath + "\"!" << endl;
    throw runtime_error("dictionary not found");
  }

  // An instance of codecvt is deleted by the locale object. 
  locale utf8(locale::empty(), new codecvt_utf8<wchar_t>);
  input.imbue(utf8);

  vector<wstring> words;

  wregex mask(L"([а-я]+)(/.*)?");
  wsmatch mr;

  wstring line;

  while (getline(input, line))
  {
    if (regex_match(line, mr, mask))
    {
      wstring word = mr[1].str();
      words.push_back(move(word));
    }
  }

  return words;
}

vector<wstring> ReadWords(DictType type)
{
  vector<wstring> words;

  cout << "Reading dictionary..." << endl;

  auto start = chrono::steady_clock::now();

  switch (type)
  {
  case DictType::Freq:
    words = ReadWordsFreq();
    break;
  case DictType::OpenOffice:
    words = ReadWordsOpenOffice();
    break;
  default:
    throw logic_error("Unknown dictionary type!");
  }

  auto finish = chrono::steady_clock::now();

  cout << "Done " << words.size() << " words in " <<
    chrono::duration_cast<chrono::milliseconds>(finish - start).count() << "ms" << endl;

  return words;
}

//////////////////////////////////////////////////////////////////////////
///
void MatchPatterns(const vector<wstring>& in_Words)
{
  static const vector<wstring> digits = { L"н", L"к", L"л", L"т", L"ч", L"п", L"[шщ]", L"с", L"в", L"д" };
  static const int count = digits.size();

  static const wstring vowels = L"[аеёиоуыэюя]";

  cout << endl;

  wcout << L"Списки слов для мнемонического запоминания цифр через согласные:" << endl;
  
  for (int i = 0; i < count; ++i)
  {
    wcout << i << ": " << digits[i] << endl;
  }

  cout << endl;

  for (int i = 0; i < count; ++i)
  {
    for (int j = 0; j < count; ++j)
    {
      cout << "-------------------------------------------------------------------------------" << endl;
      wcout << i << j << L" - " << digits[i] << digits[j] << ":" << endl;

      // Words with first two consonant letters equal to selected
      wregex mask(vowels + L"*" + digits[i] + vowels + L"*" + digits[j] + L".*");

      for (const auto& word : in_Words)
      {
        if (regex_match(word, mask))
        {
          wcout << word << endl;
        }
      }

      cout << endl;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
///
int _tmain(int argc, _TCHAR* argv[])
{
  // Set locale to old school cp866 used for console IO
  setlocale(LC_ALL, "Russian_Russia.866");

  try
  {
    // Initialize words list
    auto words = ReadWords(DictType::OpenOffice);

    MatchPatterns(words);
  }
  catch (std::exception&)
  {
    return 1;
  }

	return 0;
}
