#include "KeyDumpster.h"

KeyDumpster::KeyDumpster()
{
#if defined _DEBUG
  std::wcout << "KeyDumpster Constructed." << std::endl;
#else
#endif
}

// Find patter in memory buffer.
std::vector<unsigned char*> KeyDumpster::Find(const char* pattern, unsigned char* rangeStart, unsigned char* rangeEnd) {

  std::vector<unsigned char*> keyLocations;
  const unsigned char* pat = reinterpret_cast<const unsigned char*>(pattern);
  unsigned char* firstMatch = 0;
  for (unsigned char* pCur = rangeStart; pCur < rangeEnd; ++pCur) {
    if (*(unsigned char*)pat == (char)'\?' || *pCur == getByte(pat)) {
      if (!firstMatch) firstMatch = pCur;
      pat += (*(unsigned short*)pat == (short)'\?\?' || *(unsigned char*)pat != (char)'\?') ? 2 : 1;
      if (!*pat) {
        keyLocations.push_back(firstMatch);
        continue;
      }
      pat++;
      if (!*pat) {
        keyLocations.push_back(firstMatch);
        continue;
      }
    }
    else if (firstMatch) {
      pCur = firstMatch;
      pat = reinterpret_cast<const unsigned char*>(pattern);
      firstMatch = 0;
    }
  }
  return keyLocations;
}

// AES String concatenator type
std::string KeyDumpster::ConcatenateAESType(unsigned char* keyAddr, int type) {

  std::string hex_string = "";
  for (unsigned long long i = 0; i < m_keyDwordOffsets[type].size(); i++) {
    hex_string += hexStr(&keyAddr[m_keyDwordOffsets[type][i]], 4);
  }
  std::transform(hex_string.begin(), hex_string.end(), hex_string.begin(), ::toupper);
  return hex_string;
}

//Scan memory aes key buffer and extract the individual dword key buffers.
bool KeyDumpster::FindAESKeys(char* buffer, unsigned long long size) {

  for (unsigned long long i = 0; i < m_keyPatterns.size(); i++) {
    std::vector<unsigned char*> local_aes_key_addresses = Find(m_keyPatterns[i].c_str(), (unsigned char*)buffer, (unsigned char*)buffer + size);
    for (unsigned long long j = 0; j < local_aes_key_addresses.size(); j++) {
      // Concat and push key to vector.
      Key key = Key(ConcatenateAESType(local_aes_key_addresses[j], i));
      m_keys.m_keyvector.push_back(key);
    }
  }
  // Generate entropy score for each key.
  m_keyEntropies = KeyEntropyGenerator(m_keys);
  m_MostLikelyKey = FindMaxElements(m_keyEntropies).second;

  if (m_keyEntropies.empty() || m_MostLikelyKey.empty() || m_keys.getKeys().empty())
  {
    return false;
  }

  return true;
}

//Generate aes key entropy score.
std::vector<double> KeyDumpster::KeyEntropyGenerator(Keys keys) {

  std::vector<double> keyEntropies{};
  for (unsigned long long i = 0; i < keys.getKeys().size(); i++) {
    keyEntropies.push_back(CalcEntropy(keys.getKeys()[i].getKey()));
  }
  return keyEntropies;
}

// print key information.
bool KeyDumpster::PrintKeyInformation() {

  for (unsigned long long i = 0; i < m_keys.getKeys().size(); i++) {

    if (m_keyEntropies[i] >= 3.3) {
      if (std::find(m_falsePositives.begin(), m_falsePositives.end(), m_keys.getKeys()[i].getKey()) == m_falsePositives.end()) {
        printf("Key: 0x%s | Key Entropy: %f\n\n", m_keys.getKeys()[i].getKey().c_str(), m_keyEntropies[i]);
      }
    }
  }
  return true;
}

// log2 from: https://tfetimes.com/c-entropy/
double KeyDumpster::log2_intrinsic(double number) {
  return log(number) / log(2);
}

// hexStr from stackoverflow: https://stackoverflow.com/questions/14050452/how-to-convert-byte-array-to-hex-string-in-visual-c
std::string KeyDumpster::hexStr(const unsigned char* data, int len) {

  std::stringstream ss;
  ss << std::hex;

  for (int i(0); i < len; ++i)
    ss << std::setw(2) << std::setfill('0') << (int)data[i];

  return ss.str();
}

// Calculate entropy score on AES keys.
double KeyDumpster::CalcEntropy(std::string keyString) {

  std::map<char, int> frequencies;
  for (char c : keyString)
    frequencies[c]++;

  int numlen = keyString.length();
  double infocontent = 0;
  for (std::pair<char, int> p : frequencies) {
    double freq = static_cast<double>(p.second) / numlen;
    infocontent += freq * log2_intrinsic(freq);
  }
  infocontent *= -1;
  return infocontent;
}

// Find the biggest double value in a vector.
std::pair<double, std::vector<unsigned long long>> KeyDumpster::FindMaxElements(std::vector<double> const& v) {

  std::vector<unsigned long long> indices;
  double current_max = -DBL_MAX;

  for (unsigned long long i = 0; i < v.size(); ++i) {
    if (v[i] > current_max) {
      current_max = v[i];
      indices.clear();
    }

    if (v[i] == current_max) {
      indices.push_back(i);
    }
  }
  return std::make_pair(current_max, indices);
}

//Destructor.
KeyDumpster::~KeyDumpster() 
{
  m_keyEntropies.clear();
  m_MostLikelyKey.clear();

#if defined _DEBUG
  std::wcout << "KeyDumpster Destructed." << std::endl;
#else
#endif
}
