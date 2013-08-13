#include "wavelet_tree.hpp"

#include <algorithm>
#include <iterator>
#include <set>
#include <sstream>
#include <utility>

wavelet_tree::wavelet_tree(const std::string &str) {
  std::set<char> set(str.begin(), str.end());
  size_t alphabet_size = set.size();
  int left_alphabet_size = alphabet_size / 2;
  std::stringstream left_ss, right_ss;
  std::for_each(str.begin(), str.end(),
                [left_alphabet_size, set, this, &left_ss, &right_ss](char c) {
                  auto set_idx = std::distance(set.begin(), set.find(c));
                  if (set_idx < left_alphabet_size) {
                    _bits.push_back(false);
                    left_ss << c;
                  } else {
                    _bits.push_back(true);
                    right_ss << c;
                  }
                });
  if (left_alphabet_size > 1) {
    _children[0].reset(new wavelet_tree(left_ss.str()));
  } else {
    _alphabets[0] = *set.begin();
  }
  if (alphabet_size - left_alphabet_size > 1) {
    _children[1].reset(new wavelet_tree(right_ss.str()));
  } else {
    auto mid = set.begin();
    std::advance(mid, left_alphabet_size);
    _alphabets[1] = *mid;
  }
}

/**
 * Passes operations though to underlying string iterator, unless the
 * underlying string is empty, in which case it is a repeating sequence of
 * a single char.  Used to reconstruct the string below.  Probably far
 * from the best way to do this.
 */
class maybe_iterator : public std::iterator<std::input_iterator_tag, char> {
  const bool _use_real_iterator;
  const char _default_val;
  std::string::const_iterator _cur;
  const std::string::const_iterator _end;
public:
  maybe_iterator(const std::string &src, const char &def) :
    _use_real_iterator(!src.empty()),
    _default_val(def),
    _cur(src.begin()),
    _end(src.end()) {}
  const char &operator*() {
    if (_use_real_iterator) {
      return *_cur;
    } else {
      return _default_val;
    }
  }
  maybe_iterator operator++(int) {
    maybe_iterator it(*this);
    if (_use_real_iterator) {
      _cur++;
    }
    return it;
  }
};

wavelet_tree::operator std::string() const {
  std::stringstream ss;
  const std::string &left_str = _children[0] ? (std::string) *_children[0] : "";
  const std::string &right_str = _children[1] ? (std::string) *_children[1] : "";
  maybe_iterator left_it(left_str, _alphabets[0]);
  maybe_iterator right_it(right_str, _alphabets[1]);
  std::for_each(_bits.begin(), _bits.end(),
                [&ss, &left_it, &right_it, this](bool bit) {
                  if (!bit) {
                    ss << *left_it++;
                  } else {
                    ss << *right_it++;
                  }
                });
  return ss.str();
}

#if WT_DEBUG

#include <iostream>

void wavelet_tree::dprint(size_t depth) const {
  std::stringstream pfx_ss;
  for (size_t i = 0; i < depth; ++i) { pfx_ss << "\t"; }
  std::string pfx = pfx_ss.str();

  std::cout << pfx;
  std::copy(_bits.begin(), _bits.end(), std::ostream_iterator<int>(std::cout));
  std::cout << std::endl;
  std::cout << pfx;
  std::copy(_alphabets[0].begin(), _alphabets[0].end(), std::ostream_iterator<char>(std::cout));
  std::cout << " ";
  std::copy(_alphabets[1].begin(), _alphabets[1].end(), std::ostream_iterator<char>(std::cout));
  std::cout << std::endl;

  if (_children[0]) {
    std::cout << pfx << "left:" << std::endl;
    _children[0]->dprint(depth + 1);
  }
  if (_children[1]) {
    std::cout << pfx << "right:" << std::endl;
    _children[1]->dprint(depth + 1);
  }    
}
#endif
