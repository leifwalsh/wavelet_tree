/**
 * An implementation of a wavelet tree
 * (http://en.wikipedia.org/wiki/Wavelet_Tree) for strings.  Only encodes
 * and decodes, doesn't do any of the interesting operations.  Also
 * probably not very efficient.
 */

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

class wavelet_tree {
  std::vector<bool> _bits;
  std::unique_ptr<wavelet_tree> _children[2];
  char _alphabets[2];
public:
  wavelet_tree(const std::string &str);
  operator std::string() const;

  void dprint(size_t depth=0) const
#if WT_DEBUG
    ;
#else
    {}
#endif
};
