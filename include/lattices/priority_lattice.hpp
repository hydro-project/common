//  Copyright 2019 U.C. Berkeley RISE Lab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#ifndef INCLUDE_LATTICES_PRIORITY_LATTICE_HPP_
#define INCLUDE_LATTICES_PRIORITY_LATTICE_HPP_

#include "core_lattices.hpp"

template <class P, class V>
struct PriorityValuePair {
  P priority;
  V value;

  // Initialize at a high value since the merge logic is taking the minimum
  PriorityValuePair(P p = INT_MAX, V v = {}) : priority(std::move(p)), value(std::move(v)) {}
  
  unsigned size() { return sizeof(P) + value.size(); }
};

template <class P, class V, class CompareP = std::less<P>, class CompareV = std::less<V>>
bool operator< (const PriorityValuePair<P,V>& a, const PriorityValuePair<P,V>& b) {
    CompareP compare_p;
    CompareV compare_v;
    if (compare_p(a.priority, b.priority)) {
      // return true if a.p < b.p
      return true;
    } else if (!compare_p(b.priority, a.priority) && compare_v(a.value, b.value)) {
      return true;
    } 
    return false;
}


template <class P, class V, class CompareP = std::less<P>, class CompareV = std::less<V>>
bool operator==(const PriorityValuePair<P,V>& a, const PriorityValuePair<P,V>& b) {
    CompareP compare_p;
    CompareV compare_v;
    if (!compare_p(a.priority, b.priority) && !compare_p(b.priority, a.priority) 
      && !compare_v(a.value, b.value) && !compare_v(b.value, a.value)) {
      return true;
    }
    return false;
}

template <class P, class V, class Compare = std::less<P>>
class PriorityLattice : public Lattice<PriorityValuePair<P, V>> {
  using Element = PriorityValuePair<P, V>;
  using Base = Lattice<Element>;

 protected:
  void do_merge(const Element& p) override {
    Compare compare;
    if (compare(p.priority, this->element.priority)) {
      this->element = p;
    }
  }

 public:
  PriorityLattice() : Base(Element()) {}

  PriorityLattice(const Element& p) : Base(p) {}

  MaxLattice<unsigned> size() { return {this->element.size()}; }
};

#endif
