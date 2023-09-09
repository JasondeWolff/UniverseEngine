// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <array>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include <set>

namespace tinyusdz {

enum class CharEncoding
{
  None,
  UTF8,
  UTF8BOM, // UTF8 + BOM
  UTF16LE  // UTF16 LE(Windows Unicode)
};

inline const std::string to_lower(const std::string &str) {
  std::string result = str;
  std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
    if ((c >= 'A') && (c <= 'Z')) {
      c += 32;
    }
    return static_cast<char>(c);
  });
  return result;
}

inline bool hasNewline(const std::string &str) {
  for (size_t i = 0; i < str.size(); i++) {
    if ((str[i] == '\r') || (str[i] == '\n')) {
      return true;
    }
  }

  return false;
}

inline bool startsWith(const std::string &str, const std::string &t) {
  return (str.size() >= t.size()) &&
         std::equal(std::begin(t), std::end(t), std::begin(str));
}

inline bool endsWith(const std::string &str, const std::string &suffix) {
  return (str.size() >= suffix.size()) &&
         (str.find(suffix, str.size() - suffix.size()) != std::string::npos);
}

inline std::string removePrefix(const std::string &str,
                                const std::string &prefix) {
  if (startsWith(str, prefix)) {
    return str.substr(prefix.length());
  }
  return str;
}

inline std::string removeSuffix(const std::string &str,
                                const std::string &suffix) {
  if (endsWith(str, suffix)) {
    return str.substr(0, str.length() - suffix.length());
  }
  return str;
}

inline bool contains(const std::string &str, char c) {
  return str.find(c) != std::string::npos;
}

inline bool contains_str(const std::string &str, const std::string &substr) {
  return str.find(substr) != std::string::npos;
}

inline size_t counts(const std::string &str, char c) {
  size_t cnt = 0;
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] == c) {
      cnt++;
    }
  }
  return cnt;
}

// Remove the beginning and the ending delimiter(s) from input string
// e.g. "mystring" -> mystring
// no error for an input string which does not contain `delim` in both side.
inline std::string unwrap(const std::string &str,
                          const std::string &delim = "\"") {
  size_t n = delim.size();

  if (str.size() < n) {
    return str;
  }

  std::string s = str;

  if (s.substr(0, n) == delim) {
    s.erase(0, n);
  }

  if (s.substr(s.size() - n) == delim) {
    s.erase(s.size() - n);
  }

  return s;
}

inline std::string unwrap(const std::string &str, const std::string &l,
                          const std::string &r) {
  return removePrefix(removeSuffix(str, r), l);
}

inline std::string quote(const char *s, const std::string &quote_str = "\"") {
  return quote_str + std::string(s) + quote_str;
}

inline std::string quote(const std::string &s,
                         const std::string &quote_str = "\"") {
  return quote_str + s + quote_str;
}

inline std::string wquote(const std::string &s,
                          const std::string &quote_lstr = "\"",
                          const std::string &quote_rstr = "\"") {
  return quote_lstr + s + quote_rstr;
}

#if 0
template<typename It>
inline It quote(const It& v, const std::string &quote_str = "\"") {

  It dst;

  for (typename It::const_iterator it = v.begin(); it != v.end(); ++it) {
    dst.emplace_back(quote((*it), quote_str));
  }

  return dst;
}
#else
inline std::vector<std::string> quote(const std::vector<std::string> &vs,
                                      const std::string &quote_str = "\"") {
  std::vector<std::string> dst;

  for (const auto &item : vs) {
    dst.emplace_back(quote(item, quote_str));
  }

  return dst;
}
#endif

// Python like join  ", ".join(v)
template <typename It>
inline std::string join(const std::string &sep, const It &v) {
  std::ostringstream oss;
  if (!v.empty()) {
    typename It::const_iterator it = v.begin();
    oss << *it++;
    for (typename It::const_iterator e = v.end(); it != e; ++it)
      oss << sep << *it;
  }
  return oss.str();
}

// To avoid splitting toooo large input text(e.g. few GB).
inline std::vector<std::string> split(
    const std::string &str, const std::string &sep,
    const uint32_t kMaxItems = (std::numeric_limits<int32_t>::max)() / 100) {
  size_t s;
  size_t e = 0;

  size_t count = 0;
  std::vector<std::string> result;

  while ((s = str.find_first_not_of(sep, e)) != std::string::npos) {
    e = str.find(sep, s);
    result.push_back(str.substr(s, e - s));
    if (++count > kMaxItems) {
      break;
    }
  }

  return result;
}

//
// "{name=varname}"
//
// => ["name", "varname"]
//
// "{name=}"
//
// => ["name", ""]
//
// Return false when input string is not a variant element, or `name` and `varname` contains invalid characters.
//
bool tokenize_variantElement(const std::string &elementName, std::array<std::string, 2> *result = nullptr);

