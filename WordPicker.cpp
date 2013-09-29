#include "stdafx.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
///
vector<wstring> ReadWords()
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

  cout << "Reading dictionary..." << endl;

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
void MatchPatterns(const vector<wstring>& in_Words)
{

}

//////////////////////////////////////////////////////////////////////////
///
int _tmain(int argc, _TCHAR* argv[])
{
  try
  {
    // Initialize words list
    auto words = ReadWords();


  }
  catch (std::exception&)
  {
    return 1;
  }

	return 0;
}