bool is_variantElementName(const std::string &elementName);

///
/// Test if str contains " or '
///
/// @param[in] is_double_quote true: find escaped triple double quotes. false find escaped single double quotes.
///
bool hasQuotes(const std::string &str, bool is_double_quote);

///
/// Test if str contains """ or '''
///
/// @param[in] is_double_quote true: find escaped triple double quotes. false find escaped single double quotes.
///
bool hasTripleQuotes(const std::string &str, bool is_double_quote);

///
/// Test if str contains \""" or \'''
///
/// @param[in] is_double_quote true: find escaped triple double quotes. false find escaped single double quotes.
/// @param[out] n The number of escaped triple quotes
///
/// Return true immediately when an escaped triple quotes found when `n` is nullptr.
///
bool hasEscapedTripleQuotes(const std::string &str, bool is_double_quote, size_t *n = nullptr);

std::string escapeSingleQuote(const std::string &str, const bool is_double_quote);

std::string escapeBackslash(const std::string &str, const bool triple_quoted_string = false);

// Unescape backslash('\\' -> '\')
std::string unescapeBackslash(const std::string &str);

std::string escapeControlSequence(const std::string &str);

std::string unescapeControlSequence(const std::string &str);

std::string buildEscapedAndQuotedStringForUSDA(const std::string &str);

// TfIsValidIdentifier in pxrUSD equivalanet
inline bool isValidIdentifier(const std::string &str) {

  if (str.empty()) {
    return false;
  }

  // first char
  // [a-ZA-Z_]
  if ((('a' <= str[0]) && (str[0] <= 'z')) || (('A' <= str[0]) && (str[0] <= 'Z')) || (str[0] == '_')) {
    // ok
  } else {
    return false;
  }

  // remain chars
  // [a-ZA-Z0-9_]
  for (size_t i = 1; i < str.length(); i++) {
    if ((('a' <= str[i]) && (str[i] <= 'z')) || (('A' <= str[i]) && (str[i] <= 'Z')) || (('0' <= str[i]) && (str[i] <= '9')) || (str[i] == '_')) {
      // ok
    } else {
      return false;
    }
  }

  return true;
}


// TfMakeValidIdentifier in pxrUSD equivalanet
inline std::string makeIdentifierValid(const std::string &str) {
  std::string s;

  if (str.empty()) {
    // return '_'
    return "_";
  }

  // first char
  // [a-ZA-Z_]
  if ((('a' <= str[0]) && (str[0] <= 'z')) || (('A' <= str[0]) && (str[0] <= 'Z')) || (str[0] == '_')) {
    s.push_back(str[0]);
  } else {
    s.push_back('_');
  }

  // remain chars
  // [a-ZA-Z0-9_]
  for (size_t i = 1; i < str.length(); i++) {
    if ((('a' <= str[i]) && (str[i] <= 'z')) || (('A' <= str[i]) && (str[i] <= 'Z')) || (('0' <= str[i]) && (str[i] <= '9')) || (str[i] == '_')) {
      s.push_back(str[i]);
    } else {
      s.push_back('_');
    }
  }

  return s;
}

///
/// Simply add number suffix to make unique string.
///
/// - plane -> plane1 
/// - sphere1 -> sphere11 
/// - xform4 -> xform41 
///
///
bool makeUniqueName(std::multiset<std::string> &nameSet, const std::string &name, std::string *unique_name);


///
/// Convert string buffer to list of UTF-8 chars.
/// Example: 'こんにちは' => ['こ', 'ん', 'に', 'ち', 'は']
///
std::vector<std::string> to_utf8_chars(const std::string &str);

///
/// Convert UTF-8 char to code.
/// Return ~0u(0xffffffff) when input `u8char` is not a valid UTF-8 charcter.
///
uint32_t to_utf8_code(const std::string &u8char);

#if 0 // TODO
///
/// Convert UTF-8 code to UTF-8 char
///
/// Return empty string when input `code` is not a valid UTF-8 code.
std::string to_utf8_char(const uint32_t code);
#endif

#if 0
template<typename It>
inline std::string quote_then_join(const std::string& sep, const It& v, const std::string &quote = "\"")
{
  std::ostringstream oss;
  if (!v.empty()) {
    typename It::const_iterator it = v.begin();
    oss << wrap(*it++;
    for (typename It::const_iterator e = v.end(); it != e; ++it)
      oss << sep << *it;
  }
  return oss.str();
}
#endif

#if 0
template<typename It>
inline std::string join(const std::string& sep, It& v)
{
  std::ostringstream oss;
  if (!v.empty()) {
    typename It::iterator it = v.begin();
    oss << *it++;
    for (typename It::iterator e = v.end(); it != e; ++it)
      oss << sep << *it;
  }
  return oss.str();
}
#endif

}  // namespace tinyusdz
